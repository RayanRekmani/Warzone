#include <iostream>
#include <vector>
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
    Territory* t1 = new Territory(p1, 0, 0, c1, 10, "A"); // owned by P1
    Territory* t2 = new Territory(p2, 1, 0, c1, 0, "B");  // owned by P2, 0 armies -> guaranteed conquest
    Territory* t3 = new Territory(p1, 0, 1, c1, 4, "C");  // owned by P1
    Territory* t4 = new Territory(p2, 1, 1, c1, 3, "D");  // owned by P2

    t1->addNeighbour(t2);
    t2->addNeighbour(t1);
    t1->addNeighbour(t4);
    t4->addNeighbour(t1);

    p1->addTerritory(t1);
    p1->addTerritory(t3);
    p2->addTerritory(t2);
    p2->addTerritory(t4);

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

    // Orders issued by Player1
    p1->issueOrder(new Deploy(p1, t1, 3));
    p1->issueOrder(new Advance(p1, t1, t2, 5)); // conquest -> ownership transfer + card

    DiplomacyCard dipCard;
    Order* negotiateOrder = dipCard.play();
    Negotiate* negotiate = dynamic_cast<Negotiate*>(negotiateOrder);
    if (negotiate != nullptr) {
        negotiate->setIssuer(p1);
        negotiate->setTarget(p2);
        p1->issueOrder(negotiate);
    }

    p1->issueOrder(new Advance(p1, t1, t4, 1)); // should be invalid after negotiate

    AirliftCard airCard;
    Order* airOrder = airCard.play();
    Airlift* airlift = dynamic_cast<Airlift*>(airOrder);
    if (airlift != nullptr) {
        airlift->setIssuer(p1);
        airlift->setSource(t3);
        airlift->setTarget(t1);
        airlift->setNumArmies(2);
        p1->issueOrder(airlift);
    }

    BombCard bombCard;
    Order* bombOrder = bombCard.play();
    Bomb* bomb = dynamic_cast<Bomb*>(bombOrder);
    if (bomb != nullptr) {
        bomb->setIssuer(p1);
        bomb->setTarget(t4); // adjacent to t1
        p1->issueOrder(bomb);
    }

    BlockadeCard blockCard;
    Order* blockOrder = blockCard.play();
    Blockade* blockade = dynamic_cast<Blockade*>(blockOrder);
    if (blockade != nullptr) {
        blockade->setIssuer(p1);
        blockade->setTarget(t3);
        p1->issueOrder(blockade);
    }

    // Game engine executes orders (one round per player)
    GameEngine engine;
    engine.setPlayers(std::vector<Player*>{p1, p2});
    engine.setDeck(deck);
    engine.setMap(map);
    engine.start();

    // Demonstrate ownership transfer after conquest and blockade
    std::cout << "\nPost-round ownership checks:\n";
    std::cout << "Territory B owner: " << t2->getTerritoryOwner()->getName() << "\n";
    std::cout << "Territory C owner: " << t3->getTerritoryOwner()->getName() << "\n";

    std::cout << "\n=== End of Orders Driver Test ===\n";
    return 0;
}
