// Written by Adam Blevins, 40255384

#pragma once
#include "GameEngine.h"
#include "CommandProcessing.h"
#include "LoggingObserver.h"
#include "Map.h"
#include "Cards.h"
#include "Player.h"
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

int main() {
    GameEngine* engine = new GameEngine();
    engine->startupPhase();
    cout << *engine;

    return 0;
}