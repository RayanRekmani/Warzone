//
// Created by Rayan Rekmani on 2026-03-18.
//

#include "PlayerStrategies.h"
#include "Player.h"
#include "Orders.h"

#include <algorithm>
#include <iostream>
#include <ostream>

PlayerStrategies::~PlayerStrategies()= default;

Player *PlayerStrategies::getPlayer() {
    return player;
}

void PlayerStrategies::setPlayer(Player* player) {
    this->player = player;
}

NeutralPlayerStrategies::NeutralPlayerStrategies() {
    player = nullptr;
}

NeutralPlayerStrategies::NeutralPlayerStrategies(Player *player) {
    this->player = player;
}

NeutralPlayerStrategies::NeutralPlayerStrategies(const NeutralPlayerStrategies &other) {
    this->player = other.player;
}

NeutralPlayerStrategies &NeutralPlayerStrategies::operator=(const NeutralPlayerStrategies &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

NeutralPlayerStrategies::~NeutralPlayerStrategies() = default;

vector<Territory *> NeutralPlayerStrategies::toAttack() {
    // Neutral strategy never initiates attacks.
    return vector<Territory*>();
}

vector<Territory *> NeutralPlayerStrategies::toDefend() {
    vector<Territory*> defendList;

    if (player == nullptr || player->getTerritories() == nullptr) {
        return defendList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t != nullptr && t->getTerritoryOwner() == player) {
            defendList.push_back(t);
        }
    }

    return defendList;
}

bool NeutralPlayerStrategies::issueOrder(bool inDeployPhase) {
    (void)inDeployPhase; // Neutral strategy does not issue any orders.
    return false;
}

string NeutralPlayerStrategies::getStrategyType() {
    return "Neutral";
}

AggressivePlayerStrategies::AggressivePlayerStrategies() {
    player = nullptr;
}

AggressivePlayerStrategies::AggressivePlayerStrategies(Player *player) {
    this->player = player;
}

AggressivePlayerStrategies::AggressivePlayerStrategies(const AggressivePlayerStrategies &other) {
    this->player = other.player;
}

AggressivePlayerStrategies& AggressivePlayerStrategies::operator=(const AggressivePlayerStrategies &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

AggressivePlayerStrategies::~AggressivePlayerStrategies() = default;

//new modification: aggressive strategy attacks enemy territories adjacent to its strongest territory
vector<Territory *> AggressivePlayerStrategies::toAttack() {
    vector<Territory*> attackList;

    if (player == nullptr || player->getTerritories() == nullptr || player->getTerritories()->empty()) {
        return attackList;
    }

    Territory* strongest = nullptr;
    int maxArmies = -1;

    for (Territory* t : *player->getTerritories()) {
        if (t != nullptr && t->getTerritoryOwner() == player && t->getArmySize() > maxArmies) {
            strongest = t;
            maxArmies = t->getArmySize();
        }
    }

    if (strongest == nullptr) {
        return attackList;
    }

    vector<Territory*> neighbours = strongest->getNeighbours();
    for (Territory* neighbour : neighbours) {
        if (neighbour != nullptr && neighbour->getTerritoryOwner() != player) {
            attackList.push_back(neighbour);
        }
    }

    return attackList;
}

//new modification: aggressive strategy defends by prioritizing strongest territories first
vector<Territory *> AggressivePlayerStrategies::toDefend() {
    vector<Territory*> defendList;

    if (player == nullptr || player->getTerritories() == nullptr) {
        return defendList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t != nullptr && t->getTerritoryOwner() == player) {
            defendList.push_back(t);
        }
    }

    sort(defendList.begin(), defendList.end(), [](Territory* a, Territory* b) {
        if (a->getArmySize() != b->getArmySize()) {
            return a->getArmySize() > b->getArmySize();
        }
        return a->getTerritoryName() < b->getTerritoryName();
    });

    return defendList;
}

//new modification: aggressive strategy deploys everything to its strongest territory and attacks from it
bool AggressivePlayerStrategies::issueOrder(bool inDeployPhase) {
    if (player == nullptr) {
        return false;
    }

    OrdersList* orders = player->getOrdersList();
    if (orders == nullptr) {
        return false;
    }

    vector<Territory*> defendList = toDefend();
    if (defendList.empty()) {
        return false;
    }

    Territory* strongest = defendList.front();
    if (strongest == nullptr) {
        return false;
    }

    if (inDeployPhase) {
        int reinforcementPool = player->getReinforcementPool();
        if (reinforcementPool <= 0) {
            return false;
        }

        orders->addOrder(new Deploy(player, strongest, reinforcementPool));
        return true;
    }

    vector<Territory*> attackList = toAttack();
    for (Territory* target : attackList) {
        if (target == nullptr) {
            continue;
        }

        if (strongest->isNeighbour(target) && strongest->getArmySize() > 1) {
            int armiesToMove = strongest->getArmySize() / 2;
            if (armiesToMove > 0) {
                orders->addOrder(new Advance(player, strongest, target, armiesToMove));
                return true;
            }
        }
    }

    return false;
}

string AggressivePlayerStrategies::getStrategyType() {
    return "Aggressive";
}

BenevolentPlayerStrategies::BenevolentPlayerStrategies() {
    this->player = nullptr;
}

BenevolentPlayerStrategies::BenevolentPlayerStrategies(Player *player) {
    this->player = player;
}

BenevolentPlayerStrategies::BenevolentPlayerStrategies(const BenevolentPlayerStrategies &other) {
    this->player = other.player;
}

BenevolentPlayerStrategies& BenevolentPlayerStrategies::operator=(const BenevolentPlayerStrategies &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

BenevolentPlayerStrategies::~BenevolentPlayerStrategies() =default;

vector<Territory *> BenevolentPlayerStrategies::toAttack() {
    // Benevolent strategy does not initiate attacks.
    return vector<Territory*>();
}

vector<Territory*> BenevolentPlayerStrategies::toDefend() {
    vector<Territory*> defendList;
    if (player == nullptr || player->getTerritories() == nullptr) {
        return defendList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t != nullptr && t->getTerritoryOwner() == player) {
            defendList.push_back(t);
        }
    }

    // Benevolent strategy prioritizes weakest territories first.
    sort(defendList.begin(), defendList.end(), [](Territory* a, Territory* b) {
        if (a->getArmySize() != b->getArmySize()) {
            return a->getArmySize() < b->getArmySize();
        }
        return a->getTerritoryName() < b->getTerritoryName();
    });

    return defendList;
}

bool BenevolentPlayerStrategies::issueOrder(bool inDeployPhase) {
    if (player == nullptr) {
        return false;
    }

    OrdersList* orders = player->getOrdersList();
    if (orders == nullptr) {
        return false;
    }

    vector<Territory*> defendList = toDefend();
    if (defendList.empty()) {
        return false;
    }

    if (inDeployPhase) {
        int reinforcementPool = player->getReinforcementPool();
        if (reinforcementPool <= 0) {
            return false;
        }

        Territory* weakest = defendList.front();
        if (weakest == nullptr) {
            return false;
        }

        orders->addOrder(new Deploy(player, weakest, reinforcementPool));
        return true;
    }

    // Advance phase: only reinforce own weaker territories. Never attack.
    for (Territory* target : defendList) {
        if (target == nullptr) {
            continue;
        }

        Territory* bestSource = nullptr;
        int bestSourceArmies = 0;

        for (Territory* source : *player->getTerritories()) {
            if (source == nullptr || source == target) {
                continue;
            }
            if (source->getTerritoryOwner() != player || target->getTerritoryOwner() != player) {
                continue;
            }
            if (!source->isNeighbour(target)) {
                continue;
            }
            if (source->getArmySize() <= 1) {
                continue;
            }

            if (source->getArmySize() > bestSourceArmies) {
                bestSource = source;
                bestSourceArmies = source->getArmySize();
            }
        }

        if (bestSource != nullptr) {
            const int armiesToMove = (bestSourceArmies - 1) / 2;
            if (armiesToMove > 0) {
                orders->addOrder(new Advance(player, bestSource, target, armiesToMove));
                return true;
            }
        }
    }

    return false;
}

string BenevolentPlayerStrategies::getStrategyType() {
    return "Benevolent";
}

HumanPlayerStrategies::HumanPlayerStrategies() {
    this->player = nullptr;
}

HumanPlayerStrategies::HumanPlayerStrategies(Player *player) {
    this->player = player;
}

HumanPlayerStrategies::HumanPlayerStrategies(const HumanPlayerStrategies &other) {
    this->player = other.player;
}

HumanPlayerStrategies& HumanPlayerStrategies::operator=(const HumanPlayerStrategies &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

HumanPlayerStrategies::~HumanPlayerStrategies() = default;

//new modification: human strategy returns all adjacent enemy territories
vector<Territory*> HumanPlayerStrategies::toAttack() {
    vector<Territory*> attackList;

    if (player == nullptr || player->getTerritories() == nullptr) {
        return attackList;
    }

    for (Territory* owned : *player->getTerritories()) {
        if (owned == nullptr || owned->getTerritoryOwner() != player) {
            continue;
        }

        vector<Territory*> neighbours = owned->getNeighbours();
        for (Territory* neighbour : neighbours) {
            if (neighbour != nullptr && neighbour->getTerritoryOwner() != player) {
                if (find(attackList.begin(), attackList.end(), neighbour) == attackList.end()) {
                    attackList.push_back(neighbour);
                }
            }
        }
    }

    return attackList;
}

//new modification: human strategy returns all owned territories for the user to choose from
vector<Territory*> HumanPlayerStrategies::toDefend() {
    vector<Territory*> defendList;

    if (player == nullptr || player->getTerritories() == nullptr) {
        return defendList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t != nullptr && t->getTerritoryOwner() == player) {
            defendList.push_back(t);
        }
    }

    return defendList;
}

//new modification: helper for human strategy to let user choose a territory
Territory* HumanPlayerStrategies::chooseTerritories(vector<Territory*> territories) {
    if (territories.empty()) {
        return nullptr;
    }

    cout << "Choose a territory:" << endl;
    for (int i = 0; i < territories.size(); i++) {
        if (territories[i] != nullptr) {
            cout << i << ": " << territories[i]->getTerritoryName()
                 << " (armies: " << territories[i]->getArmySize() << ")" << endl;
        }
    }

    int choice;
    cin >> choice;

    if (choice < 0 || choice >= territories.size()) {
        cout << "Invalid choice." << endl;
        return nullptr;
    }

    return territories[choice];
}

//new modification: helper for human strategy to let user choose number of armies
int HumanPlayerStrategies::chooseArmies(Territory* territory) {
    if (territory == nullptr) {
        return 0;
    }

    cout << "Enter number of armies for " << territory->getTerritoryName() << ": ";
    int armies;
    cin >> armies;

    if (armies < 0) {
        armies = 0;
    }

    return armies;
}

//new modification: helper for human strategy deploy phase
bool HumanPlayerStrategies::issueDeployOrder() {
    if (player == nullptr) {
        return false;
    }

    int reinforcementPool = player->getReinforcementPool();
    if (reinforcementPool <= 0) {
        return false;
    }

    vector<Territory*> defendList = toDefend();
    if (defendList.empty()) {
        return false;
    }

    cout << "\n[Human] Deploy phase for " << player->getName() << endl;
    Territory* target = chooseTerritories(defendList);
    if (target == nullptr) {
        return false;
    }

    int armies = chooseArmies(target);
    if (armies <= 0 || armies > reinforcementPool) {
        armies = reinforcementPool;
    }

    player->getOrdersList()->addOrder(new Deploy(player, target, armies));
    return true;
}

//new modification: helper for human strategy advance phase
void HumanPlayerStrategies::issueAdvanceOrder() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return;
    }

    cout << "\n[Human] Advance phase for " << player->getName() << endl;
    cout << "Choose source territory:" << endl;

    vector<Territory*> owned = *player->getTerritories();
    Territory* source = chooseTerritories(owned);
    if (source == nullptr || source->getArmySize() <= 1) {
        cout << "Invalid source territory." << endl;
        return;
    }

    vector<Territory*> possibleTargets = source->getNeighbours();
    if (possibleTargets.empty()) {
        cout << "No neighbouring territories." << endl;
        return;
    }

    cout << "Choose target territory:" << endl;
    Territory* target = chooseTerritories(possibleTargets);
    if (target == nullptr) {
        return;
    }

    int maxPossible = source->getArmySize() - 1;
    cout << "Enter number of armies to advance (max " << maxPossible << "): ";
    int armies;
    cin >> armies;

    if (armies <= 0 || armies > maxPossible) {
        cout << "Invalid number of armies." << endl;
        return;
    }

    player->getOrdersList()->addOrder(new Advance(player, source, target, armies));
}

//ew modification: human strategy asks the user what to do instead of acting automatically
bool HumanPlayerStrategies::issueOrder(bool inDeployPhase) {
    if (player == nullptr) {
        return false;
    }

    if (inDeployPhase) {
        return issueDeployOrder();
    }

    cout << "\n[Human] Do you want to issue an advance order? (1 = yes, 0 = no): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        issueAdvanceOrder();
        return true;
    }

    return false;
}

string HumanPlayerStrategies::getStrategyType() {
    return "Human";
}

CheaterPlayerStrategies::CheaterPlayerStrategies() {
    this->player = nullptr;
    conqueredThisTurn = false;
}

CheaterPlayerStrategies::CheaterPlayerStrategies(Player* player) {
    this->player = player;
    conqueredThisTurn = false;
}

CheaterPlayerStrategies::CheaterPlayerStrategies(const CheaterPlayerStrategies &other) {
    this->player = other.player;
    conqueredThisTurn = other.conqueredThisTurn;
}

CheaterPlayerStrategies& CheaterPlayerStrategies::operator=(const CheaterPlayerStrategies &other) {
    if (this != &other) {
        this->player = other.player;
        this->conqueredThisTurn = other.conqueredThisTurn;
    }
    return *this;
}

CheaterPlayerStrategies::~CheaterPlayerStrategies() = default;

vector<Territory*> CheaterPlayerStrategies::toAttack() {
    vector<Territory*> attackList;
    if (player == nullptr || player->getTerritories() == nullptr) {
        return attackList;
    }

    for (Territory* owned : *player->getTerritories()) {
        if (owned == nullptr || owned->getTerritoryOwner() != player) {
            continue;
        }

        vector<Territory*> neighbours = owned->getNeighbours();
        for (Territory* neighbour : neighbours) {
            if (neighbour == nullptr || neighbour->getTerritoryOwner() == player) {
                continue;
            }
            if (find(attackList.begin(), attackList.end(), neighbour) == attackList.end()) {
                attackList.push_back(neighbour);
            }
        }
    }

    return attackList;
}

vector<Territory*> CheaterPlayerStrategies::toDefend() {
    vector<Territory*> defendList;
    if (player == nullptr || player->getTerritories() == nullptr) {
        return defendList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t != nullptr && t->getTerritoryOwner() == player) {
            defendList.push_back(t);
        }
    }

    return defendList;
}

bool CheaterPlayerStrategies::issueOrder(bool inDeployPhase) {
    if (player == nullptr) {
        return false;
    }

    if (inDeployPhase) {
        conqueredThisTurn = false;
        return false;
    }

    if (conqueredThisTurn) {
        return false;
    }

    vector<Territory*> targets = toAttack();
    if (targets.empty()) {
        conqueredThisTurn = true;
        return false;
    }

    for (Territory* target : targets) {
        if (target == nullptr) {
            continue;
        }

        Player* previousOwner = target->getTerritoryOwner();
        if (previousOwner != nullptr && previousOwner != player) {
            previousOwner->removeTerritory(target);
        }

        target->setTerritoryOwner(player);
        player->addTerritory(target);
    }

    conqueredThisTurn = true;
    player->setConqueredTerritoryThisTurn(true);
    return true;
}

string CheaterPlayerStrategies::getStrategyType() {
    return "Cheater";
}

void printTerritoryVector(vector<Territory *> &v) {
    for (auto t : v) {
        cout << t->getTerritoryName()<<" ";
    }
    cout << endl;
}