// developed by Lyna Taleb, 40210567
// appended by Rayan Rekmani 40283058
// appended by Adam Blevins 40255384

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Cards.h"
#include "Map.h"

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

// GAMEENGINE CLASS --------------------------------------------------------------------
class GameEngine {
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

    friend ostream& operator<<(ostream& os, const GameEngine& engine);

private:
    State* currentState; //stores addy of a state obj, points to where currentState is in states vector
    vector<State*>* states;
    CommandProcessor* commandProcessor;
    Deck* deck;
    MapLoader* maploader;
    Map* map;
    vector<Player*> players;
    void initializeStates(); //creates all states and transitions between them
};
