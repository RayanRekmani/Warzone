//
// Created by Rayan Rekmani on 2026-03-18.
//

#ifndef UNTITLED_PLAYERSTRATEGIES_H
#define UNTITLED_PLAYERSTRATEGIES_H

#include "Map.h"

//forward declaration
class Player;

class PlayerStrategies {
public:
    virtual vector<Territory*> toAttack() = 0;

    virtual vector<Territory*> toDefend() = 0;
    //true deploys phase false advances phase
    virtual  bool issueOrder(bool inDeployPhase) = 0;

    virtual string getStrategyType() = 0;
    virtual ~PlayerStrategies();
    virtual Player* getPlayer();
    virtual void setPlayer(Player* player);
protected:
    Player* player;
};

class NeutralPlayerStrategies : public PlayerStrategies {
    public:
    NeutralPlayerStrategies();
    NeutralPlayerStrategies(Player* player);
    NeutralPlayerStrategies(const NeutralPlayerStrategies& other);
    NeutralPlayerStrategies& operator=(const NeutralPlayerStrategies& other);
    ~NeutralPlayerStrategies() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    bool issueOrder(bool inDeployPhase) override;
    string getStrategyType() override;
    private:
    friend ostream& operator<<(ostream& os, const NeutralPlayerStrategies& other);
};

class AggressivePlayerStrategies : public PlayerStrategies {
    public:
    AggressivePlayerStrategies();
    AggressivePlayerStrategies(Player* player);
    AggressivePlayerStrategies(const AggressivePlayerStrategies& other);
    AggressivePlayerStrategies& operator=(const AggressivePlayerStrategies& other);
    ~AggressivePlayerStrategies() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    bool issueOrder(bool inDeployPhase) override;
    string getStrategyType() override;
    private:
    friend ostream& operator<<(ostream& os, const AggressivePlayerStrategies& other);
};

class BenevolentPlayerStrategies : public PlayerStrategies {
    public:
    BenevolentPlayerStrategies();
    BenevolentPlayerStrategies(Player* player);
    BenevolentPlayerStrategies(const BenevolentPlayerStrategies& other);
    BenevolentPlayerStrategies& operator=(const BenevolentPlayerStrategies& other);
    ~BenevolentPlayerStrategies() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    bool issueOrder(bool inDeployPhase) override;
    string getStrategyType() override;
private:
    friend ostream& operator<<(ostream& os, const BenevolentPlayerStrategies& other);
};

class HumanPlayerStrategies : public PlayerStrategies {
    public:
    HumanPlayerStrategies();
    HumanPlayerStrategies(Player* player);
    HumanPlayerStrategies(const HumanPlayerStrategies& other);
    HumanPlayerStrategies& operator=(const HumanPlayerStrategies& other);
    ~HumanPlayerStrategies() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    bool issueOrder(bool inDeployPhase) override;
    string getStrategyType() override;
private:
    bool issueDeployOrder();
    void issueAdvanceOrder();
    Territory* chooseTerritories(vector<Territory*> territories);
    int chooseArmies(Territory* territory);
    friend ostream& operator<<(ostream& os, const HumanPlayerStrategies& other);
};

class CheaterPlayerStrategies : public PlayerStrategies {
    public:
    CheaterPlayerStrategies();
    CheaterPlayerStrategies(Player* player);
    CheaterPlayerStrategies(const CheaterPlayerStrategies& other);
    CheaterPlayerStrategies& operator=(const CheaterPlayerStrategies& other);
    ~CheaterPlayerStrategies() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    bool issueOrder(bool inDeployPhase) override;
    string getStrategyType() override;
    private:
    bool conqueredThisTurn;
    friend ostream& operator<<(ostream& os, const CheaterPlayerStrategies& other);
};

void printTerritoryVector(vector<Territory*>& v);


#endif //UNTITLED_PLAYERSTRATEGIES_H
