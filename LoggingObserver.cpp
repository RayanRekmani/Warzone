//
// Created by Rayan Rekmani on 2026-02-24.
//

#include "LoggingObserver.h"
#include <fstream>
#include <algorithm>

using std::endl;

ILoggable::~ILoggable() {}

ostream& operator<<(ostream &strm, const ILoggable &iloggable){
    return strm << iloggable.stringToLog();
}

Observer::Observer() {}

Observer::~Observer() {}

ostream& operator<<(ostream &strm, const Observer &o){
    return strm << "Observer";
}

Subject::Subject() {
    observers = new list<Observer*>();
}

Subject::~Subject() {
    delete observers;
    observers = nullptr;
}

Subject::Subject(const Subject &s) {
    observers = new list<Observer*>(*s.observers);
}

Subject &Subject::operator=(const Subject &s) {
    if (this != &s) {
        delete observers;
        observers = new list<Observer*>(*s.observers);
    }
    return *this;
}

void Subject::attach(Observer *o) {
    observers->push_back(o);
}

void Subject::detach(Observer *o) {
    observers->remove(o);
}

void Subject::notify(ILoggable *il) {
    for (Observer* o : *observers) {
        o->update(il);
    }
}

ostream& operator<<(ostream &strm, const Subject &s){
    return strm << "Subject observer count: " << s.observers->size();
}

LogObserver::LogObserver() {}

void LogObserver::update(ILoggable* il) {

    std::ofstream file("gamelog.txt", std::ios::app);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open gamelog.txt" << endl;
        return;
    }

    file << il->stringToLog() << endl;
}

ostream& operator<<(ostream &strm, const LogObserver &lo){
    return strm << "LogObserver";
}