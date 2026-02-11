#include "Orders.h"
#include <stdexcept>

// =======================================================
// Order Base Class Implementation
// =======================================================

Order::Order(const std::string& type) : orderType(type), effect("") {}

Order::Order(const Order& other) {
    orderType = other.orderType;
    effect = other.effect;
}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        orderType = other.orderType;
        effect = other.effect;
    }
    return *this;
}

Order::~Order() {}

std::string Order::getOrderType() const {
    return orderType;
}

std::string Order::getEffect() const {
    return effect;
}

std::ostream& operator<<(std::ostream& out, const Order& order) {
    out << order.orderType;
    if (!order.effect.empty()) {
        out << " | Effect: " << order.effect;
    }
    return out;
}

// =======================================================
// Deploy
// =======================================================

Deploy::Deploy() : Order("Deploy") {}

Deploy::Deploy(const Deploy& other) : Order(other) {}

Deploy& Deploy::operator=(const Deploy& other) {
    Order::operator=(other);
    return *this;
}

Deploy::~Deploy() {}

bool Deploy::validate() {
    return true; // placeholder for Assignment 1
}

bool Deploy::execute() {
    if (!validate()) {
        effect = "Deploy order is invalid.";
        return false;
    }
    effect = "Deploy order executed.";
    return true;
}

Order* Deploy::clone() const {
    return new Deploy(*this);
}

// =======================================================
// Advance
// =======================================================

Advance::Advance() : Order("Advance") {}

Advance::Advance(const Advance& other) : Order(other) {}

Advance& Advance::operator=(const Advance& other) {
    Order::operator=(other);
    return *this;
}

Advance::~Advance() {}

bool Advance::validate() {
    return true;
}

bool Advance::execute() {
    if (!validate()) {
        effect = "Advance order is invalid.";
        return false;
    }
    effect = "Advance order executed.";
    return true;
}

Order* Advance::clone() const {
    return new Advance(*this);
}

// =======================================================
// Bomb
// =======================================================

Bomb::Bomb() : Order("Bomb") {}

Bomb::Bomb(const Bomb& other) : Order(other) {}

Bomb& Bomb::operator=(const Bomb& other) {
    Order::operator=(other);
    return *this;
}

Bomb::~Bomb() {}

bool Bomb::validate() {
    return true;
}

bool Bomb::execute() {
    if (!validate()) {
        effect = "Bomb order is invalid.";
        return false;
    }
    effect = "Bomb order executed.";
    return true;
}

Order* Bomb::clone() const {
    return new Bomb(*this);
}

// =======================================================
// Blockade
// =======================================================

Blockade::Blockade() : Order("Blockade") {}

Blockade::Blockade(const Blockade& other) : Order(other) {}

Blockade& Blockade::operator=(const Blockade& other) {
    Order::operator=(other);
    return *this;
}

Blockade::~Blockade() {}

bool Blockade::validate() {
    return true;
}

bool Blockade::execute() {
    if (!validate()) {
        effect = "Blockade order is invalid.";
        return false;
    }
    effect = "Blockade order executed.";
    return true;
}

Order* Blockade::clone() const {
    return new Blockade(*this);
}

// =======================================================
// Airlift
// =======================================================

Airlift::Airlift() : Order("Airlift") {}

Airlift::Airlift(const Airlift& other) : Order(other) {}

Airlift& Airlift::operator=(const Airlift& other) {
    Order::operator=(other);
    return *this;
}

Airlift::~Airlift() {}

bool Airlift::validate() {
    return true;
}

bool Airlift::execute() {
    if (!validate()) {
        effect = "Airlift order is invalid.";
        return false;
    }
    effect = "Airlift order executed.";
    return true;
}

Order* Airlift::clone() const {
    return new Airlift(*this);
}

// =======================================================
// Negotiate
// =======================================================

Negotiate::Negotiate() : Order("Negotiate") {}

Negotiate::Negotiate(const Negotiate& other) : Order(other) {}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    Order::operator=(other);
    return *this;
}

Negotiate::~Negotiate() {}

bool Negotiate::validate() {
    return true;
}

bool Negotiate::execute() {
    if (!validate()) {
        effect = "Negotiate order is invalid.";
        return false;
    }
    effect = "Negotiate order executed.";
    return true;
}

Order* Negotiate::clone() const {
    return new Negotiate(*this);
}

// =======================================================
// OrdersList Implementation
// =======================================================

OrdersList::OrdersList() {}

OrdersList::OrdersList(const OrdersList& other) {
    for (Order* o : other.orders) {
        orders.push_back(o->clone());
    }
}

OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {

        // clear existing
        for (Order* o : orders)
            delete o;
        orders.clear();

        // deep copy
        for (Order* o : other.orders)
            orders.push_back(o->clone());
    }
    return *this;
}

OrdersList::~OrdersList() {
    for (Order* o : orders)
        delete o;
}

void OrdersList::addOrder(Order* order) {
    orders.push_back(order);
}

void OrdersList::remove(int index) {
    if (index < 0 || index >= orders.size())
        throw std::out_of_range("Index out of range");

    delete orders[index];
    orders.erase(orders.begin() + index);
}

void OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex < 0 || fromIndex >= orders.size() ||
        toIndex < 0 || toIndex >= orders.size())
        throw std::out_of_range("Index out of range");

    Order* temp = orders[fromIndex];
    orders.erase(orders.begin() + fromIndex);
    orders.insert(orders.begin() + toIndex, temp);
}

Order* OrdersList::getOrder(int index) const {
    if (index < 0 || index >= orders.size())
        throw std::out_of_range("Index out of range");

    return orders[index];
}

int OrdersList::size() const {
    return orders.size();
}

void OrdersList::printOrders() const {
    for (Order* o : orders)
        std::cout << *o << std::endl;
}

std::ostream& operator<<(std::ostream& out, const OrdersList& list) {
    for (int i = 0; i < list.orders.size(); i++) {
        out << *(list.orders[i]) << std::endl;
    }
    return out;
}
