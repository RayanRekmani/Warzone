//
// Created by Rayan Rekmani on 2026-02-11.
//

#include <iostream>
#include "MapDriver.h"
#include "Map.h"

using std::string;
using std::cout;
using std::endl;

void testLoadMap(){

    MapLoader maploader;

    string map_brasil = "maps/brasil.map";
    string map_euro = "maps/europe.map";
    string map_solar = "maps/solar.map";
    string map_google = "maps/google.map";
    string map_europass = "maps/europass.map";

    cout << "Loading Brasil Map" << endl;
    Map* m1 = maploader.loadMap(map_brasil);
    m1->validateMap();
    cout << "----------------------------------------------" <<endl;
    cout << endl;

    cout << "Loading Euro Map" << endl;
    Map* m2 = maploader.loadMap(map_euro);
    m2->validateMap();
    cout << "----------------------------------------------" <<endl;
    cout << endl;

    cout << "Loading Solar Map" << endl;
    Map* m3 = maploader.loadMap(map_solar);
    m3->validateMap();
    cout << "----------------------------------------------" <<endl;
    cout << endl;

    cout << "Loading Google Map" << endl;
    Map* m4 = maploader.loadMap(map_google);
    m4->validateMap();
    cout << "----------------------------------------------" <<endl;
    cout << endl;

    cout << "Loading Europass Map" << endl;
    Map* m5 = maploader.loadMap(map_europass);
    m5->validateMap();
    cout << "----------------------------------------------" <<endl;
    cout << endl;

    delete m1;
    delete m2;
    delete m3;
    delete m4;
    delete m5;
}
