// Player.h

#pragma once

#include <iosfwd>
#include <string>
#include <vector>

class Territory;    // from Map.h
class Hand;         // from Cards.h
class OrdersList;   // from Orders.h
class Order;        // from Orders.h

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

    // helper used by game to give player territories
    void addTerritory(Territory* t);

    // getters 
    std::string getName() const;
    Hand* getHand() const;
    OrdersList* getOrdersList() const;
    std::vector<Territory*>* getTerritories() const;

    // stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Player& p);

private:
    // pointer data members 
    std::string* name;                      
    std::vector<Territory*>* territories;  
    Hand* hand;                             
    OrdersList* orders;                     

private:
    // Rule of 3 helpers
    void destroy();
    void copyFrom(const Player& other);
};
