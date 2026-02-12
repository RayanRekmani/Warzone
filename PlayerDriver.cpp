// PlayerDriver.cpp

#include "Player.h"

#include <iostream>
#include <vector>

// teammate headers
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

int main() {
    std::cout << " PlayerDriver Demo \n";

    // Create a continent 
    Continent* c1 = new Continent("C1", 5);

    Territory* t1 = new Territory("T1", 0, 0, c1);
    Territory* t2 = new Territory("T2", 1, 1, c1);

    // Create a Player
    Player p("Alice");


    p.addTerritory(t1);
    p.addTerritory(t2);




    std::cout << p << "\n";
    std::cout << "Player has a hand of cards\n";
    std::cout << "Player has an orders list\n\n";

   
    std::vector<Territory*>* defend = p.toDefend();
    std::vector<Territory*>* attack = p.toAttack();

    std::cout << "toDefend() returned " << defend->size() << " territory(ies):\n";
    for (Territory* t : *defend) {
        std::cout << "  defend: " << *t << "\n";
    }

    std::cout << "\n";

    std::cout << "toAttack() returned " << attack->size() << " territory(ies):\n";
    for (Territory* t : *attack) {
        std::cout << "  attack: " << *t << "\n";
    }

    delete defend;
    delete attack;

    std::cout << "\n";


    Order* o = new Deploy();
    p.issueOrder(o);

    std::cout << "Issued one order using issueOrder().\n";
    std::cout << "OrdersList size is now: " << p.getOrdersList()->size() << "\n";

    std::cout << "End \n";
    delete t1;
    delete t2;
    delete c1;

    return 0;
}
