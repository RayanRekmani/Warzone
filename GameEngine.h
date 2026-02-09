// developed by Lyna Taleb, 40210567
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std; // for readability

// class declarations (Stste + GameEngine)

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

    void start(); //main game loopp
    void processCommand(string command); //validates transition and executes if valid

    friend ostream& operator<<(ostream& os, const GameEngine& engine);

private:
    State* currentState; //stores addy of a state obj, points to where currentState is in states vector
    vector<State*>* states;

    void initializeStates(); //creates all states and transitions between them
};
