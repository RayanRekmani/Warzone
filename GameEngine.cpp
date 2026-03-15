// developed by Lyna Taleb, 40210567
// appended by Rayan Rekmani 40283058
// appended by Adam Blevins 40255384
#include "GameEngine.h"
#include "CommandProcessing.h"
#include <random>
#include "Player.h"
#include <iostream>
#include "Cards.h"
#include "LoggingObserver.h"
#include "Orders.h"
#include "Map.h"
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
} //constructor initializes all states and transitions + sets current state to start state

GameEngine::GameEngine(const GameEngine& other) {
    states = new vector<State*>(*other.states);
    currentState = other.currentState;
} //copy constructor (GameEngine copying isnt useed in the gameplay this is just to satisfy the rule of 3)

GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        delete states;
        states = new vector<State*>(*other.states);
        currentState = other.currentState;
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
    players_added->addTransition("addplayer", players_added);
    players_added->addTransition("gamestart", assign_reinforcement);
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
    } else { //if not valid, print error msg and stay in current state
        cout << "ERROR: Command not allowed in current state." << endl;
    }
} //validates user command by checking if it is a valid transition from current state, if valid updates current state to next state

State* GameEngine::getCurrentState() const {
    return currentState;
}


void GameEngine::setPlayers(const vector<Player*>& p) {
    players = p;
}

void GameEngine::setDeck(Deck* d) {
    deck = d;
}

void GameEngine::setMap(Map* m) {
    map = m;
}

void GameEngine::startupPhase(){
    cout << "Enter commands (type quit to stop)\n";
    Command* c;
    commandProcessor = new CommandProcessor(this);
    deck = new Deck();
    maploader = new MapLoader();

    //while in startup phase, keep accepting commands, then process them.
    while(getCurrentState()->getName() != "assign_reinforcement"){
            cout << "Current State: " << getCurrentState()->getName() << endl;
            cout << "> ";
            c = commandProcessor->getCommand();

            if(c->getCommand() == "quit"){ // If the user entered quit, break before doing anything else
                break;
            }

            vector<string> arguments; // Stores the arguments of a command. There should only ever be two arguments max in a command maximum.

            if(c->getCommand().find(" ") < c->getCommand().length()){ // If there's a space in the command, split it up the command.
                arguments.push_back(c->getCommand().substr(0, c->getCommand().find(" "))); // Puts the first argument in the arguments vector.
                arguments.push_back(c->getCommand().substr(c->getCommand().find(" ") + 1, c->getCommand().length())); // Puts everything else in the second argument.
            }
            else{
                arguments.push_back(c->getCommand());
                arguments.push_back(""); // Some times arguments[1] will be called, this line prevents the program from crashing if there is only one argument in the command.
            }

            if(commandProcessor->validate(c)){
                // The start of a big else if chain to do an action based off the command.
                if(arguments[0] == "loadmap"){
                    string mapName = arguments[1];

                    if(mapName.substr(0, 5) != "maps/") { // If user forgot to enter maps/ in the file name, add it for them.
                        mapName = "maps/" + mapName;
                    }
                    if(mapName.substr(mapName.length() - 4, mapName.length()) != ".map") { // If user forgot to enter the .map suffix in the file name, add it for them.
                        mapName += ".map";
                    }

                    map = maploader->loadMap(mapName);
                    if(map){ // If the map loaded successfully, transition to validatemap state.
                        processCommand(arguments[0]);
                    }

                }

                else if(arguments[0] == "validatemap"){
                    bool isValid = map->validateMap();
                    if(isValid){ // If the map is valid, transition to addplayer state.
                        processCommand(arguments[0]);
                    }
                }

                else if(arguments[0] == "addplayer"){
                    if(players.size() < 6){
                        if(arguments[1] == ""){
                            arguments[1] = "anonymous";
                        }
                        players.push_back(new Player(arguments[1]));
                        processCommand(arguments[0]);
                    }
                    else{
                        cout << "ERROR: The maximum number of players (6) has already been reached." << endl;
                    }
                }

                else if(arguments[0] == "gamestart"){
                    if(players.size() >= 2){
                        // Randomize turn order
                        auto rng = default_random_engine {};
                        shuffle(players.begin(), players.end(), rng); // Randomize order of player vector, randomizing the paly order.
                        
                        // Give away all territories fairly
                        vector<Territory*> territories = map->getTerritories();
                        //shuffle(territories.begin(), territories.end(), rng); // Randomize order of player vector, randomizing the paly order. TODO DECIDE
                        int j = 0; // index used to loop through the player array.
                        for(int i = 0; i < territories.size(); i++){
                            territories[i]->setTerritoryOwner(players[j]);
                            j++;
                            if(j == players.size()){ // Reset j to 0 if its larger than the number of players
                                j = 0;
                            }
                        }
                        map->setTerritories(territories);

                        // Give 50 armies to each players reinforcement pool
                        for(int i = 0; i < players.size(); i++){
                            players[i]->setReinforcementPool(50);
                        }

                        
                        // Draws 2 cards to each players hand
                        for(int i = 0; i < players.size(); i++){
                            players[i]->getHand()->addCard(deck->draw());
                            players[i]->getHand()->addCard(deck->draw());
                        }
                
                        processCommand(arguments[0]);
                    }
                    else{
                        cout << "ERROR: The game requires at least two players to start." << endl;
                    }
                }
            }
            else{
                cout << "ERROR: Unrecognized command in the current state." << endl;
            }
            cout << endl;
    }
} // Completes the start up phase of the game

void GameEngine::start() {
    // Print all territories and their army sizes for a player
    auto printTerritories = [](Player* p) {
        cout << "Territories for " << p->getName() << ": ";
        vector<Territory*>* territories = p->getTerritories();

        // Handle the case where the player has no territories
        if (territories == nullptr || territories->empty()) {
            cout << "(none)\n";
            return;
        }

        // Iterate over all territories and print name and army size
        for (size_t i = 0; i < territories->size(); ++i) {
            Territory* t = (*territories)[i];
            if (t == nullptr) continue;

            cout << t->getTerritoryName() << "(" << t->getArmySize() << ")";
            if (i + 1 < territories->size()) {
                cout << ", ";
            }
        }
        cout << "\n";
    };

    // Minimal test loop: executes one turn per player (one round)
    cout << "=== Executing One Round (One Turn Per Player) ===\n";

    for (Player* p : players) {
        if (p == nullptr) continue;

        // Print start-of-turn header
        cout << "\n-- Player Turn: " << p->getName() << " --\n";

        // Print territories and armies at the start of the turn
        printTerritories(p);

        // Execute all orders in the player's order list
        OrdersList* orders = p->getOrdersList();
        if (orders != nullptr) {
            for (int i = 0; i < orders->size(); ++i) {
                Order* order = orders->getOrder(i);
                if (order == nullptr) continue;

                // Validate then execute, and print results
                bool valid = order->validate();
                cout << order->getOrderType() << " | Validate: " << (valid ? "true" : "false") << "\n";
                order->execute();
                cout << order->getOrderType() << " | Effect: " << order->getEffect() << "\n";
            }
        }

        // Print territories and armies at the end of the turn
        printTerritories(p);

        // Check if player conquered at least one territory this turn
        if (p->hasConqueredTerritoryThisTurn()) {
            if (deck != nullptr) {
                Card* drawn = deck->draw();
                if (drawn != nullptr) {
                    p->getHand()->addCard(drawn);
                    cout << "Player " << p->getName() << " conquered a territory this turn; a card was awarded.\n";
                } else {
                    cout << "Player " << p->getName() << " conquered a territory this turn; no card available (deck empty).\n";
                }
            } else {
                cout << "Player " << p->getName() << " conquered a territory this turn; no card awarded (deck not initialized).\n";
            }

            // Reset the conquered flag for the next round
            p->setConqueredTerritoryThisTurn(false);
        }
    }

    // End-of-round message
    for (Player* p : players) {
        if (p != nullptr) {
            p->clearNegotiatedPlayers();
        }
    }
    cout << "\n=== End of Round ===\n";
} // minimal test loop: one round per player
