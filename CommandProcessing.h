//
// Created by Rayan Rekmani on 2026-02-24.
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

class Command : public Subject, public ILoggable {
private:
    string* command;
    string* effect;

public:
    Command(string cmd);
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    string getCommand() const;
    string getEffect() const;
    void saveEffect(string eff);
    string stringToLog() override; // many classes log (ex. CommandProcessor) so we need to override

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
    string stringToLog() override;
    friend ostream& operator<<(ostream& out, const CommandProcessor& cp);
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
