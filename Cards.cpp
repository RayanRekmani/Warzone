// Written by Adam Blevins, 40255384

// TODO: Finalize order creation during the play() method.
#pragma once
#include "Cards.h"
#include "Orders.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

/*
*-------------------------------------------------------------------
* Class Hand 
*-------------------------------------------------------------------
*/

// Default constructor
Hand::Hand(){

}

// Parameter constructor. Takes a Player pointer p and sets the Hand object's player attribute to p.
Hand::Hand(Player* p){
    this->player = p;
}

// Copy Constructor. Copies each card in the given hand.
Hand::Hand(const Hand& hand){
    for(int i = 0; i < hand.hand.size(); i++){
        this->addCard(hand.hand[i]);
    }

}

// Adds a given Card pointer to the bottom of the hand.
void Hand::addCard(Card* card){
    this->hand.push_back(card);
}

// Removes a card from the hand at a given index. Used in the playCard() method to remove a card after its played.
void Hand::removeCard(int i){
    this->hand.erase(this->hand.begin() + i);
}

// Returns a Card pointer in the hand at an index i.
Card* Hand::getCard(int i){
    return this->hand[i];
}

// Returns the vector of all cards in the hand.
vector<Card*> Hand::getAllCards(){
    return this->hand;
}

// Returns the number of cards in the hand.
int Hand::getSize(){
    return this->hand.size();
}

// Method to play a card at an index i and removes it from the hand. 
void Hand::playCard(int i, Deck* deck, OrdersList* ordersList){
    // Each card has a play() method that overrides the main card classes play() method, which each creates an order corresponding to the card's type and adds it to an OrdersList.
   ordersList->addOrder(this->hand[i]->play()); 
   deck->addCard(this->hand[i]);
   this->removeCard(i);
}

// Setter for the Player pointer in the hand.
void Hand::setPlayer(Player* p){
    this->player = p;
}

// Prints each card in the hand.
ostream& Hand::print(ostream& out){
    out << "This hand contains: " << endl;
    for(int i = 0; i < this->hand.size(); i++){
        hand[i]->print(out);
    }
    return out;
}

// Redirects << operator to Hand's print method.
ostream& operator <<(ostream& strm, Hand& a){
    return a.print(strm);
}

// Destructor. Deletes every card in the hand.
Hand::~Hand(){
    for(int i = 0; i < this->hand.size(); i++){
        delete this->hand[i];
        this->hand[i] = nullptr;
    }
}

/*
*-------------------------------------------------------------------
* Class Deck
*-------------------------------------------------------------------
*/

// Default constructor
Deck::Deck(){

}

// Copy Constructor  
Deck::Deck(const Deck& deck){
    // For each card in the deck being copied, add a copy of it to the end of the end of the new Deck's deck.
    for(int i = 0; i < deck.deck.size(); i++){
        this->addCard(deck.deck[i]->copy());
    }
}

// Draws and returns card from the bottom of the deck and removes it from the deck. Returns nullptr if the deck is empty.
Card* Deck::draw(){
    // If the deck is not empty, return the last card and remove it from the deck.
    if(this->deck.size() > 0){ 
        Card* card = this->deck[this->deck.size()-1];
        this->deck.pop_back(); 
        return card;
    }
    // If the deck is empty, return a nullptr
    return nullptr;
}

// Adds a Card pointer to the bottom of the deck.
void Deck::addCard(Card* card) {
    deck.push_back(card);
}

// Returns size of the deck vector.
int Deck::getSize(){
    return deck.size();
}

// Draws a card from the deck and adds it to a given hand.
void Deck::drawToHand(Hand* hand) {
    hand->addCard(this->draw());
}

// Print method to print every card in the deck.
ostream& Deck::print(ostream& out){
    out << "This deck contains: " << endl;
    for(int i = 0; i < deck.size(); i++){
        deck[i]->print(out); // Each card has a print() method that overrides the base Card classes print, meaning the correct type of card is printed through polymorphism.
    }
    return out;
}

// Redirects << operator to the print() method.
ostream& operator<<(ostream& strm, Deck& a){
    return a.print(strm);
}

// Assignment operator overload. Copies the deck from one Deck to another.
Deck& Deck::operator=(const Deck& d){
    if(this != &d){ // If the decks aren't the same, add each card to this deck's deck.
        for(int i = 0; i < d.deck.size(); i++){
            this->deck.push_back(d.deck[i]);
        }
    }
    return *this;
}

// Shuffles the deck using the random_shuffle() method.
void Deck::shuffle(){
    random_shuffle(this->deck.begin(), this->deck.end());
}

// Destructor. Deletes every card in the deck.
Deck::~Deck(){
    for(int i = 0; i < this->deck.size(); i++){
        delete this->deck[i];
        this->deck[i] = nullptr;
    }
}

/*
*-------------------------------------------------------------------
* Class Card 
*-------------------------------------------------------------------
*/

// Default Constructor
Card::Card(){

}

// Copy Constructor. Since each Card class has no attributes it just returns a new card of the same type.
Card::Card(const Card& card){

}

// Copy method used in the Deck class's copy constructor. Each derived card has a copy() method that overrides this one.
Card* Card::copy(){
    return new Card();
}

// Plays the card. Since the method doesn't remove the card from the hand playing it, the playCard() method should be used when playing a card.
// Returns an order corresponding to the card's type.
Order* Card::play(){
    return nullptr; // This specific method shouldn't be called, if it is then an error has happened.
}

// Print method for the type of card. Each derived card has their own overriding print() method that prints which type of card it is.
ostream& Card::print(ostream& out){
    return out << "This is a card.";
}

// Redirects << operator to the print() method.
ostream& operator<<(ostream& strm, Card& a){
    return a.print(strm);
}

// Assignment operator overload
Card& Card::operator=(const Card& c){
    return *this;
}

// Destructor
Card::~Card(){
    
}

/*
*-------------------------------------------------------------------
* Class AirliftCard
*-------------------------------------------------------------------
*/

// Default Constructor
AirliftCard::AirliftCard(){

}

// Copy Constructor. Since each Card class has no attributes it just returns a new card of the same type.
AirliftCard::AirliftCard(const AirliftCard& airliftCard){

}

// Copy method used in the Deck class's copy constructor.
Card* AirliftCard::copy(){
    return new AirliftCard();
}

// Plays the card. Since the method doesn't remove the card from the hand playing it, the playCard() method should be used when playing a card.
// Returns an airlift order.
Order* AirliftCard::play(){
    return new Airlift(); // Placeholder
}

// Prints that this is an airlift card.
ostream& AirliftCard::print(ostream& out){
    return out << "This is an airlift card." << endl;
}

// Redirects << operator to the print() method.
ostream& operator<<(ostream& strm, AirliftCard& a){
    return a.print(strm);
}

// Assignment operator override
AirliftCard& AirliftCard::operator=(const AirliftCard& c){
    return *this;
}

// Destructor
AirliftCard::~AirliftCard(){
    
}

/*
*-------------------------------------------------------------------
* Class BombCard 
*-------------------------------------------------------------------
*/

// Default Constructor
BombCard::BombCard(){

}

// Copy Constructor. Since each Card class has no attributes it just returns a new card of the same type.
BombCard::BombCard(const BombCard& bombCard){

}

// Copy method used in the Deck class's copy constructor.
Card* BombCard::copy(){
    return new BombCard();
}

// Plays the card. Since the method doesn't remove the card from the hand playing it, the playCard() method should be used when playing a card.
// Returns a bomb order.
Order* BombCard::play(){
    return new Bomb(); // Placeholder
}

// Prints that this is an airlift card.
ostream& BombCard::print(ostream& out){
    return out << "This is a bomb card." << endl;
}

// Redirects << operator to the print() method.
ostream& operator<<(ostream& strm, BombCard& a){
    return a.print(strm);
}

// Assignment operator override
BombCard& BombCard::operator=(const BombCard& c){
    return *this;
}

// Destructor
BombCard::~BombCard(){
    
}

/*
*-------------------------------------------------------------------
* Class DiplomacyCard
*-------------------------------------------------------------------
*/

// Default Constructor
DiplomacyCard::DiplomacyCard(){

}

// Copy Constructor. Since each Card class has no attributes it just returns a new card of the same type.
DiplomacyCard::DiplomacyCard(const DiplomacyCard& diplomacyCard){

}

// Copy method used in the Deck class's copy constructor.
Card* DiplomacyCard::copy(){
    return new DiplomacyCard();
}

// Plays the card. Since the method doesn't remove the card from the hand playing it, the playCard() method should be used when playing a card.
// Returns a negotiate order.
Order* DiplomacyCard::play(){
    return new Negotiate(); // Placeholder
}

// Prints that this is an airlift card.
ostream& DiplomacyCard::print(ostream& out){
    return out << "This is a diplomacy card." << endl;
}

// Redirects << operator to the print() method.
ostream& operator<<(ostream& strm, DiplomacyCard& a){
    return a.print(strm);
}

// Assignment operator override
DiplomacyCard& DiplomacyCard::operator=(const DiplomacyCard& c){
    return *this;
}

// Destructor
DiplomacyCard::~DiplomacyCard(){
    
}

/*
*-------------------------------------------------------------------
* Class ReinforcementCard
*-------------------------------------------------------------------
*/

// Default Constructor
ReinforcementCard::ReinforcementCard(){

}

// Copy Constructor. Since each Card class has no attributes it just returns a new card of the same type.
ReinforcementCard::ReinforcementCard(const ReinforcementCard& reinforcementCard){

}

// Copy method used in the Deck class's copy constructor.
Card* ReinforcementCard::copy(){
    return new ReinforcementCard();
}

// Plays the card. Since the method doesn't remove the card from the hand playing it, the playCard() method should be used when playing a card.
// Returns a deploy order.
Order* ReinforcementCard::play(){
    return new Deploy(); // Placeholder
}

// Prints that this is an airlift card.
ostream& ReinforcementCard::print(ostream& out){
    return out << "This is a reinforcement card." << endl;
}

// Redirects << operator to the print() method.
ostream& operator<<(ostream& strm, ReinforcementCard& a){
    return a.print(strm);
}

// Assignment operator override
ReinforcementCard& ReinforcementCard::operator=(const ReinforcementCard& c){
    return *this;
}

// Destructor
ReinforcementCard::~ReinforcementCard(){
    
}

/*
*-------------------------------------------------------------------
* Class BlockadeCard
*-------------------------------------------------------------------
*/

// Default Constructor
BlockadeCard::BlockadeCard(){
   
}

// Copy Constructor. Since each Card class has no attributes it just returns a new card of the same type.
BlockadeCard::BlockadeCard(const BlockadeCard& blockadeCard){
   
}

// Copy method used in the Deck class's copy constructor.
Card* BlockadeCard::copy(){
    return new BlockadeCard();
}

// Plays the card. Since the method doesn't remove the card from the hand playing it, the playCard() method should be used when playing a card.
// Returns a blockade order.
Order* BlockadeCard::play(){
    return new Blockade(); // Placeholder
}

// Prints that this is a blockade card.
ostream& BlockadeCard::print(ostream& out){
    return out << "This is a blockade card." << endl;
}

// Redirects << operator to the print() method.
ostream& operator<<(ostream& strm, BlockadeCard& a){
    return a.print(strm);
}

// Assignment operator override
BlockadeCard& BlockadeCard::operator=(const BlockadeCard& c){
    return *this;
}

// Destructor
BlockadeCard::~BlockadeCard(){
    
}


