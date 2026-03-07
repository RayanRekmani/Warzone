//
// Created by Rayan Rekmani on 2026-02-24.
//

#include "CommandProcessing.h"

Command::Command(string cmd) {
    command = new string(cmd);
    effect = new string("");
}

Command::Command(const Command& other) {
    command = new string(*other.command);
    effect = new string(*other.effect);
}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        delete command;
        delete effect;
        command = new string(*other.command);
        effect = new string(*other.effect);
    }
    return *this;
}

Command::~Command() {
    delete command;
    delete effect;
}

string Command::getCommand() const { return *command; }
string Command::getEffect() const { return *effect; }

void Command::saveEffect(string eff) {
    *effect = eff;
    notify(this);
}

string Command::stringToLog() const {
    return "Command Effect: " + *effect;
}

ostream& operator<<(ostream& out, const Command& c) {
    out << "Command: " << *c.command
        << " | Effect: " << *c.effect;
    return out;
}

CommandProcessor::CommandProcessor() {
    commands = new vector<Command*>;
    gameEngine = nullptr;
}

CommandProcessor::CommandProcessor(GameEngine* ge) {
    commands = new vector<Command*>;
    gameEngine = ge;
}

CommandProcessor::CommandProcessor(const CommandProcessor& other) {
    gameEngine = other.gameEngine;
    commands = new vector<Command*>;
    for (auto c : *other.commands)
        commands->push_back(new Command(*c));
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        for (auto c : *commands) delete c;
        commands->clear();

        gameEngine = other.gameEngine;

        for (auto c : *other.commands)
            commands->push_back(new Command(*c));
    }
    return *this;
}

CommandProcessor::~CommandProcessor() {
    for (auto c : *commands) delete c;
    delete commands;
}

void CommandProcessor::setGameEngine(GameEngine* ge) {
    gameEngine = ge;
}

string CommandProcessor::readCommand() {
    string cmd;
    getline(cin, cmd);
    return cmd;
}

void CommandProcessor::saveCommand(Command* c) {
    commands->push_back(c);
    notify(this);
}

Command* CommandProcessor::getCommand() {
    string cmd = readCommand();
    Command* c = new Command(cmd);
    saveCommand(c);
    return c;
}

bool CommandProcessor::validate(Command* c) {

    if (!gameEngine) return false;

    string state = gameEngine->getCurrentState()->getName();
    string cmd = c->getCommand();

    bool valid = false;

    if (cmd.rfind("loadmap", 0) == 0 &&
        (state == "start" || state == "map_loaded"))
        valid = true;

    else if (cmd == "validatemap" && state == "map_loaded")
        valid = true;

    else if (cmd.rfind("addplayer", 0) == 0 &&
            (state == "map_validated" || state == "players_added"))
        valid = true;

    else if (cmd == "assigncountries" && state == "players_added")
        valid = true;

    else if (cmd == "play" && state == "win")
        valid = true;

    else if (cmd == "end" && state == "win")
        valid = true;

    if (!valid)
        c->saveEffect("Invalid command in current state");

    return valid;
}

string CommandProcessor::stringToLog() const {
    if (commands->empty()) return "";
    return "Command: " + commands->back()->getCommand();
}

ostream& operator<<(ostream& out, const CommandProcessor& cp) {
    for (auto c : *cp.commands)
        out << *c << endl;
    return out;
}

FileLineReader::FileLineReader() {
}

FileLineReader::FileLineReader(string filename) {
    file.open(filename);
}

FileLineReader::~FileLineReader() {
    if (file.is_open())
        file.close();
}

string FileLineReader::readLineFromFile() {
    string line;
    getline(file, line);
    return line;
}


// =====================================================
// ADAPTER
// =====================================================


FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine* ge, string filename) : CommandProcessor(ge) {
    flr = new FileLineReader(filename);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
}

string FileCommandProcessorAdapter::readCommand() {
    return flr->readLineFromFile();
}

void testCommandProcessor(int argc, char* argv[]) {

    GameEngine* engine = new GameEngine();
    CommandProcessor* cp = nullptr;

    if (argc > 1 && string(argv[1]) == "-file") {
        cp = new FileCommandProcessorAdapter(engine, argv[2]);
        cout << "Reading commands from file\n";
    }
    else {
        cp = new CommandProcessor(engine);
        cout << "Reading commands from console\n";
    }


    engine->start();

    delete cp;
    delete engine;
}