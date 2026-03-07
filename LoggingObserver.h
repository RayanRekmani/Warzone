//
// Created by Rayan Rekmani on 2026-02-24.
//

#pragma once
#include <string>
#include <list>
#include <iostream>
#include <algorithm>
using std::string;
using std::list;
using std::ostream;

class ILoggable{
public:
    virtual ~ILoggable();
    virtual string stringToLog() const = 0;
};
ostream& operator<<(ostream &strm, const ILoggable &il);

class Observer{
public:
    virtual ~Observer();
    virtual void update(ILoggable*) = 0;
protected:
    Observer();
};

ostream& operator<<(ostream &strm, const Observer &o);

class Subject{
public:
    Subject();
    virtual ~Subject();
    Subject(const Subject &s);
    Subject& operator =(const Subject& s);
    virtual void attach(Observer* o);
    virtual void detach(Observer* o);
    virtual void notify(ILoggable* il);
private:
    friend ostream& operator<<(ostream&, const Subject&);
    list<Observer*>* observers;
};

ostream& operator<<(ostream &strm, const Subject &s);

class LogObserver: public Observer{
public:
    LogObserver();
    void update(ILoggable*);
};
ostream& operator<<(ostream &strm, const LogObserver &lo);