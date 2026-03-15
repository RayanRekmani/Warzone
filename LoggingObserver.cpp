//
// Created by Rayan Rekmani on 2026-02-24.
//
// appended by Alyaa Shalaby 40234333
#include "LoggingObserver.h"
#include <fstream>

using std::endl;

// =========================================================================
// ILoggable
// =========================================================================

/** @brief Default constructor — no data members to initialise. */
ILoggable::ILoggable() {}

/** @brief Copy constructor — no data members to copy. */
ILoggable::ILoggable(const ILoggable& other) {
    (void)other;
}

/**
 * @brief Assignment operator — no data members to assign.
 * @return Reference to this object.
 */
ILoggable& ILoggable::operator=(const ILoggable& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

/** @brief Destructor. */
ILoggable::~ILoggable() {}

/**
 * @brief Streams the loggable string representation to an output stream.
 * @param strm  The output stream.
 * @param il    The ILoggable object.
 * @return Reference to strm for chaining.
 */
ostream& operator<<(ostream& strm, const ILoggable& il) {
    return strm << il.stringToLog();
}

// =========================================================================
// Observer
// =========================================================================

/** @brief Default constructor. */
Observer::Observer() {}

/** @brief Copy constructor. */
Observer::Observer(const Observer& other) {
    (void)other;
}

/**
 * @brief Assignment operator.
 * @return Reference to this object.
 */
Observer& Observer::operator=(const Observer& other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

/** @brief Virtual destructor. */
Observer::~Observer() {}

/**
 * @brief Streams a generic description of the Observer.
 * @param strm  The output stream.
 * @param o     The Observer object.
 * @return Reference to strm for chaining.
 */
ostream& operator<<(ostream& strm, const Observer& o) {
    (void)o;
    return strm << "Observer";
}

// =========================================================================
// Subject
// =========================================================================

/**
 * @brief Default constructor — allocates an empty observer list on the heap.
 */
Subject::Subject() {
    observers = new list<Observer*>();
}

/**
 * @brief Destructor — releases the heap-allocated observer list.
 *        Does NOT delete the Observer objects themselves (they are owned
 *        by the caller).
 */
Subject::~Subject() {
    delete observers;
    observers = nullptr;
}

/**
 * @brief Copy constructor — performs a deep copy of the observer list.
 * @param s The Subject to copy.
 */
Subject::Subject(const Subject& s) {
    observers = new list<Observer*>(*s.observers);
}

/**
 * @brief Assignment operator — replaces the observer list with a deep copy.
 * @param s The Subject to assign from.
 * @return Reference to this object.
 */
Subject& Subject::operator=(const Subject& s) {
    if (this != &s) {
        delete observers;
        observers = new list<Observer*>(*s.observers);
    }
    return *this;
}

/**
 * @brief Adds an observer to the notification list.
 * @param o Pointer to the Observer to attach (must not be null).
 */
void Subject::attach(Observer* o) {
    observers->push_back(o);
}

/**
 * @brief Removes an observer from the notification list.
 * @param o Pointer to the Observer to detach.
 */
void Subject::detach(Observer* o) {
    observers->remove(o);
}

/**
 * @brief Calls update(il) on every attached Observer.
 *
 * Game classes invoke this after any state change that should be logged,
 * passing themselves (as ILoggable*) so that stringToLog() is called
 * on the correct object.
 *
 * @param il Pointer to the ILoggable whose state just changed.
 */
void Subject::notify(ILoggable* il) {
    for (Observer* o : *observers) {
        o->update(il);
    }
}

/**
 * @brief Streams the number of currently attached observers.
 * @param strm  The output stream.
 * @param s     The Subject object.
 * @return Reference to strm for chaining.
 */
ostream& operator<<(ostream& strm, const Subject& s) {
    return strm << "Subject observer count: " << s.observers->size();
}

// =========================================================================
// LogObserver
// =========================================================================

/** @brief Default constructor. */
LogObserver::LogObserver() : Observer() {}

/** @brief Copy constructor. */
LogObserver::LogObserver(const LogObserver& other) : Observer(other) {
    (void)other;
}

/**
 * @brief Assignment operator.
 * @return Reference to this object.
 */
LogObserver& LogObserver::operator=(const LogObserver& other) {
    if (this != &other) {
        Observer::operator=(other);
    }
    return *this;
}

/** @brief Destructor. */
LogObserver::~LogObserver() {}

/**
 * @brief Appends a log entry to "gamelog.txt".
 *
 * Opens "gamelog.txt" in append mode, writes the string produced by
 * il->stringToLog() followed by a newline, then closes the file.
 * Prints an error to stderr if the file cannot be opened.
 *
 * @param il Pointer to the ILoggable whose stringToLog() provides the entry.
 */
void LogObserver::update(ILoggable* il) {
    std::ofstream file("gamelog.txt", std::ios::app);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open gamelog.txt" << endl;
        return;
    }

    file << il->stringToLog() << endl;
    file.close();
}

/**
 * @brief Streams a generic description of the LogObserver.
 * @param strm  The output stream.
 * @param lo    The LogObserver object.
 * @return Reference to strm for chaining.
 */
ostream& operator<<(ostream& strm, const LogObserver& lo) {
    (void)lo;
    return strm << "LogObserver";
}
