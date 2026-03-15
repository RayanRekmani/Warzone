#include <iostream>
#include <fstream>
#include <sstream> 
#include "LoggingObserver.h"
#include "CommandProcessing.h"
#include "Orders.h"
#include "GameEngine.h"
using namespace std;

void testLoggingObserver() {

    // Clear the log file
    ofstream clearFile("gamelog.txt");
    clearFile << "=== Game Log ===\n";
    clearFile.close();

    // One logger shared by everything
    LogObserver* logger = new LogObserver();

   cout << "--- Demo 1: Class hierarchy ---\n";

GameEngine ge;
Subject*   s1 = static_cast<Subject*>(&ge);
ILoggable* l1 = static_cast<ILoggable*>(&ge);
(void)s1; (void)l1;
cout << "GameEngine       is Subject + ILoggable: OK\n";

OrdersList ol;
Subject*   s2 = static_cast<Subject*>(&ol);
ILoggable* l2 = static_cast<ILoggable*>(&ol);
(void)s2; (void)l2;
cout << "OrdersList       is Subject + ILoggable: OK\n";

Deploy d;
Subject*   s3 = static_cast<Subject*>(&d);
ILoggable* l3 = static_cast<ILoggable*>(&d);
(void)s3; (void)l3;
cout << "Order (Deploy)   is Subject + ILoggable: OK\n";

// CommandProcessor and Command verified in Demo 3
cout << "CommandProcessor is Subject + ILoggable: verified via Demo 3\n";
cout << "Command          is Subject + ILoggable: verified via Demo 3\n";
    
    //  GameEngine::processCommand() logs the new state

    cout << "\n--- Demo 2: GameEngine state transitions ---\n";

    ge.attach(logger);
    ge.processCommand("loadmap");       
    ge.processCommand("validatemap");   
    cout << "State transitions logged.\n";

   
    // DEMO 3: saveCommand() and saveEffect() log commands
  
    cout << "\n--- Demo 3: Command logging ---\n";

CommandProcessor cp(&ge); 
    cp.attach(logger);

    istringstream fakeInput("addplayer\n");
    streambuf* oldBuf = cin.rdbuf(fakeInput.rdbuf());
    Command* cmd = cp.getCommand();   
    cin.rdbuf(oldBuf);

    if (cmd) {
        cmd->attach(logger);
        cmd->saveEffect("Player added.");   
        cout << "Command and effect logged.\n";
    }

  
    // DEMO 4: OrdersList::addOrder() logs each added order
    
    cout << "\n--- Demo 4: Adding orders to list ---\n";

    ol.attach(logger);

    Deploy*  deploy = new Deploy();
    Advance* adv    = new Advance();
    deploy->attach(logger);
    adv->attach(logger);

    ol.addOrder(deploy);   
    ol.addOrder(adv);     
    cout << "Orders added and logged.\n";

  
    // DEMO 5: Order::execute() logs the effect
  
    cout << "\n--- Demo 5: Executing orders ---\n";


    deploy->execute();   
    adv->execute();     
    cout << "Order effects logged.\n";


    cout << "\n--- Contents of gamelog.txt ---\n";
    ifstream logFile("gamelog.txt");
    string line;
    while (getline(logFile, line)) {
        cout << "  " << line << "\n";
    }
    cout << "-------------------------------\n";

    delete logger;
}

int main() {
    testLoggingObserver();
    return 0;
}
