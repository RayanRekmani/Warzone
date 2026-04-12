// developed by Lyna Taleb, 40210567
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"

/*
this file will basically create a GameEngiune obj, start the loop, allow user to enter commands, 
as well as observe state transitions as they happen
*/

int main() {
    GameEngine engine;

    Player* p1 = new Player("Lyna");
    Player* p2 = new Player("Rayan");

    Continent* c1 = new Continent("NorthAmerica", 5);

    Territory* t1 = new Territory("A", 0, 0, c1);
    Territory* t2 = new Territory("B", 1, 0, c1);
    Territory* t3 = new Territory("C", 2, 0, c1);

    t1->addNeighbour(t2);
    t2->addNeighbour(t1);
    t2->addNeighbour(t3);
    t3->addNeighbour(t2);

    t1->setTerritoryOwner(p1);
    t2->setTerritoryOwner(p1);
    t3->setTerritoryOwner(p2);

    t1->setArmySize(8);
    t2->setArmySize(5);
    t3->setArmySize(6);

    p1->addTerritory(t1);
    p1->addTerritory(t2);
    p2->addTerritory(t3);

    c1->addTerritory(t1);
    c1->addTerritory(t2);
    c1->addTerritory(t3);

    vector<Continent*> continents = { c1 };
    vector<Territory*> territories = { t1, t2, t3 };

    Map* map = new Map(continents, territories);

    Deck* deck = new Deck();
    deck->addCard(new BombCard());
    deck->addCard(new AirliftCard());
    deck->addCard(new BlockadeCard());
    deck->addCard(new DiplomacyCard());

    p1->getHand()->addCard(deck->draw());
    p2->getHand()->addCard(deck->draw());

    engine.setPlayers({ p1, p2 });
    engine.setMap(map);
    engine.setDeck(deck);

    engine.mainGameLoop(3);

    delete p1;
    delete p2;
    delete deck;
    delete map;

    return 0;
}
