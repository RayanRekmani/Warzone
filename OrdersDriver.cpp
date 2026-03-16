#include <iostream>
#include <vector>
#include <iomanip>
#include "Orders.h"
#include "Cards.h"
#include "Player.h"
#include "Map.h"
#include "GameEngine.h"

int main() {
    std::cout << "=== Orders Driver Test ===\n\n";

    // Players
    Player* p1 = new Player("Player1");
    Player* p2 = new Player("Player2");

    // Map + Territories
    Continent* c1 = new Continent("C1", 0);
    Territory* t1 = new Territory(p1, 0, 0, c1, 10, "A"); // P1
    Territory* t2 = new Territory(p2, 1, 0, c1, 0, "B");  // P2, 0 armies -> guaranteed conquest
    Territory* t3 = new Territory(p1, 0, 1, c1, 4, "C");  // P1
    Territory* t4 = new Territory(p2, 1, 1, c1, 3, "D");  // P2

    // Add neighbors
    t1->addNeighbour(t2); t2->addNeighbour(t1);
    t1->addNeighbour(t4); t4->addNeighbour(t1);

    // Assign territories to players
    p1->addTerritory(t1); p1->addTerritory(t3);
    p2->addTerritory(t2); p2->addTerritory(t4);

    // Map
    Map* map = new Map();
    map->addContinent(c1);
    map->addTerritory(t1);
    map->addTerritory(t2);
    map->addTerritory(t3);
    map->addTerritory(t4);

    // Deck for card rewards
    Deck* deck = new Deck();
    deck->addCard(new BombCard());
    deck->addCard(new BlockadeCard());
    deck->addCard(new AirliftCard());
    deck->addCard(new DiplomacyCard());

    // ---------------- Orders for Player1 ----------------
    p1->issueOrder(new Deploy(p1, t1, 3));
    p1->issueOrder(new Advance(p1, t1, t2, 5)); // conquest -> ownership transfer + card

    DiplomacyCard dipCard;
    Order* negotiateOrder = dipCard.play();
    Negotiate* negotiate = dynamic_cast<Negotiate*>(negotiateOrder);
    if (negotiate) {
        negotiate->setIssuer(p1);
        negotiate->setTarget(p2);
        p1->issueOrder(negotiate);
    }

    p1->issueOrder(new Advance(p1, t1, t4, 1)); // invalid after negotiate

    AirliftCard airCard;
    Order* airOrder = airCard.play();
    Airlift* airlift = dynamic_cast<Airlift*>(airOrder);
    if (airlift) {
        airlift->setIssuer(p1);
        airlift->setSource(t3);
        airlift->setTarget(t1);
        airlift->setNumArmies(2);
        p1->issueOrder(airlift);
    }

    BombCard bombCard;
    Order* bombOrder = bombCard.play();
    Bomb* bomb = dynamic_cast<Bomb*>(bombOrder);
    if (bomb) {
        bomb->setIssuer(p1);
        bomb->setTarget(t4);
        p1->issueOrder(bomb);
    }

    BlockadeCard blockCard;
    Order* blockOrder = blockCard.play();
    Blockade* blockade = dynamic_cast<Blockade*>(blockOrder);
    if (blockade) {
        blockade->setIssuer(p1);
        blockade->setTarget(t3);
        p1->issueOrder(blockade);
    }

    // ---------------- Execute Orders ----------------
    GameEngine engine;
    engine.setPlayers({p1, p2});
    engine.setDeck(deck);
    engine.setMap(map);

    // Reinforcement Phase
    std::cout << "=== Reinforcement Phase ===\n";
    engine.reinforcementPhase();

    // Execute Orders Phase
    std::cout << "\n=== Execute Orders Phase ===\n";

    std::cout << "-- Step 1: Execute all Deploy orders --\n";
    engine.executeOrdersPhase(); // should execute orders round-robin

    // ---------------- Post-round Territory Status ----------------
    std::cout << "\nPost-round territory status:\n";
    std::cout << std::left 
              << std::setw(12) << "Territory"
              << std::setw(12) << "Owner"
              << std::setw(8)  << "Armies" << "\n";

    for (Territory* t : map->getTerritories()) {
        std::cout << std::left
                  << std::setw(12) << t->getTerritoryName()
                  << std::setw(12) << (t->getTerritoryOwner() ? t->getTerritoryOwner()->getName() : "None")
                  << std::setw(8)  << t->getArmySize()
                  << "\n";
    }

    std::cout << "\n=== End of Orders Driver Test ===\n";

    return 0;
}