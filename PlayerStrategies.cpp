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

vector<Territory *> AggressivePlayerStrategies::toAttack() {
    return {};
}

vector<Territory *> AggressivePlayerStrategies::toDefend() {
    return {};
}

bool AggressivePlayerStrategies::issueOrder(bool inDeployPhase) {
    return true;
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

vector<Territory*> HumanPlayerStrategies::toAttack() {
    return {};
}

vector<Territory*> HumanPlayerStrategies::toDefend() {
    return {};
}

bool HumanPlayerStrategies::issueOrder(bool inDeployPhase) {
    return true;
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
