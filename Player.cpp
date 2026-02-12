// Player.cpp

#include "Player.h"

#include <iostream>


#include "Map.h"     // Territory, Continent
#include "Cards.h"   // Hand
#include "Orders.h"  // Order, OrdersList, Deploy...


Player::Player()
    : name(new std::string("anonymous"))
    , territories(new std::vector<Territory*>())
    , hand(new Hand(this))          // link hand to this player 
    , orders(new OrdersList()) {
}

Player::Player(const std::string& n)
    : name(new std::string(n))
    , territories(new std::vector<Territory*>())
    , hand(new Hand(this))          // link hand to this player
    , orders(new OrdersList()) {
}

Player::Player(const Player& other)
    : name(nullptr)
    , territories(nullptr)
    , hand(nullptr)
    , orders(nullptr) {
    copyFrom(other);
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        destroy();
        copyFrom(other);
    }
    return *this;
}

Player::~Player() {
    destroy();
}


//defend the first territory in the list 
std::vector<Territory*>* Player::toDefend() const {
    std::vector<Territory*>* result = new std::vector<Territory*>();

    if (territories != nullptr && !territories->empty()) {
        result->push_back((*territories)[0]);
    }

    return result;
}

// attack the last territory in the list 
std::vector<Territory*>* Player::toAttack() const {
    std::vector<Territory*>* result = new std::vector<Territory*>();

    if (territories != nullptr && !territories->empty()) {
        result->push_back((*territories)[territories->size() - 1]);
    }

    return result;
}

// Adds an order to player's orders list.
void Player::issueOrder(Order* order) {
    if (order == nullptr) {
        std::cout << "[Player] Can't issue a null order\n";
        return;
    }

    orders->addOrder(order);
}

//Adds a Territory* to player's collection.

void Player::addTerritory(Territory* t) {
    if (t == nullptr) {
        return;
    }
    territories->push_back(t);
}


std::string Player::getName() const {
    return *name;
}

Hand* Player::getHand() const {
    return hand;
}

OrdersList* Player::getOrdersList() const {
    return orders;
}

std::vector<Territory*>* Player::getTerritories() const {
    return territories;
}

// stream output
std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player{name=" << p.getName()
       << ", territories=" << p.territories->size()
       << "}";
    return os;
}

// destroy(): deletes memory owned by Player
void Player::destroy() {
    delete name;
    name = nullptr;

    delete territories;
    territories = nullptr;

    delete hand;
    hand = nullptr;

    delete orders;
    orders = nullptr;
}

//copies values from another Player
void Player::copyFrom(const Player& other) {
    name = new std::string(*other.name);

    territories = new std::vector<Territory*>();
    *territories = *other.territories;  // copy pointer values only

    hand = new Hand(*other.hand);       // uses Hand copy constructor
    hand->setPlayer(this);              // relink copied hand to this new Player

    orders = new OrdersList(*other.orders); // uses OrdersList copy constructor
}
