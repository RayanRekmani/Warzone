// developed by Lyna Taleb, 40210567
#include "GameEngine.h"
#include <algorithm>

/*
control flow of the game w a state machine using a pointer to a state obj
when user enters valid command, pointer is updated to reference next state
(no implementation of game, just state transitions)
*/ 

// STATE CLASS --------------------------------------------------------------------

State::State(string name) {
    this->name = new string(name);
    transitions = new map<string, State*>();
} //allocate memoryfor state name and transitions map

State::State(const State& other) {
    name = new string(*other.name);
    transitions = new map<string, State*>(*other.transitions);
} //deep copy constructor (prevent 2 copies of tstate obj from sharing same memory)

State& State::operator=(const State& other) {
    if (this != &other) {
        delete name;
        delete transitions;
        name = new string(*other.name);
        transitions = new map<string, State*>(*other.transitions);
    }
    return *this;
} //assignment operator (same as copy constructor but also deallocate existing memory to prevent leaks)

State::~State() {
    delete name;
    delete transitions;
} //destructor

string State::getName() const {
    return *name;
} //returns name of current state 

void State::addTransition(string command, State* nextState) {
    (*transitions)[command] = nextState;
} //adds a transition from current state to nextState when user enters command

bool State::isCommandValid(string command) const {
    return transitions->find(command) != transitions->end();
} //checks if user command is valid in current state by looking for it in transitions map

State* State::getNextState(string command) const {
    return transitions->at(command);
} //returns pointer to next state ONLY IF COMMAND VALIDATED

ostream& operator<<(ostream& os, const State& state) {
    os << *state.name;
    return os;
} //we are required to allow the engine to print current srate during execution

// GAMEENGINE CLASS --------------------------------------------------------------------

GameEngine::GameEngine() {
    states = new vector<State*>();
    initializeStates();
    currentState = states->at(0); // start state
    commandProcessor = nullptr;
    deck = nullptr;
    maploader = nullptr;
    map = nullptr;
} //constructor initializes all states and transitions + sets current state to start state

GameEngine::GameEngine(const GameEngine& other) {
    states = new vector<State*>(*other.states);
    currentState = other.currentState;
    commandProcessor = other.commandProcessor;
    deck = other.deck;
    maploader = other.maploader;
    map = other.map;
    players = other.players;
} //copy constructor (GameEngine copying isnt useed in the gameplay this is just to satisfy the rule of 3)

GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        delete states;
        states = new vector<State*>(*other.states);
        currentState = other.currentState;
        commandProcessor = other.commandProcessor;
        deck = other.deck;
        maploader = other.maploader;
        map = other.map;
        players = other.players;
    }
    return *this;
} //same reason as copy constructor, also deallocate existing memory to prevent leaks

GameEngine::~GameEngine() {
    for (State* s : *states)
        delete s;
    delete states;
} //destructor deallocates memory for all state objects and the states vector itself

void GameEngine::initializeStates() { 
    State* start = new State("start");
    State* map_loaded = new State("map_loaded");
    State* map_validated = new State("map_validated");
    State* players_added = new State("players_added");
    State* assign_reinforcement = new State("assign_reinforcement");
    State* issue_orders = new State("issue_orders");
    State* execute_orders = new State("execute_orders");
    State* win = new State("win");
    State* end = new State("end");

    start->addTransition("loadmap", map_loaded);
    map_loaded->addTransition("validatemap", map_validated);
    map_validated->addTransition("addplayer", players_added);
    players_added->addTransition("assigncountries", assign_reinforcement);
    assign_reinforcement->addTransition("issueorder", issue_orders);
    issue_orders->addTransition("endissueorders", execute_orders);
    execute_orders->addTransition("endexecorders", assign_reinforcement);
    execute_orders->addTransition("win", win);
    win->addTransition("play", start);
    win->addTransition("end", end);

    states->push_back(start);
    states->push_back(map_loaded);
    states->push_back(map_validated);
    states->push_back(players_added);
    states->push_back(assign_reinforcement);
    states->push_back(issue_orders);
    states->push_back(execute_orders);
    states->push_back(win);
    states->push_back(end);
} //this fnction build the state machine exatly as shown in the state diagram in the assignment instructions

void GameEngine::processCommand(string command) {
    if (currentState->isCommandValid(command)) {
        currentState = currentState->getNextState(command);
        cout << "Transitioned to state: " << *currentState << endl;
        notify(this);
    } else { //if not valid, print error msg and stay in current state
        cout << "ERROR: Command not allowed in current state." << endl;
    }
} //validates user command by checking if it is a valid transition from current state, if valid updates current state to next state

string GameEngine::stringToLog() const {
    return "GameEngine transitioned to state: " + currentState->getName();
}

void GameEngine::start() {
    string command;
    while (currentState->getName() != "end") {
        cout << "\nCurrent state: " << *currentState << endl;
        cout << "> ";
        cin >> command;
        processCommand(command);
    }
} //this is the main game loop that will keep looping until user reaches the end state, it prompts user for commands and processes them

void GameEngine::setPlayers(const vector<Player*>& players) {
    this->players = players;
} //sets list of players for assignment 2 gameplay loop

void GameEngine::setDeck(Deck* deck) {
    this->deck = deck;
} //sets deck pointer used when players draw cards

void GameEngine::setMap(Map* map) {
    this->map = map;
} //sets map pointer used for continent bonus and winner detection

State* GameEngine::getCurrentState() const {
    return currentState;
} //returns current state pointer

bool GameEngine::ownsEntireContinent(Player* player, Continent* continent) const {
    if (player == nullptr || continent == nullptr || map == nullptr) {
        return false;
    }

    vector<Territory*> continentTerritories = map->getTerritoriesByContinent(continent);

    if (continentTerritories.empty()) {
        return false;
    }

    for (Territory* t : continentTerritories) {
        if (t == nullptr || t->getTerritoryOwner() != player) {
            return false;
        }
    }

    return true;
} //checks if every territory in continent belongs to given player

void GameEngine::removeEliminatedPlayers() {
    players.erase(
        remove_if(players.begin(), players.end(),
            [](Player* p) {
                return p == nullptr || p->getTerritories()->empty();
            }),
        players.end()
    );
} //removes all players that no longer own any territory

bool GameEngine::hasWinner() const {
    if (players.size() == 1) {
        return true;
    }

    if (map == nullptr) {
        return false;
    }

    vector<Territory*> allTerritories = map->getTerritories();

    if (allTerritories.empty()) {
        return false;
    }

    Player* owner = allTerritories[0]->getTerritoryOwner();
    if (owner == nullptr) {
        return false;
    }

    for (Territory* t : allTerritories) {
        if (t->getTerritoryOwner() != owner) {
            return false;
        }
    }

    return true;
} //checks if one player owns every territory on the map

Player* GameEngine::getWinner() const {
    if (!hasWinner()) {
        return nullptr;
    }

    if (players.size() == 1) {
        return players[0];
    }

    vector<Territory*> allTerritories = map->getTerritories();
    if (allTerritories.empty()) {
        return nullptr;
    }

    return allTerritories[0]->getTerritoryOwner();
} //returns winner pointer if game has ended

void GameEngine::reinforcementPhase() {
    if (map == nullptr) {
        cout << "ERROR: No map loaded." << endl;
        return;
    }

    cout << "\n=== Reinforcement Phase ===" << endl;

    for (Player* p : players) {
        if (p == nullptr) {
            continue;
        }

        int territoryCount = static_cast<int>(p->getTerritories()->size());
        int reinforcementArmies = territoryCount / 3;

        if (reinforcementArmies < 3) {
            reinforcementArmies = 3;
        }

        vector<Continent*> continents = map->getContinents();
        for (Continent* c : continents) {
            if (ownsEntireContinent(p, c)) {
                reinforcementArmies += c->getExtraSoldiers();
            }
        }

        p->setReinforcementPool(p->getReinforcementPool() + reinforcementArmies);

        cout << p->getName() << " receives "
             << reinforcementArmies
             << " reinforcement armies." << endl;
    }
} //assigns reinforcement armies based on territory count + continent bonus with minimum of 3

void GameEngine::issueOrdersPhase() {
    cout << "\n=== Issue Orders Phase ===" << endl;

    for (Player* p : players) {
        if (p != nullptr) {
            p->setConqueredTerritoryThisTurn(false);
            p->clearNegotiatedPlayers();
        }
    }

    bool moreOrders = true;

    while (moreOrders) {
        moreOrders = false;

        for (Player* p : players) {
            if (p == nullptr) {
                continue;
            }

            bool issued = p->issueOrder(deck, players);

            if (issued) {
                moreOrders = true;
                cout << p->getName() << " issued an order." << endl;
            }
        }
    }
} //asks each player in round robin order to issue one order at a time until all players are done

void GameEngine::executeOrdersPhase() {
    cout << "\n=== Execute Orders Phase ===" << endl;

    for (Player* p : players) {
        if (p == nullptr) {
            continue;
        }

        OrdersList* orders = p->getOrdersList();
        int i = 0;

        while (i < orders->size()) {
            Order* currentOrder = orders->getOrder(i);

            if (currentOrder != nullptr && currentOrder->getOrderType() == "Deploy") {
                cout << p->getName() << " executes: " << *currentOrder << endl;
                currentOrder->execute();
                orders->remove(i);
            } else {
                i++;
            }
        }
    }

    bool ordersLeft = true;

    while (ordersLeft) {
        ordersLeft = false;

        for (Player* p : players) {
            if (p == nullptr) {
                continue;
            }

            OrdersList* orders = p->getOrdersList();

            if (orders->size() > 0) {
                ordersLeft = true;

                Order* currentOrder = orders->getOrder(0);
                if (currentOrder != nullptr) {
                    cout << p->getName() << " executes: " << *currentOrder << endl;
                    currentOrder->execute();
                }
                orders->remove(0);
            }
        }
    }

    if (deck != nullptr) {
        for (Player* p : players) {
            if (p != nullptr && p->hasConqueredTerritoryThisTurn()) {
                deck->drawToHand(p->getHand());
                cout << p->getName() << " conquered a territory and drew a card." << endl;
            }
        }
    }

    removeEliminatedPlayers();
} //executes all deploy orders first then executes remaining orders one by one in round robin order

void GameEngine::mainGameLoop() {
    cout << "\n=== Main Game Loop Started ===" << endl;

    int turnCount = 0;
    int maxTurns = 3;

    while (!hasWinner() && turnCount < maxTurns) {
        turnCount++;
        cout << "\n--- Turn " << turnCount << " ---" << endl;

        removeEliminatedPlayers();

        if (players.size() <= 1) {
            break;
        }

        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
    }

    Player* winner = getWinner();
    if (winner != nullptr) {
        cout << "\nWinner: " << winner->getName() << endl;
    } else {
        cout << "\nGame ended after " << maxTurns << " turns without a winner." << endl;
    }
} //controls the game until one player owns all territories or all other players are eliminated