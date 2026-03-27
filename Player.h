// Player.h

#pragma once

#include <iosfwd>
#include <string>
#include <vector>

class Territory;    // from Map.h
class Hand;         // from Cards.h
class OrdersList;   // from Orders.h
class Order;        // from Orders.h
class PlayerStrategies; // from PlayerStrategies.h
//ADDED LINE FOR GAMEENGINE PART 3
class Deck;

class Player {
public:
    // Constructors 
    Player();
    Player(const std::string& name);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    // return list of territories
    std::vector<Territory*>* toDefend() const;
    std::vector<Territory*>* toAttack() const;

    // add order to orders list
    void issueOrder(Order* order);

    bool issueOrder(bool inDeployPhase);

    //ADDED LINE FOR GAMEENGINE PART 3
    bool issueOrder(Deck* deck, const std::vector<Player*>& allPlayers);

    void setPlayerStrategies(PlayerStrategies* playerStrategies);
    PlayerStrategies* getPlayerStrategies() const;

    // helper used by game to give player territories
    void addTerritory(Territory* t);
    void removeTerritory(Territory* t);

    // getters 
    std::string getName() const;
    Hand* getHand() const;
    OrdersList* getOrdersList() const;
    std::vector<Territory*>* getTerritories() const;
    int getReinforcementPool() const;
    bool hasConqueredTerritoryThisTurn() const;
    bool isNegotiatingWith(Player* other) const;

    // setters
    void setReinforcementPool(int);
    void setConqueredTerritoryThisTurn(bool value);
    void addNegotiatedPlayer(Player* other);
    void clearNegotiatedPlayers();

    // stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Player& p);

private:
    // pointer data members 
    std::string* name;                      
    std::vector<Territory*>* territories;  
    Hand* hand;                             
    OrdersList* orders;                     
    int reinforcementPool;        
    bool conqueredTerritoryThisTurn;
    std::vector<Player*> negotiatedPlayers;
    PlayerStrategies* strategy;

private:
    // Rule of 3 helpers
    void destroy();
    void copyFrom(const Player& other);
};
