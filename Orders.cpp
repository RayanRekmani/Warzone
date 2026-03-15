#include "Orders.h"
#include <stdexcept>
#include <algorithm>
#include <random>

#include "Map.h"
#include "Player.h"

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
//default constructor
Deploy::Deploy()
    : Order("Deploy")
    , issuer(nullptr)
    , target(nullptr)
    , numArmies(0) {}

//parameterized constructor
Deploy::Deploy(Player* issuer, Territory* target, int numArmies)
    : Order("Deploy")
    , issuer(issuer)
    , target(target)
    , numArmies(numArmies) {}

//copy constructor
Deploy::Deploy(const Deploy& other)
    : Order(other)
    , issuer(other.issuer)
    , target(other.target)
    , numArmies(other.numArmies) {}

//assignment operator
Deploy& Deploy::operator=(const Deploy& other) {
    Order::operator=(other);
    if (this != &other) {
        issuer = other.issuer;
        target = other.target;
        numArmies = other.numArmies;
    }
    return *this;
}

Deploy::~Deploy() {}

bool Deploy::validate() {
    if (issuer == nullptr || target == nullptr) {
        return false;
    }
    if (numArmies <= 0) {
        return false;
    }
    if (target->getTerritoryOwner() != issuer) {
        return false;
    }
    if (issuer->getReinforcementPool() < numArmies) {
        return false;
    }
    return true;
}

bool Deploy::execute() {
    if (!validate()) {
        effect = "Deploy order is invalid.";
        return false;
    }

    const int newPool = issuer->getReinforcementPool() - numArmies;
    issuer->setReinforcementPool(newPool);

    const int newArmySize = target->getArmySize() + numArmies;
    target->setArmySize(newArmySize);

    effect = "Deploy order executed." + std::to_string(numArmies) + 
            " armies to " + target->getTerritoryName();
    return true;
}

Order* Deploy::clone() const {
    return new Deploy(*this);
}

// =======================================================
// Advance
// =======================================================

Advance::Advance()
    : Order("Advance")
    , issuer(nullptr)
    , source(nullptr)
    , target(nullptr)
    , numArmies(0) {}

//Stores all the data required to execute an advance order
Advance::Advance(Player* issuer, Territory* source, Territory* target, int numArmies)
    : Order("Advance")
    , issuer(issuer)
    , source(source)
    , target(target)
    , numArmies(numArmies) {}

Advance::Advance(const Advance& other)
    : Order(other)
    , issuer(other.issuer)
    , source(other.source)
    , target(other.target)
    , numArmies(other.numArmies) {}

Advance& Advance::operator=(const Advance& other) {
    Order::operator=(other);
    if (this != &other) {
        issuer = other.issuer;
        source = other.source;
        target = other.target;
        numArmies = other.numArmies;
    }
    return *this;
}

Advance::~Advance() {}

bool Advance::validate() {
    if (issuer == nullptr || source == nullptr || target == nullptr) {
        return false;
    }
    if (numArmies <= 0) {
        return false;
    }
    if (source->getTerritoryOwner() != issuer) {
        return false;
    }
    if (!source->isNeighbour(target)) {
        return false;
    }
    Player* targetOwner = target->getTerritoryOwner();
    if (targetOwner != nullptr && targetOwner != issuer) {
        if (issuer->isNegotiatingWith(targetOwner) || targetOwner->isNegotiatingWith(issuer)) {
            return false;
        }
    }
    if (source->getArmySize() < numArmies) {
        return false;
    }
    return true;
}

bool Advance::execute() {
    // Step 1: Validate the order
    if (!validate()) {
        effect = "Advance order is invalid.";
        return false;
    }

    // Step 2: Moving armies within player's own territories
    if (target->getTerritoryOwner() == issuer) {
        source->setArmySize(source->getArmySize() - numArmies);
        target->setArmySize(target->getArmySize() + numArmies);
        effect = "Advance order executed: armies moved between owned territories.";
        return true;
    }

    // Step 3: Attacking an enemy territory
    // Remove attacking armies from source territory
    source->setArmySize(source->getArmySize() - numArmies);

    int attacking = numArmies;           // number of armies attacking
    int defending = target->getArmySize(); // number of defending armies

    // Random generator for battle simulation
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Step 3a: Calculate defender casualties
    int defendersKilled = 0;
    for (int i = 0; i < attacking; ++i) {
        if (dist(rng) <= 0.60) { // 60% chance per attacking unit
            ++defendersKilled;
        }
    }

    // Step 3b: Calculate attacker casualties
    int attackersKilled = 0;
    for (int i = 0; i < defending; ++i) {
        if (dist(rng) <= 0.70) { // 70% chance per defending unit
            ++attackersKilled;
        }
    }

    // Make sure we don't subtract more than available
    defendersKilled = std::min(defendersKilled, defending);
    attackersKilled = std::min(attackersKilled, attacking);

    // Update remaining armies
    attacking -= attackersKilled;
    defending -= defendersKilled;

    // Step 4: Determine outcome
    if (defending == 0) {
        // Attacker conquered the territory
        Player* previousOwner = target->getTerritoryOwner();
        if (previousOwner != nullptr && previousOwner != issuer) {
            previousOwner->removeTerritory(target); // remove from defender's list
        }

        target->setTerritoryOwner(issuer);   // transfer ownership
        if (issuer != nullptr) {
            issuer->addTerritory(target);    // add to attacker's list
        }

        target->setArmySize(attacking);      // place surviving attackers
        issuer->setConqueredTerritoryThisTurn(true); // mark for card reward

        effect = "Advance order executed: territory conquered.";
    } else {
        // Attack failed, defenders remain
        target->setArmySize(defending);
        // Surviving attackers are lost (do NOT return to source)
        effect = "Advance order executed: attack failed.";
    }

    return true;
}

Order* Advance::clone() const {
    return new Advance(*this);
}

// =======================================================
// Bomb
// =======================================================

Bomb::Bomb()
    : Order("Bomb")
    , issuer(nullptr)
    , target(nullptr) {}

Bomb::Bomb(Player* issuer, Territory* target)
    : Order("Bomb")
    , issuer(issuer)
    , target(target) {}

Bomb::Bomb(const Bomb& other)
    : Order(other)
    , issuer(other.issuer)
    , target(other.target) {}

Bomb& Bomb::operator=(const Bomb& other) {
    Order::operator=(other);
    if (this != &other) {
        issuer = other.issuer;
        target = other.target;
    }
    return *this;
}

Bomb::~Bomb() {}

bool Bomb::validate() {
    if (issuer == nullptr || target == nullptr) {
        return false;
    }
    if (target->getTerritoryOwner() == issuer) {
        return false;
    }
    std::vector<Territory*>* territories = issuer->getTerritories();
    if (territories == nullptr) {
        return false;
    }
    for (Territory* t : *territories) {
        if (t != nullptr && t->isNeighbour(target)) {
            return true;
        }
    }
    return false;
}

bool Bomb::execute() {
    if (!validate()) {
        effect = "Bomb order is invalid.";
        return false;
    }
    int current = target->getArmySize();
    int reduced = current / 2; // remove half the armies (round down)
    target->setArmySize(reduced);
    effect = "Bomb order executed.";
    return true;
}

void Bomb::setIssuer(Player* p) {
    issuer = p;
}

void Bomb::setTarget(Territory* t) {
    target = t;
}

Order* Bomb::clone() const {
    return new Bomb(*this);
}

// =======================================================
// Blockade
// =======================================================

namespace {
Player* getNeutralPlayer() {
    static Player* neutral = nullptr;
    if (neutral == nullptr) {
        neutral = new Player("Neutral");
    }
    return neutral;
}
}

Blockade::Blockade()
    : Order("Blockade")
    , issuer(nullptr)
    , target(nullptr) {}

Blockade::Blockade(Player* issuer, Territory* target)
    : Order("Blockade")
    , issuer(issuer)
    , target(target) {}

Blockade::Blockade(const Blockade& other)
    : Order(other)
    , issuer(other.issuer)
    , target(other.target) {}

Blockade& Blockade::operator=(const Blockade& other) {
    Order::operator=(other);
    if (this != &other) {
        issuer = other.issuer;
        target = other.target;
    }
    return *this;
}

Blockade::~Blockade() {}

bool Blockade::validate() {
    if (issuer == nullptr || target == nullptr) {
        return false;
    }
    if (target->getTerritoryOwner() != issuer) {
        return false;
    }
    return true;
}

bool Blockade::execute() {
    if (!validate()) {
        effect = "Blockade order is invalid.";
        return false;
    }
    target->setArmySize(target->getArmySize() * 2);  // Double the number of armies on the target territory

    // Get/create the Neutral player to take ownership of the territory
    Player* neutral = getNeutralPlayer();
    if (issuer != nullptr) {                          // Remove the territory from the issuing player's list
        issuer->removeTerritory(target);
    }
    target->setTerritoryOwner(neutral);             // Transfer ownership to the Neutral player
    if (neutral != nullptr) {
        neutral->addTerritory(target);
    }

    effect = "Blockade order executed.";
    return true;
}

void Blockade::setIssuer(Player* p) {
    issuer = p;
}

void Blockade::setTarget(Territory* t) {
    target = t;
}

Order* Blockade::clone() const {
    return new Blockade(*this);
}

// =======================================================
// Airlift
// =======================================================

Airlift::Airlift()
    : Order("Airlift")
    , issuer(nullptr)
    , source(nullptr)
    , target(nullptr)
    , numArmies(0) {}

Airlift::Airlift(Player* issuer, Territory* source, Territory* target, int numArmies)
    : Order("Airlift")
    , issuer(issuer)
    , source(source)
    , target(target)
    , numArmies(numArmies) {}

Airlift::Airlift(const Airlift& other)
    : Order(other)
    , issuer(other.issuer)
    , source(other.source)
    , target(other.target)
    , numArmies(other.numArmies) {}

Airlift& Airlift::operator=(const Airlift& other) {
    Order::operator=(other);
    if (this != &other) {
        issuer = other.issuer;
        source = other.source;
        target = other.target;
        numArmies = other.numArmies;
    }
    return *this;
}

Airlift::~Airlift() {}

bool Airlift::validate() {
    if (issuer == nullptr || source == nullptr || target == nullptr) {
        return false;
    }
    if (numArmies <= 0) {
        return false;
    }
    if (source->getTerritoryOwner() != issuer) {
        return false;
    }
    if (target->getTerritoryOwner() != issuer) {
        return false;
    }
    if (source->getArmySize() < numArmies) {
        return false;
    }
    return true;
}

bool Airlift::execute() {
    // First, check if the order is valid (ownership, armies, territories exist, etc.)
    if (!validate()) {
        effect = "Airlift order is invalid."; // if invalid, set effect message
        return false;                         // stop execution
    }

    // Remove the armies from the source territory
    source->setArmySize(source->getArmySize() - numArmies);

    // Add the armies to the target territory
    target->setArmySize(target->getArmySize() + numArmies);

    // Set the effect message to indicate successful execution
    effect = "Airlift order executed.";

    return true; // indicate that execution succeeded
}

void Airlift::setIssuer(Player* p) {
    issuer = p;
}

void Airlift::setSource(Territory* t) {
    source = t;
}

void Airlift::setTarget(Territory* t) {
    target = t;
}

void Airlift::setNumArmies(int n) {
    numArmies = n;
}

Order* Airlift::clone() const {
    return new Airlift(*this);
}

// =======================================================
// Negotiate
// =======================================================

Negotiate::Negotiate()
    : Order("Negotiate")
    , issuer(nullptr)
    , target(nullptr) {}

Negotiate::Negotiate(Player* issuer, Player* target)
    : Order("Negotiate")
    , issuer(issuer)
    , target(target) {}

Negotiate::Negotiate(const Negotiate& other)
    : Order(other)
    , issuer(other.issuer)
    , target(other.target) {}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    Order::operator=(other);
    if (this != &other) {
        issuer = other.issuer;
        target = other.target;
    }
    return *this;
}

Negotiate::~Negotiate() {}

bool Negotiate::validate() {
    if (issuer == nullptr || target == nullptr) {
        return false;
    }
    if (issuer == target) {     // Check that the target is not the same as the issuer
        return false;
    }
    return true;
}

bool Negotiate::execute() {
    if (!validate()) {
        effect = "Negotiate order is invalid.";
        return false;
    }

    // Establish a temporary pact between the issuer and target for the current turn
    // Add each player to the other's negotiatedPlayers list (stored in the Player class).
    // This records a temporary truce so any attack between these two players during
    // the current turn will be considered invalid.
    issuer->addNegotiatedPlayer(target);    // Issuer will not attack target this turn
    target->addNegotiatedPlayer(issuer);    // Target will not attack issuer this turn
    effect = "Negotiate order executed.";
    return true;
}

void Negotiate::setIssuer(Player* p) {
    issuer = p;
}

void Negotiate::setTarget(Player* p) {
    target = p;
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

void OrdersList::move(int i, int j) {
    if (i < 0 || i >= orders.size() ||
        j < 0 || j >= orders.size())
        throw std::out_of_range("Index out of range");

    Order* temp = orders[i];
    orders.erase(orders.begin() + i);
    orders.insert(orders.begin() + j, temp);
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
    for (size_t i = 0; i < list.orders.size(); i++) {
        out << *(list.orders[i]) << std::endl;
    }
    return out;
}
