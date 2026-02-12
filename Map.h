//
// Created by Rayan Rekmani on 2026-02-10.
//

#ifndef UNTITLED_MAP_H
#define UNTITLED_MAP_H
#include <iosfwd>
#include <string>
#include <vector>
using namespace std;
using std::string;



//forward declarations:
class Continent;
class Territory;
class MapLoader;
class Player;

class Map {
private:
    //need cout so:
    friend ostream& operator<<(ostream& os, const Map& map);
    vector<Continent*> continents;//to hold all continents
    vector<Territory*> territories;//to hold all territories
    //the following checks all regions within the SAME continent
    void continentDFS(Territory* current, Continent* continent, vector<string>* visited);
    //the following checks all regions all continents
    void mapDFS(Territory* current, vector<string>& visited);
public:
    //default constructor
    Map();
    //destructor
    ~Map();
    //parametrized constructor
    Map(vector<Continent*> continents, vector<Territory*> territories);
    //copy constructor
    Map(const Map& map);
    //assignment operator
    Map& operator=(const Map& map);

    void setContinents(vector<Continent*> continents);
    void setTerritories(vector<Territory*> territories);

    vector<Territory*> getTerritories();
    vector<Continent*> getContinents();
    vector<Territory*> getTerritoriesByContinent(Continent* continent);

    void addContinent(Continent* continent);
    void addTerritory(Territory* territory);

    bool allContinentsAreConnected();
    bool allTerritoriesAreConnected();
    bool validateMap();
};
class Continent {
private:
    friend ostream& operator<<(ostream& os, const Continent& continent);
    string name;
    int extraSoldiers{}; //added bonus if you have the entire continent
    vector<Territory*> territories;
public:
    //default
    Continent();

    string getName() const;

    //not using pointers so no need for destructor
    //copy constructor
    Continent(const Continent &continent);
    //parametrized constructor
    Continent(string name, int extraSoldiers);
    //assignment operator
    Continent& operator=(const Continent& continent);
    void setName(string name);
    void setExtraSoldiers(int extraSoldiers);
    void addTerritory(Territory* territory);
    //these ^ v take const since they don't modify the object
    int getExtraSoldiers() const;
};
class Territory {
    private:
    friend ostream& operator<<(ostream& os, const Territory& territory);
    Continent* continent; //each territory belongs to a continent
    Player* player{}; //each territory has at most one owner but that owner can change
    int armySize{};
    int armySizeTemporary{}; //allows modification during battle or new turns etc
    string territoryName;
    vector<Territory*> neighbours; // list of all territories it has access to by border
    int x,y; //actual coordinates
public:
    Territory();
    //constructor to use when opening map as it is not yet setup with player owners etc
    Territory(string territoryName, int x, int y,Continent* continent);
    Territory(const Territory& territory);
    Territory& operator=(const Territory& territory);
    Territory(Player* player, int x, int y, Continent* continent, int armySize, string territoryName);
    Territory(Player* player, int x, int y, Continent* continent, int armySize, string territoryName, vector<Territory*> neighbours);
    Player* getTerritoryOwner() const;//gets the owner of the territory
    Continent* getContinent() const;
    int getX() const;
    int getY() const;
    int getArmySize() const;
    int getArmySizeTemporary() const;
    string getTerritoryName() const;
    vector<Territory*> getNeighbours() const;
    bool isNeighbour(Territory* territory);
    void setNeighbours(vector<Territory*> neighbours);
    void setTerritoryOwner(Player* player);
    void setContinent(Continent* continent);
    void setTerritoryName(string name);
    void addNeighbour(Territory* territory);
    void setArmySize(int armySize);
    void setArmySizeTemporary(int armySizeTemporary);
};
class MapLoader {
    private:
    static Continent* getContinentByName(vector<Continent*> continents, string continentName);
    static Territory* getTerritoryByName(vector<Territory*> territories, string territoryName);
    public:
    MapLoader();
    Map* loadMap(const string& mapFilepath);
};


#endif //UNTITLED_MAP_H
