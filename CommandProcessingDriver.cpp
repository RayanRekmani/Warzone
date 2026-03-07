#include "CommandProcessing.h"
#include "GameEngine.h"
#include "LoggingObserver.h"
#include <iostream>

using namespace std;

void testCommandProcessor() {

    GameEngine* engine = new GameEngine();

    CommandProcessor* cp = new CommandProcessor(engine);

    LogObserver* logger = new LogObserver();
    cp->attach(logger);

    cout << "Enter commands (type quit to stop)\n";

    while (true) {

        Command* c = cp->getCommand();

        cout << "Command entered: " << c->getCommand() << endl;

        bool valid = cp->validate(c);

        if (valid) {
            c->saveEffect("Command accepted");

            string cmd = c->getCommand();
            string firstWord = cmd.substr(0, cmd.find(" "));
            engine->processCommand(firstWord);
        }

        cout << *c << endl;

        if (c->getCommand() == "quit")
            break;
    }

    delete cp;
    delete engine;
}

int main() {
    testCommandProcessor();
}
