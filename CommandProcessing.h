//
// Created by Rayan Rekmani on 2026-02-24.
// Appenede by Alyaa Shalaby
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "GameEngine.h"
#include "LoggingObserver.h"

using namespace std;

// ILoggable is an interface for objects that can be logged

//
class TournamentCommand {
private:
    vector<string>* mapFiles;
    vector<string>* playerStrategies;
    int* numberOfGames;
    int* maxNumberOfTurns;

public:
    TournamentCommand();
    TournamentCommand(const TournamentCommand& other);
    TournamentCommand& operator=(const TournamentCommand& other);
    ~TournamentCommand();

    void setMapFiles(const vector<string>& maps);
    void setPlayerStrategies(const vector<string>& strategies);
    void setNumberOfGames(int games);
    void setMaxNumberOfTurns(int turns);

    vector<string> getMapFiles() const;
    vector<string> getPlayerStrategies() const;
    int getNumberOfGames() const;
    int getMaxNumberOfTurns() const;

    friend ostream& operator<<(ostream& out, const TournamentCommand& tc);
};
//



class Command : public Subject, public ILoggable {
private:
    string* command;
    string* effect;
    TournamentCommand* tournamentData; //

public:
    Command(string cmd);
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    string getCommand() const;
    string getEffect() const;
    void saveEffect(string eff);
    void setTournamentData(TournamentCommand* td);
    TournamentCommand* getTournamentData() const; //
    string stringToLog() const override; // many classes log (ex. CommandProcessor) so we need to override

    friend ostream& operator<<(ostream& out, const Command& c);
};

class CommandProcessor : public Subject, public ILoggable {
protected:
    //these are protected so derived classes (FileCommandProcessorAdapter) can still access internal data and override behavior
    vector<Command*>* commands;
    GameEngine* gameEngine;
    virtual string readCommand();   //using virtual so it's overridable and we can use the correct  version
    void saveCommand(Command* c);

public:
    CommandProcessor();
    CommandProcessor(GameEngine*);
    CommandProcessor(const CommandProcessor& other);
    CommandProcessor& operator=(const CommandProcessor& other);
    virtual ~CommandProcessor(); //needs to be virtual so proper derived destructors run
    Command* getCommand();
    bool validate(Command* c);
    void setGameEngine(GameEngine* ge);
    string stringToLog() const override;
    friend ostream& operator<<(ostream& out, const CommandProcessor& cp);

    TournamentCommand* parseTournamentCommand(const string& cmd);
    bool validateTournamentCommand(TournamentCommand* tc);

    vector<string> split(const string& str, char delimiter);
    string trim(const string& str);
    bool isNumber(const string& str);
    bool hasDuplicateStrategies(const vector<string>& strategies);
    bool isValidStrategy(const string& strategy); //
};


class FileLineReader {
private:
    ifstream file;
    string fileName;
    friend ostream& operator<<(ostream&, const FileLineReader&);
public:
    FileLineReader(string filename);
    FileLineReader();
    ~FileLineReader();
    string readLineFromFile();
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* flr;
protected:
    string readCommand() override;
public:
    FileCommandProcessorAdapter(GameEngine* gameEngine, string fileName);
    FileCommandProcessorAdapter(string fileName);
    FileCommandProcessorAdapter();
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &f);
    ~FileCommandProcessorAdapter() override;
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& f);
};

void testCommandProcessor(int argc, char* argv[]);

