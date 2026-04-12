// developed by Lyna Taleb, 40210567
// appended by Rayan Rekmani 40283058
// appended by Adam Blevins 40255384

#pragma once

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

#include "LoggingObserver.h"

class CommandProcessor;
class Continent;
class Deck;
class Map;
class MapLoader;
class Player;
class PlayerStrategies;
class Territory;
class TournamentCommand;

class State {
public:
    State(std::string name);
    State(const State& other);
    State& operator=(const State& other);
    ~State();

    std::string getName() const;
    void addTransition(std::string command, State* nextState);
    bool isCommandValid(std::string command) const;
    State* getNextState(std::string command) const;

    friend std::ostream& operator<<(std::ostream& os, const State& state);

private:
    std::string* name;
    std::map<std::string, State*>* transitions;
};

class GameEngine : public Subject, public ILoggable {
public:
    GameEngine();
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);
    ~GameEngine() override;

    void initializeStates();
    void processCommand(std::string command);
    std::string stringToLog() const override;
    void start();
    void setPlayers(const std::vector<Player*>& players);
    void setDeck(Deck* deck);
    void setMap(Map* map);
    State* getCurrentState() const;
    bool ownsEntireContinent(Player* player, Continent* continent) const;
    void removeEliminatedPlayers();
    bool hasWinner() const;
    Player* getWinner() const;
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    std::string mainGameLoop(int maxTurns = 20);
    void startupPhase();
    std::string createTable(std::vector<std::vector<std::string>> winners);
    void processTournamentCommand(TournamentCommand* tc);

    friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

private:
    std::vector<State*>* states;
    State* currentState;
    CommandProcessor* commandProcessor;
    Deck* deck;
    MapLoader* maploader;
    Map* map;
    std::vector<Player*> players;
};