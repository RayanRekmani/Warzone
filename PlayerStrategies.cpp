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
}

CheaterPlayerStrategies::CheaterPlayerStrategies(Player* player) {
    this->player = player;
}

CheaterPlayerStrategies::CheaterPlayerStrategies(const CheaterPlayerStrategies &other) {
    this->player = other.player;
}

CheaterPlayerStrategies& CheaterPlayerStrategies::operator=(const CheaterPlayerStrategies &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

CheaterPlayerStrategies::~CheaterPlayerStrategies() = default;

vector<Territory*> CheaterPlayerStrategies::toAttack() {
    return {};
}

vector<Territory*> CheaterPlayerStrategies::toDefend() {
    return {};
}

bool CheaterPlayerStrategies::issueOrder(bool inDeployPhase) {
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
