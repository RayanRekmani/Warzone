// developed by Lyna Taleb, 40210567
// appended by Rayan Rekmani 40283058
// appended by Adam Blevins 40255384
// appended by Alyaa Shalaby 40234333

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Cards.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "LoggingObserver.h"

using namespace std; // for readability

// class declarations (Stste + GameEngine)

class CommandProcessor;

// STATE CLASS --------------------------------------------------------------------
class State {
public:
    State(string name);
    State(const State& other); //copy constructor
    State& operator=(const State& other); //assignment operator
    ~State(); //destructor

    string getName() const; //which state currently in

    void addTransition(string command, State* nextState);
    bool isCommandValid(string command) const;
    State* getNextState(string command) const;

    friend ostream& operator<<(ostream& os, const State& state); //display current state

private:
    string* name; //required by prof
    map<string, State*>* transitions; //user commands -> next state
};

class TournamentCommand;

// GAMEENGINE CLASS --------------------------------------------------------------------
class GameEngine : public Subject, public ILoggable {
public:
    GameEngine();
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);
    ~GameEngine();

    void startupPhase(); // brings game to main game loop
    void start(); //main game loopp
    void processCommand(string command); //validates transition and executes if valid

    void setPlayers(const vector<Player*>& players);
    void setDeck(Deck* deck);
    void setMap(Map* map);

    State* getCurrentState() const;

    void mainGameLoop(); //main loop for assignment 2
    void reinforcementPhase(); //assign reinforcement armies to players
    void issueOrdersPhase(); //players issue orders in round robin fashion
    void executeOrdersPhase(); //execute all deploy first then remaining orders

    string stringToLog() const override;

    friend ostream& operator<<(ostream& os, const GameEngine& engine);
    void processTournamentCommand(TournamentCommand* tc);

private:
    State* currentState; //stores addy of a state obj, points to where currentState is in states vector
    vector<State*>* states;
    CommandProcessor* commandProcessor;
    Deck* deck;
    MapLoader* maploader;
    Map* map;
    vector<Player*> players;
    void initializeStates(); //creates all states and transitions between them

    bool ownsEntireContinent(Player* player, Continent* continent) const; //checks if player owns every territory in continent
    void removeEliminatedPlayers(); //removes players with no territories left
    bool hasWinner() const; //checks if game has winner
    Player* getWinner() const; //returns winner if one exists
};