#pragma once
#include <iostream>
#include <vector>
#include <string>

// =======================================================
// Base Class: Order
// =======================================================

class Order {
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

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& out, const Order& order);
};

// =======================================================
// Derived Classes
// =======================================================

class Deploy : public Order {
public:
    Deploy();
    Deploy(const Deploy& other);
    Deploy& operator=(const Deploy& other);
    ~Deploy();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
};

class Advance : public Order {
public:
    Advance();
    Advance(const Advance& other);
    Advance& operator=(const Advance& other);
    ~Advance();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
};

class Bomb : public Order {
public:
    Bomb();
    Bomb(const Bomb& other);
    Bomb& operator=(const Bomb& other);
    ~Bomb();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
};

class Blockade : public Order {
public:
    Blockade();
    Blockade(const Blockade& other);
    Blockade& operator=(const Blockade& other);
    ~Blockade();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
};

class Airlift : public Order {
public:
    Airlift();
    Airlift(const Airlift& other);
    Airlift& operator=(const Airlift& other);
    ~Airlift();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
};

class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const Negotiate& other);
    Negotiate& operator=(const Negotiate& other);
    ~Negotiate();

    bool validate() override;
    bool execute() override;
    Order* clone() const override;
};

// =======================================================
// OrdersList Class (Container)
// =======================================================

class OrdersList {
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

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& out, const OrdersList& list);
};
