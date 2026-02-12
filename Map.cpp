//
// Created by Rayan Rekmani on 2026-02-10.
//

#include "Map.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

ostream& operator<<(ostream& os, const Map& map) {
    return os << "Map; Number of territories: "<< map.territories.size() << ", number of continents: " << map.continents.size();
}

ostream& operator<<(ostream &strm, const Continent &c){
    return strm << "Continent; Name: " << c.getName() << " , extra soldier bonus: " << c.getExtraSoldiers() << endl;
}

ostream& operator<<(ostream &strm, const Territory &t){
    return strm << "Territory; Name: " << t.getTerritoryName() << ", belonging to continent: " << t.continent->getName() << endl;
}

Map::Map() {}

//destructor
Map::~Map() {
    for (auto c: continents) {
        delete c;
    }
    for (auto t: territories) {
        delete t;
    }
}

Map::Map(const Map &map) {
    for (auto c: map.continents) {
        continents.push_back(new Continent(*c));
    }
    for (auto t: map.territories) {
        Continent* newContinent = nullptr;
        for (auto c : continents) {
            if (c->getName() == t->getContinent()->getName()) {
                newContinent = c;
                break;
            }
        }
        auto* newTerritory = new Territory(
            t->getTerritoryName(),
            t->getX(),
            t->getY(),
            newContinent
        );
        newTerritory->setArmySize(t->getArmySize());
        territories.push_back(newTerritory);
    }
    for (size_t i = 0; i < map.territories.size(); i++) {
        Territory* original = map.territories[i];
        Territory* copy = territories[i];
        for (auto neighbour : original->getNeighbours()) {
            for (auto t : territories) {
                if (t->getTerritoryName() == neighbour->getTerritoryName()) {
                    copy->addNeighbour(t);
                    break;
                }
            }
        }
    }
}

Map::Map(vector<Continent *> continents, vector<Territory *> territories) {
    this->continents = continents;
    this->territories = territories;
}

Map& Map::operator=(const Map& map) {
    if (this == &map) {
        return *this;
    }
    for (auto c: continents) {
        delete c;
    }
    for (auto t: territories) {
        delete t;
    }
    continents.clear();
    territories.clear();
    for (auto c: map.continents) {
        continents.push_back(new Continent(*c));
    }
    for (auto t: map.territories) {
        Continent* new_c = nullptr; //so we can derive adjacencies since passing the old would point to the other map
        for (auto c: continents) {
            if (c->getName()==t->getContinent()->getName()) {
                new_c = c;
                break;
            }
        }
        auto new_t = new Territory(
            t->getTerritoryName(),
            t->getX(),
            t->getY(),
            new_c);
        new_t->setArmySize(t->getArmySize());
        territories.push_back(new_t);
    }
    for (size_t i=0; i<map.territories.size(); i++) {
        Territory* original = map.territories[i];
        Territory* copy = territories[i];
        for (auto neighbour : original->getNeighbours()) {
            for (auto t : territories) {
                if (t->getTerritoryName() == neighbour->getTerritoryName()) {
                    copy->addNeighbour(t);
                    break;
                }
            }
        }
    }
    return *this;
}
vector<Territory *> Map::getTerritories() {
    return territories;
}
vector<Continent *> Map::getContinents() {
    return continents;
}

void Map::setTerritories(vector<Territory *> territories) {
    this->territories = territories;
}

void Map::setContinents(vector<Continent *> continents) {
    this->continents = continents;
}

void Map::addTerritory(Territory *territory) {
    territories.push_back(territory);
}

void Map::addContinent(Continent *continent) {
    continents.push_back(continent);
}

void Map::mapDFS(Territory *current, vector<string>& visited) {
    if (find(visited.begin(), visited.end(), current->getTerritoryName()) != visited.end()) {
        return;
    }
    visited.push_back(current->getTerritoryName());
    const auto& neighbours = current->getNeighbours();
    for (Territory* neighbour : neighbours) {
        mapDFS(neighbour, visited);
    }
}

bool Map::allTerritoriesAreConnected() {
    if (territories.empty()) {
        cout << "Map has no territories." << endl;
        return false;
    }

    vector<string> visited;
    mapDFS(territories[0], visited);


    if (visited.size() == territories.size()) {
        cout << "All territories are connected" << endl;
        return true;
    }

    cout << "Not all territories are connected" << endl;
    return false;
}

void Map::continentDFS(Territory *current, Continent *continent, vector<string> *visited) {
    // Skip if already visited
    if (find(visited->begin(), visited->end(), current->getTerritoryName()) != visited->end()) {
        return;
    }

    visited->push_back(current->getTerritoryName());
    //visit continent neighbours
    for (auto neighbour : current->getNeighbours()) {
        if (neighbour->getContinent()->getName() == continent->getName()) {
            continentDFS(neighbour, continent, visited);
        }
    }
}

bool Map::validateMap() {
    cout << endl << "Checking Map Validity..." << endl;

    bool continentsConnected = allContinentsAreConnected();
    bool territoriesConnected = allTerritoriesAreConnected();

    if (continentsConnected && territoriesConnected) {
        cout << "Map is valid!" << endl;
        return true;
    } else {
        cout << "Map is not valid!" << endl;
        if (!continentsConnected) {
            cout << "  Reason: Not all continents are connected subgraphs" << endl;
        }
        if (!territoriesConnected) {
            cout << "  Reason: Not all territories are connected" << endl;
        }
        return false;
    }
}

bool Map::allContinentsAreConnected() {
    if (continents.empty()) {
        cout << "ERROR: No continents found" << endl;
        return false;
    }

    for (int i = 0; i < continents.size(); i++) {
        vector<Territory*> territories = getTerritoriesByContinent(continents[i]);

        if (territories.empty()) {
            cout << "ERROR: Continent has no territories!" << endl;
            return false;
        }
        vector<string> visited;
        continentDFS(territories[0], continents[i], &visited);


        if (visited.size() != territories.size()) {
            cout << "ERROR: Continent is not a connected subgraph!" << endl;
            return false;
        }
    }

    cout << "All continents are connected subgraphs!" << endl;
    return true;
}

vector<Territory *> Map::getTerritoriesByContinent(Continent *continent) {
    vector<Territory*> result;
    for (Territory* territory : this->territories) {
        if (territory->getContinent()->getName() == continent->getName()) {
            result.push_back(territory);
        }
    }
    return result;
}

Continent::Continent() {
    name = "";
    extraSoldiers = 0;
}

string Continent::getName() const{
    return name;
}

int Continent::getExtraSoldiers() const {
    return extraSoldiers;
}

void Continent::setExtraSoldiers(int extraSoldiers) {
    this->extraSoldiers = extraSoldiers;
}

void Continent::setName(string name) {
    this->name = std::move(name);
}

Continent::Continent(const Continent &continent) {
    setName(continent.getName());
    setExtraSoldiers(continent.getExtraSoldiers());
}

Continent::Continent(string name, int extraSoldiers) {
    setName(std::move(name));
    setExtraSoldiers(extraSoldiers);
}

Territory::Territory() {
    continent = nullptr;
    armySize = 0;
    armySizeTemporary = 0;
    player = nullptr;
    x = 0;
    y = 0;
    territoryName = "";
}

Territory::Territory(const Territory &territory) {
    continent = territory.continent;
    armySize = territory.armySize;
    armySizeTemporary = territory.armySizeTemporary;
    player = territory.player;
    x = territory.x;
    y = territory.y;
    territoryName = territory.territoryName;
    neighbours = territory.neighbours;
}

Territory& Territory::operator=(const Territory &territory) {
    if (this != &territory) {
        this->continent = territory.continent;
        this->armySize = territory.armySize;
        this->armySizeTemporary = territory.armySizeTemporary;
        this->player = territory.player;
        this->x = territory.x;
        this->y = territory.y;
        this->territoryName = territory.territoryName;
        this->neighbours = territory.neighbours;
    }
    return *this;
}

Territory::Territory(string territoryName, int x, int y, Continent *continent) {
    this->continent = continent;
    this->territoryName = std::move(territoryName);
    this->x = x;
    this->y = y;
}

Territory::Territory(Player *player, int x, int y, Continent *continent, int armySize, string territoryName, vector<Territory *> neighbours) {
    this->continent = continent;
    this->armySize = armySize;
    this->armySizeTemporary = armySize; //due to it being uninitialized
    this->player = player;
    this->x = x;
    this->y = y;
    this->territoryName = std::move(territoryName);
    this->neighbours = std::move(neighbours);
}

Territory::Territory(Player *player, int x, int y, Continent *continent, int armySize, string territoryName) {
    this->continent = continent;
    this->armySize = armySize;
    this->armySizeTemporary = armySize; //due to it being uninitialized
    this->player = player;
    this->x = x;
    this->y = y;
    this->territoryName = std::move(territoryName);
}

Player* Territory::getTerritoryOwner() const {
    return player;
}

int Territory::getX() const {
    return x;
}

int Territory::getY() const {
    return y;
}

int Territory::getArmySize() const {
    return armySize;
}

int Territory::getArmySizeTemporary() const {
    return armySizeTemporary;
}

void Territory::setTerritoryOwner(Player *player) {
    this->player = player;
}

void Territory::setArmySize(int armySize) {
    this->armySize = armySize;
}

void Territory::setArmySizeTemporary(int armySizeTemporary) {
    this->armySizeTemporary = armySizeTemporary;
}

string Territory::getTerritoryName() const {
    return territoryName;
}

void Territory::setTerritoryName(string name) {
    this->territoryName = std::move(name);
}

void Territory::setNeighbours(vector<Territory*> neighbours) {
    this->neighbours = std::move(neighbours);
}

vector<Territory*> Territory::getNeighbours() const {
    return neighbours;
}

bool Territory::isNeighbour(Territory *territory) {
    if (this->neighbours.empty()) {
        return false;
    } else if (std::find(neighbours.begin(),neighbours.end(),territory) == neighbours.end()) {
        return false;
    }
    return true;
}

void Territory::setContinent(Continent *continent) {
    this->continent = continent;
}

void Territory::addNeighbour(Territory *neighbour) {
    this->neighbours.push_back(neighbour);
}

Continent *Territory::getContinent() const {
    return continent;
}

MapLoader::MapLoader() = default;

Map* MapLoader::loadMap(const std::string& mapFilepath) {
    std::ifstream input(mapFilepath);
    if (!input) {
        std::cout << "ERROR: File not found\n";
        return nullptr;
    }

    std::vector<Continent*> createdContinents;
    std::vector<Territory*> createdTerritories;

    std::string line;

    while (std::getline(input, line)) {
        // to skip comments, whitespace and empty lines
        if (line.empty() || line[0] == ';')
            continue;

        if (line.find("[continents]") != std::string::npos) {
            while (std::getline(input, line)) {
                if (line.empty() || line[0] == ';')
                    continue;

                // [ means the next section is there
                if (line[0] == '[')
                    break;

                std::stringstream ss(line);

                std::string name;
                int bonus;
                std::string color; // we dont use it but pass it so we can ignore it

                ss >> name >> bonus >> color;

                // skip if name is empty or parsing failed
                if (name.empty() || ss.fail()) {
                    std::cout << "WARNING: Skipping invalid continent line: '" << line << "'\n";
                    continue;
                }

                createdContinents.push_back(new Continent(name, bonus));
            }
        }

        //same procedure as continents
        if (line.find("[countries]") != std::string::npos) {
            while (std::getline(input, line)) {
                if (line.empty() || line[0] == ';')
                    continue;

                if (line[0] == '[')
                    break;

                std::stringstream ss(line);

                int id;
                std::string name;
                int continentId;
                int x, y;

                ss >> id >> name >> continentId >> x >> y;

                if (ss.fail() || name.empty()) {
                    std::cout << "WARNING: Skipping invalid territory line: '" << line << "'\n";
                    continue;
                }

                //since on the map it is 1-index but c++ is 0 index we need to -1 so just make sure it's within our range
                if (continentId < 1 || continentId > static_cast<int>(createdContinents.size())) {
                    std::cout << "ERROR: Invalid continent ID " << continentId << " for territory " << name << "\n";
                    continue;
                }

                Continent* continent = createdContinents[continentId - 1]; //here we subtract one that's why before we double check

                Territory* territory = new Territory(name, x, y, continent);
                createdTerritories.push_back(territory);
                continent->addTerritory(territory);
            }
        }
    }

    std::cout << "\nParsing complete:\n";
    std::cout << "  Continents: " << createdContinents.size() << "\n";
    std::cout << "  Territories: " << createdTerritories.size() << "\n\n";

    if (createdContinents.empty() || createdTerritories.empty()) {
        std::cout << "ERROR: Missing sections\n";
        for (auto c : createdContinents) delete c;
        for (auto t : createdTerritories) delete t;
        return nullptr;
    }

    input.clear();
    input.seekg(0); //puts the reader at the beginning as  we gonna check borders now

    while (std::getline(input, line)) {

        if (line.find("[borders]") != std::string::npos) {

            while (std::getline(input, line)) {
                if (line.empty() || line[0] == ';')
                    continue;

                if (line[0] == '[')
                    break;
                std::stringstream ss(line);

                int countryId;
                ss >> countryId;

                if (ss.fail() || countryId <= 0 || countryId > static_cast<int>(createdTerritories.size())) continue; //same concept as before

                Territory* current = createdTerritories[countryId - 1];

                int neighbourId;

                while (ss >> neighbourId) {

                    if (neighbourId <= 0 || neighbourId > static_cast<int>(createdTerritories.size()))
                        continue;

                    Territory* neighbour = createdTerritories[neighbourId - 1];

                    current->addNeighbour(neighbour);
                }
            }
            break;
        }
    }

    input.close();

    std::cout << "Map loaded successfully!\n\n";

    return new Map(createdContinents, createdTerritories);
}

void Continent::addTerritory(Territory* territory) {
    territories.push_back(territory);
}

Continent *MapLoader::getContinentByName(vector<Continent *> continents, string continentName) {
    for (const auto& continent : continents) {
        if (continent->getName() == continentName) {
            return continent;
        }
    }
    return nullptr;
}

Territory* MapLoader::getTerritoryByName(vector<Territory*> territories, string territoryName) {
    for (const auto& territory : territories) {
        if (territory->getTerritoryName() == territoryName) {
            return territory;
        }
    }
    return nullptr;
}

