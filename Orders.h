#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "LoggingObserver.h"

class Player;
class Territory;
class AirliftCard;
class BombCard;
class BlockadeCard;
class DiplomacyCard;

// =======================================================
// Base Class: Order
// =======================================================

class Order : public Subject, public ILoggable {
protected:
    std::string orderType;
    std::string effect;

public:
    // Constructors & Rule of Three
    Order(const std::string& type);
    Order(const Order& other);
    Order& operator=(const Order& other);
    virtual ~Order();

    // Getters
    std::string getOrderType() const;
    std::string getEffect() const;

    // Core behavior
    virtual bool validate() = 0;
    virtual bool execute() = 0;

    // Polymorphic copy
    virtual Order* clone() const = 0;
virtual std::string stringToLog() const = 0;
    // Stream insertion
    friend std::ostream& operator<<(std::ostream& out, const Order& order);
};

// =======================================================
// Derived Classes
// =======================================================
//-------------- DEPLOY --------------
class Deploy : public Order {
public:
    Deploy();
    Deploy(Player* issuer, Territory* target, int numArmies);
    Deploy(const Deploy& other);
    Deploy& operator=(const Deploy& other);
    ~Deploy();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
     std::string stringToLog() const override;

private:
    Player* issuer;
    Territory* target;
    int numArmies;
};

//-------------- ADVANCE --------------
class Advance : public Order {
public:
    Advance();
    Advance(Player* issuer, Territory* source, Territory* target, int numArmies);
    Advance(const Advance& other);
    Advance& operator=(const Advance& other);
    ~Advance();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
     std::string stringToLog() const override;

private:
    Player* issuer;
    Territory* source;
    Territory* target;
    int numArmies;
};

//-------------- BOMB -----------------
class Bomb : public Order {
public:
    Bomb(const Bomb& other);
    Bomb& operator=(const Bomb& other);
    ~Bomb();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
     std::string stringToLog() const override;

    void setIssuer(Player* p);
    void setTarget(Territory* t);

private:
    friend class BombCard;
    Bomb();
    Bomb(Player* issuer, Territory* target);

    Player* issuer;
    Territory* target;
};

//-------------- BLOCKADE -----------------
class Blockade : public Order {
public:
    Blockade(const Blockade& other);
    Blockade& operator=(const Blockade& other);
    ~Blockade();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
     std::string stringToLog() const override;

    void setIssuer(Player* p);
    void setTarget(Territory* t);

private:
    friend class BlockadeCard;
    Blockade();
    Blockade(Player* issuer, Territory* target);

    Player* issuer;
    Territory* target;
};

//-------------- AIRLIFT ------------------
class Airlift : public Order {
public:
    Airlift(const Airlift& other);
    Airlift& operator=(const Airlift& other);
    ~Airlift();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
     std::string stringToLog() const override;

    void setIssuer(Player* p);
    void setSource(Territory* t);
    void setTarget(Territory* t);
    void setNumArmies(int n);

private:
    friend class AirliftCard;
    Airlift();
    Airlift(Player* issuer, Territory* source, Territory* target, int numArmies);

    Player* issuer;
    Territory* source;
    Territory* target;
    int numArmies;
};

//-------------- NEGOTIATE -------------------
class Negotiate : public Order {
public:
    Negotiate(const Negotiate& other);
    Negotiate& operator=(const Negotiate& other);
    ~Negotiate();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
     std::string stringToLog() const override;

    void setIssuer(Player* p);
    void setTarget(Player* p);

private:
    friend class DiplomacyCard;      // Only the DiplomacyCard class can create a Negotiate order
    Negotiate();
    Negotiate(Player* issuer, Player* target);

    Player* issuer;
    Player* target;
};

// =======================================================
// OrdersList Class (Container)
// =======================================================

class OrdersList : public Subject, public ILoggable  {
private:
    std::vector<Order*> orders;

public:
    // Rule of Three
    OrdersList();
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    ~OrdersList();

    // Core functionality
    void addOrder(Order* order);
    void remove(int index);
    void move(int fromIndex, int toIndex);

    Order* getOrder(int index) const;
    int size() const;

    void printOrders() const;
std::string stringToLog() const;
    // Stream insertion
    friend std::ostream& operator<<(std::ostream& out, const OrdersList& list);
};
