//
// Created by Rayan Rekmani on 2026-02-24.
// appended by Alyaa Shalaby
//

#include "CommandProcessing.h"
#include "GameEngine.h"
#include <cctype>


TournamentCommand::TournamentCommand() {
    mapFiles = new vector<string>();
    playerStrategies = new vector<string>();
    numberOfGames = new int(0);
    maxNumberOfTurns = new int(0);
}

TournamentCommand::TournamentCommand(const TournamentCommand& other) {
    mapFiles = new vector<string>(*other.mapFiles);
    playerStrategies = new vector<string>(*other.playerStrategies);
    numberOfGames = new int(*other.numberOfGames);
    maxNumberOfTurns = new int(*other.maxNumberOfTurns);
}

TournamentCommand& TournamentCommand::operator=(const TournamentCommand& other) {
    if (this != &other) {
        delete mapFiles;
        delete playerStrategies;
        delete numberOfGames;
        delete maxNumberOfTurns;

        mapFiles = new vector<string>(*other.mapFiles);
        playerStrategies = new vector<string>(*other.playerStrategies);
        numberOfGames = new int(*other.numberOfGames);
        maxNumberOfTurns = new int(*other.maxNumberOfTurns);
    }
    return *this;
}

TournamentCommand::~TournamentCommand() {
    delete mapFiles;
    delete playerStrategies;
    delete numberOfGames;
    delete maxNumberOfTurns;
}

void TournamentCommand::setMapFiles(const vector<string>& maps) {
    *mapFiles = maps;
}

void TournamentCommand::setPlayerStrategies(const vector<string>& strategies) {
    *playerStrategies = strategies;
}

void TournamentCommand::setNumberOfGames(int games) {
    *numberOfGames = games;
}

void TournamentCommand::setMaxNumberOfTurns(int turns) {
    *maxNumberOfTurns = turns;
}

vector<string> TournamentCommand::getMapFiles() const {
    return *mapFiles;
}

vector<string> TournamentCommand::getPlayerStrategies() const {
    return *playerStrategies;
}

int TournamentCommand::getNumberOfGames() const {
    return *numberOfGames;
}

int TournamentCommand::getMaxNumberOfTurns() const {
    return *maxNumberOfTurns;
}

ostream& operator<<(ostream& out, const TournamentCommand& tc) {
    out << "Maps: ";
    for (const auto& m : tc.getMapFiles()) {
        out << m << " ";
    }

    out << "| Strategies: ";
    for (const auto& s : tc.getPlayerStrategies()) {
        out << s << " ";
    }

    out << "| Games: " << tc.getNumberOfGames()
        << " | Max Turns: " << tc.getMaxNumberOfTurns();
    return out;
}

Command::Command(string cmd) {
    command = new string(cmd);
    effect = new string("");
      tournamentData = nullptr; //
}

Command::Command(const Command& other) {
    command = new string(*other.command);
    effect = new string(*other.effect);

     tournamentData = other.tournamentData ? new TournamentCommand(*other.tournamentData) : nullptr; //
}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        delete command;
        delete effect;
        delete tournamentData;//
        command = new string(*other.command);
        effect = new string(*other.effect);
        tournamentData = other.tournamentData ? new TournamentCommand(*other.tournamentData) : nullptr;
    }
    return *this;
}

Command::~Command() {
    delete command;
    delete effect;
    delete tournamentData;
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
//
void Command::setTournamentData(TournamentCommand* td) {
    if (tournamentData != nullptr) {
        delete tournamentData;
    }
    tournamentData = td;
}

TournamentCommand* Command::getTournamentData() const {
    return tournamentData;
}//

ostream& operator<<(ostream& out, const Command& c) {
    out << "Command: " << *c.command
        << " | Effect: " << *c.effect;
    
     if (c.tournamentData != nullptr) {
        out << " | " << *c.tournamentData;
    }//
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

string CommandProcessor::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";

    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

// Splits a string by delimiter and trims each token
vector<string> CommandProcessor::split(const string& str, char delimiter) {
    vector<string> result;
    string current = "";

    for (char ch : str) {
        if (ch == delimiter) {
            result.push_back(trim(current));
            current = "";
        } else {
            current += ch;
        }
    }

    result.push_back(trim(current));
    return result;
}

// Checks if a string contains only digits
bool CommandProcessor::isNumber(const string& str) {
    if (str.empty()) return false;

    for (char ch : str) {
        if (!isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

// Checks if tournament player strategy is one of the valid computer strategies
bool CommandProcessor::isValidStrategy(const string& strategy) {
    return strategy == "Aggressive" ||
           strategy == "Benevolent" ||
           strategy == "Neutral" ||
           strategy == "Cheater";
}

// Reject duplicate strategies 
bool CommandProcessor::hasDuplicateStrategies(const vector<string>& strategies) {
    for (int i = 0; i < static_cast<int>(strategies.size()); i++) {
        for (int j = i + 1; j < static_cast<int>(strategies.size()); j++) {
            if (strategies[i] == strategies[j]) {
                return true;
            }
        }
    }
    return false;
}


TournamentCommand* CommandProcessor::parseTournamentCommand(const string& cmd) {
    size_t mPos = cmd.find("-M ");
    size_t pPos = cmd.find("-P ");
    size_t gPos = cmd.find("-G ");
    size_t dPos = cmd.find("-D ");

    if (mPos == string::npos || pPos == string::npos ||
        gPos == string::npos || dPos == string::npos) {
        return nullptr;
    }

    if (!(mPos < pPos && pPos < gPos && gPos < dPos)) {
        return nullptr;
    }

    string mapsPart = trim(cmd.substr(mPos + 3, pPos - (mPos + 3)));
    string playersPart = trim(cmd.substr(pPos + 3, gPos - (pPos + 3)));
    string gamesPart = trim(cmd.substr(gPos + 3, dPos - (gPos + 3)));
    string turnsPart = trim(cmd.substr(dPos + 3));

    if (!isNumber(gamesPart) || !isNumber(turnsPart)) {
        return nullptr;
    }

    vector<string> maps = split(mapsPart, ',');
    vector<string> players = split(playersPart, ',');

    TournamentCommand* tc = new TournamentCommand();
    tc->setMapFiles(maps);
    tc->setPlayerStrategies(players);
    tc->setNumberOfGames(stoi(gamesPart));
    tc->setMaxNumberOfTurns(stoi(turnsPart));

    return tc;
}

// Validates tournament command 
bool CommandProcessor::validateTournamentCommand(TournamentCommand* tc) {
    if (tc == nullptr) return false;

    vector<string> maps = tc->getMapFiles();
    vector<string> players = tc->getPlayerStrategies();
    int games = tc->getNumberOfGames();
    int turns = tc->getMaxNumberOfTurns();

    if (maps.size() < 1 || maps.size() > 5) return false;
    if (players.size() < 2 || players.size() > 4) return false;
    if (games < 1 || games > 5) return false;
    if (turns < 10 || turns > 50) return false;

    for (const string& map : maps) {
        if (map.empty()) return false;
    }

    for (const string& player : players) {
        if (!isValidStrategy(player)) return false;
    }

    if (hasDuplicateStrategies(players)) return false;

    return true;
}//

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

    else if (cmd == "gamestart" && state == "players_added")
        valid = true;

    else if (cmd == "play" && state == "win")
        valid = true;

    else if (cmd == "end" && state == "win")
        valid = true;
    
     else if (cmd.rfind("tournament", 0) == 0 && state == "start") {
        TournamentCommand* tc = parseTournamentCommand(cmd);

        if (validateTournamentCommand(tc)) {
            c->setTournamentData(tc);
            c->saveEffect("Tournament command accepted");
            valid = true;
        }
        else {
            delete tc;
            c->saveEffect("Invalid tournament command");
            valid = false;
        }
    }//

    if (!valid && c->getEffect().empty())
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
