//
// Created by Rayan Rekmani on 2026-02-24.
//
// appended by Alyaa Shalaby 40234333
#pragma once
#include <string>
#include <list>
#include <iostream>
#include <algorithm>
using std::string;
using std::list;
using std::ostream;
class ILoggable {
public:
    /** @brief Default constructor. */
    ILoggable();

    /** @brief Copy constructor. */
    ILoggable(const ILoggable& other);

    /** @brief Assignment operator. */
    ILoggable& operator=(const ILoggable& other);

    /** @brief Virtual destructor to allow safe polymorphic deletion. */
    virtual ~ILoggable();

    /**
     * @brief Returns the string that should be appended to the log file.
     * @return A single-line description of the loggable event.
     */
    virtual string stringToLog() const = 0;
};

/** @brief Stream insertion operator for ILoggable (delegates to stringToLog()). */
ostream& operator<<(ostream& strm, const ILoggable& il);

// =========================================================================
// Observer
// =========================================================================

/**
 * @class Observer
 * @brief Abstract base class for all listeners in the Observer pattern.
 *
 * Subclasses override update() to react when a Subject notifies them of a
 * state change.  The constructor and copy operations are protected to
 * prevent direct instantiation.
 */
class Observer {
public:
    /** @brief Virtual destructor. */
    virtual ~Observer();

    /**
     * @brief Called by a Subject when its state has changed.
     * @param il Pointer to the ILoggable whose state changed.
     */
    virtual void update(ILoggable* il) = 0;

protected:
    /** @brief Default constructor (protected — only subclasses may instantiate). */
    Observer();

    /** @brief Copy constructor (protected). */
    Observer(const Observer& other);

    /** @brief Assignment operator (protected). */
    Observer& operator=(const Observer& other);
};

/** @brief Stream insertion operator for Observer. */
ostream& operator<<(ostream& strm, const Observer& o);

// =========================================================================
// Subject
// =========================================================================

/**
 * @class Subject
 * @brief Maintains a list of Observer objects and broadcasts notifications.
 *
 * Game classes that need to notify the log observer (GameEngine, Order,
 * OrdersList, Command, CommandProcessor) inherit from Subject.  They call
 * notify(this) after any significant state change to push a log entry.
 */
class Subject {
public:
    /** @brief Default constructor — allocates the internal observer list. */
    Subject();

    /** @brief Copy constructor — deep-copies the observer list. */
    Subject(const Subject& s);

    /** @brief Assignment operator — deep-copies the observer list. */
    Subject& operator=(const Subject& s);

    /** @brief Destructor — deallocates the internal observer list. */
    virtual ~Subject();

    /**
     * @brief Registers an observer to receive future notifications.
     * @param o Pointer to the Observer to attach.
     */
    virtual void attach(Observer* o);

    /**
     * @brief Removes a previously registered observer.
     * @param o Pointer to the Observer to detach.
     */
    virtual void detach(Observer* o);

    /**
     * @brief Notifies all attached observers of a state change.
     * @param il Pointer to the ILoggable subject whose state changed.
     */
    virtual void notify(ILoggable* il);

private:
    friend ostream& operator<<(ostream&, const Subject&);

    /** @brief Heap-allocated list of attached observers. */
    list<Observer*>* observers;
};

/** @brief Stream insertion operator for Subject (prints observer count). */
ostream& operator<<(ostream& strm, const Subject& s);

// =========================================================================
// LogObserver
// =========================================================================

/**
 * @class LogObserver
 * @brief Concrete Observer that appends a log entry to "gamelog.txt".
 *
 * When update() is called, it opens "gamelog.txt" in append mode, writes
 * the string returned by ILoggable::stringToLog(), then closes the file.
 * A single LogObserver instance should be attached to every Subject in the
 * game (GameEngine, CommandProcessor, each Command, each Order, each
 * OrdersList) so that all events are captured in one chronological file.
 */
class LogObserver : public Observer {
public:
    /** @brief Default constructor. */
    LogObserver();

    /** @brief Copy constructor. */
    LogObserver(const LogObserver& other);

    /** @brief Assignment operator. */
    LogObserver& operator=(const LogObserver& other);

    /** @brief Destructor. */
    ~LogObserver() override;

    /**
     * @brief Appends il->stringToLog() to "gamelog.txt".
     * @param il Pointer to the ILoggable whose state was just recorded.
     */
    void update(ILoggable* il) override;
};

/** @brief Stream insertion operator for LogObserver. */
ostream& operator<<(ostream& strm, const LogObserver& lo);
