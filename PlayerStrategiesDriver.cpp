#include <iostream>
#include <vector>

#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include "PlayerStrategies.h"

using namespace std;

static void connectBidirectional(Territory* a, Territory* b) {
    if (a == nullptr || b == nullptr) {
        return;
    }
    a->addNeighbour(b);
    b->addNeighbour(a);
}

static void printTerritoryNames(const vector<Territory*>& territories) {
    if (territories.empty()) {
        cout << "(none)";
    }
    for (Territory* t : territories) {
        if (t != nullptr) {
            cout << t->getTerritoryName() << " ";
        }
    }
    cout << endl;
}

static void printOwnership(const vector<Territory*>& territories) {
    for (Territory* t : territories) {
        if (t == nullptr) {
            continue;
        }
        Player* owner = t->getTerritoryOwner();
        cout << t->getTerritoryName() << " -> "
             << (owner != nullptr ? owner->getName() : string("(none)"))
             << " (armies: " << t->getArmySize() << ")" << endl;
    }
    cout << endl;
}

static void demoPlayerMethods(Player* p) {
    cout << p->getName() << " strategy: " << p->getPlayerStrategies()->getStrategyType() << endl;

    vector<Territory*>* defend = p->toDefend();
    vector<Territory*>* attack = p->toAttack();

    cout << "  toDefend: ";
    printTerritoryNames(*defend);
    cout << "  toAttack: ";
    printTerritoryNames(*attack);

    delete defend;
    delete attack;
}

int main() {
    cout << "===== PlayerStrategies Driver =====\n\n";

    // shared map for benevolent / neutral / cheater scenarios
    Continent* continent = new Continent("DemoContinent", 5);

    Territory* bWeak = new Territory("B-Weak", 0, 0, continent);
    Territory* bStrong = new Territory("B-Strong", 1, 0, continent);
    Territory* nZone = new Territory("Neutral-Zone", 2, 0, continent);
    Territory* cHome = new Territory("Cheater-Home", 3, 0, continent);
    Territory* cFar = new Territory("Cheater-Far", 4, 0, continent);

    connectBidirectional(bWeak, bStrong);
    connectBidirectional(bStrong, nZone);
    connectBidirectional(nZone, cHome);
    connectBidirectional(cHome, cFar);

    vector<Territory*> allTerritories = {bWeak, bStrong, nZone, cHome, cFar};

    Player* benevolent = new Player("BenevolentBot");
    Player* neutral = new Player("NeutralBot");
    Player* cheater = new Player("CheaterBot");

    benevolent->setPlayerStrategies(new BenevolentPlayerStrategies(benevolent));
    neutral->setPlayerStrategies(new NeutralPlayerStrategies(neutral));
    cheater->setPlayerStrategies(new CheaterPlayerStrategies(cheater));

    bWeak->setTerritoryOwner(benevolent);
    bStrong->setTerritoryOwner(benevolent);
    benevolent->addTerritory(bWeak);
    benevolent->addTerritory(bStrong);
    bWeak->setArmySize(1);
    bStrong->setArmySize(8);

    nZone->setTerritoryOwner(neutral);
    neutral->addTerritory(nZone);
    nZone->setArmySize(4);

    cHome->setTerritoryOwner(cheater);
    cFar->setTerritoryOwner(cheater);
    cheater->addTerritory(cHome);
    cheater->addTerritory(cFar);
    cHome->setArmySize(5);
    cFar->setArmySize(2);

    cout << "Initial ownership:\n";
    printOwnership(allTerritories);

    cout << "[Scenario 1] Method behavior by strategy (toDefend/toAttack)\n";
    demoPlayerMethods(benevolent);
    demoPlayerMethods(neutral);
    demoPlayerMethods(cheater);
    cout << endl;

    cout << "[Scenario 2] Benevolent issueOrder behavior\n";
    benevolent->setReinforcementPool(5);
    bool beneDeployIssued = benevolent->issueOrder(true);
    bool beneAdvanceIssued = benevolent->issueOrder(false);
    cout << "  deploy issued: " << (beneDeployIssued ? "yes" : "no") << endl;
    cout << "  advance issued: " << (beneAdvanceIssued ? "yes" : "no") << endl;
    cout << "  orders in list: " << benevolent->getOrdersList()->size() << endl;
    cout << endl;

    cout << "[Scenario 3] Neutral issueOrder behavior (should never issue)\n";
    bool neutralDeployIssued = neutral->issueOrder(true);
    bool neutralAdvanceIssued = neutral->issueOrder(false);
    cout << "  deploy issued: " << (neutralDeployIssued ? "yes" : "no") << endl;
    cout << "  advance issued: " << (neutralAdvanceIssued ? "yes" : "no") << endl;
    cout << endl;

    cout << "[Scenario 4] Cheater issueOrder behavior (auto-conquer once/turn)\n";
    cout << "  ownership before cheater turn:\n";
    printOwnership(allTerritories);

    cheater->issueOrder(true); // reset once-per-turn flag
    bool cheaterFirstAdvance = cheater->issueOrder(false);
    bool cheaterSecondAdvance = cheater->issueOrder(false);

    cout << "  first advance call conquered? " << (cheaterFirstAdvance ? "yes" : "no") << endl;
    cout << "  second advance same turn conquered? " << (cheaterSecondAdvance ? "yes" : "no") << endl;
    cout << "  ownership after cheater turn:\n";
    printOwnership(allTerritories);

    // new modification: aggressive strategy demo for person 2
    cout << "[Scenario 5] Aggressive strategy behavior\n";

    Player* aggressive = new Player("AggressiveBot");
    Player* enemy = new Player("EnemyBot");

    aggressive->setPlayerStrategies(new AggressivePlayerStrategies(aggressive));
    enemy->setPlayerStrategies(new NeutralPlayerStrategies(enemy));

    Territory* aStrong = new Territory("Aggro-Strong", 20, 0, continent);
    Territory* aWeak = new Territory("Aggro-Weak", 21, 0, continent);
    Territory* aEnemy = new Territory("Aggro-Target", 22, 0, continent);

    connectBidirectional(aStrong, aWeak);
    connectBidirectional(aStrong, aEnemy);

    aStrong->setTerritoryOwner(aggressive);
    aWeak->setTerritoryOwner(aggressive);
    aEnemy->setTerritoryOwner(enemy);

    aggressive->addTerritory(aStrong);
    aggressive->addTerritory(aWeak);
    enemy->addTerritory(aEnemy);

    aStrong->setArmySize(10);
    aWeak->setArmySize(2);
    aEnemy->setArmySize(4);

    demoPlayerMethods(aggressive);

    aggressive->setReinforcementPool(6);
    bool aggressiveDeployIssued = aggressive->issueOrder(true);
    bool aggressiveAdvanceIssued = aggressive->issueOrder(false);

    cout << "  deploy issued: " << (aggressiveDeployIssued ? "yes" : "no") << endl;
    cout << "  advance issued: " << (aggressiveAdvanceIssued ? "yes" : "no") << endl;
    cout << "  orders in list: " << aggressive->getOrdersList()->size() << endl;
    cout << endl;

    // new modification: human strategy demo for person 2
    cout << "[Scenario 6] Human strategy behavior (requires user input)\n";

    Player* human = new Player("HumanPlayer");
    human->setPlayerStrategies(new HumanPlayerStrategies(human));

    Territory* h1 = new Territory("Human-Home", 30, 0, continent);
    Territory* h2 = new Territory("Human-Front", 31, 0, continent);
    Territory* hEnemy = new Territory("Human-Enemy", 32, 0, continent);

    connectBidirectional(h1, h2);
    connectBidirectional(h2, hEnemy);

    h1->setTerritoryOwner(human);
    h2->setTerritoryOwner(human);
    hEnemy->setTerritoryOwner(enemy);

    human->addTerritory(h1);
    human->addTerritory(h2);
    enemy->addTerritory(hEnemy);

    h1->setArmySize(7);
    h2->setArmySize(3);
    hEnemy->setArmySize(4);

    demoPlayerMethods(human);

    human->setReinforcementPool(5);

    cout << "  now the human player will choose a deploy order.\n";
    bool humanDeployIssued = human->issueOrder(true);
    cout << "  deploy issued: " << (humanDeployIssued ? "yes" : "no") << endl;

    cout << "  now the human player will decide whether to issue an advance order.\n";
    bool humanAdvanceIssued = human->issueOrder(false);
    cout << "  advance issued: " << (humanAdvanceIssued ? "yes" : "no") << endl;
    cout << "  human orders in list: " << human->getOrdersList()->size() << endl;
    cout << endl;

    // new modification: keep dynamic strategy switch demo because rubric requires it
    cout << "[Scenario 7] Dynamic strategy switch: Neutral -> Aggressive when attacked\n";
    Player* neutralSwitch = new Player("NeutralSwitch");
    Player* attacker = new Player("Attacker");
    neutralSwitch->setPlayerStrategies(new NeutralPlayerStrategies(neutralSwitch));
    attacker->setPlayerStrategies(new AggressivePlayerStrategies(attacker));

    Territory* x = new Territory("X", 10, 0, continent);
    Territory* y = new Territory("Y", 11, 0, continent);
    connectBidirectional(x, y);

    x->setTerritoryOwner(attacker);
    attacker->addTerritory(x);
    x->setArmySize(5);

    y->setTerritoryOwner(neutralSwitch);
    neutralSwitch->addTerritory(y);
    y->setArmySize(3);

    cout << "  before attack strategy: " << neutralSwitch->getPlayerStrategies()->getStrategyType() << endl;
    Advance attackOrder(attacker, x, y, 1);
    attackOrder.execute();
    cout << "  after attack strategy: " << neutralSwitch->getPlayerStrategies()->getStrategyType() << endl;

    delete x;
    delete y;
    delete neutralSwitch;
    delete attacker;

    delete aggressive;
    delete enemy;
    delete human;

    delete aStrong;
    delete aWeak;
    delete aEnemy;
    delete h1;
    delete h2;
    delete hEnemy;

    delete benevolent;
    delete neutral;
    delete cheater;

    delete bWeak;
    delete bStrong;
    delete nZone;
    delete cHome;
    delete cFar;
    delete continent;

    cout << "\n===== End of PlayerStrategies Driver =====\n";
    return 0;
}