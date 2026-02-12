// Written by Adam Blevins, 40255384
#pragma once
#include <vector>
#include <iostream>
#include "Orders.h"

using namespace std;

class Hand;
class Deck;
class Card;
class Player;

/*
* The Hand class keeps track of the cards in a player's hand.
*/

class Hand {

    public:
        Hand(); // Default constructor
        Hand(Player* p); // Parameter constructor with a Player pointer.
        Hand(const Hand&); // Copy constructor
        void addCard(Card* card); // Adds a card object to hand.
        void removeCard(int); // Removes card from the hand at index i.
        Card* getCard(int); // Returns a pointer to the Card at index i.
        vector<Card*> getAllCards(); // Returns the vector of all cards in the hand.
        int getSize(); // Returns the number of cards in the hand.
        void setPlayer(Player* p); // Sets player attribute to Player pointer p.
        void playCard(int, Deck*, OrdersList*); // Calls the play() method of a card stored in the hand at index i. Returns corresponding order to card type and puts the card back in the deck.
        ostream& print(ostream&); // Prints each card in the hand.
        ~Hand(); // Deconstructor
    private:
        vector<Card*> hand; // Holds the cards in the player's hand.
        Player* player; // Pointer to which player owns the hand.
        
};

/*
* The Deck class contains pointers to all cards used in the game's deck of cards. It can be drawn from by any hand. 
*/

class Deck {

    public:
        Deck(); // Default constructor
        Deck(const Deck&); // Copy constructor
        Card* draw(); // Returns first card in the deck vector and removes it.
        void drawToHand(Hand*); // Removes bottom card from deck and adds it to a hand.
        void addCard(Card*); // Adds a card to the end of the deck
        int getSize(); // Returns size of the deck vector.
        ostream& print(ostream&); // Prints all the cards in the deck, first to last.
        Deck& operator=(const Deck&); // Assignment operator overload
        void shuffle(); // Shuffles the deck
        ~Deck(); // Deconstructor
    private:
        vector<Card*> deck; // Vector that stores all the cards in the deck



};

/*
* The Card class is the base class of all derived subtypes of cards. A Card should never be declared. Only its children should be initialized.
* The implementation of the card system relies heavily on polymorphism to work.
*/

class Card { 

    public:
        Card(); // Default constructor
        Card(const Card&); // Copy constructor
        virtual Card* copy(); // Returns a copy of a card. Since each card has no attributes it returns a new card of the same type. Used in the copy constructor for the Deck class.
        virtual Order* play(); // Creates and returns an order corresponding the card and removes the card from the hand. All of Card's children have a play() method. 
        virtual ostream& print(ostream&); // Prints what type of card it is. All of Card's children have a print() method that overloads it.
        Card& operator=(const Card&); // Assignment operator overload
        ~Card(); // Deconstructor

    
};

/*
* Airlift card class, extends Card. Allows player to move troops from any territory they control to any other territory they control.
*/

class AirliftCard: public Card {

    public: 
        AirliftCard(); // Default constructor
        AirliftCard(const AirliftCard&); // Copy constructor
        Card* copy(); // Returns new airlift card.
        Order* play(); // Plays the airlift card.Returns a new order corresponding to the card's type (Airlift).
        ostream& print(ostream&); // Prints what type of card it is. Inserts string "This is an airlift card." to ostream.
        AirliftCard& operator=(const AirliftCard&); // Assignment operator overload
        ~AirliftCard(); // Deconstructor

};

/*
* Blockade card class, extends Card. Allows player to make a territory they control neutral, but multiplies the number of troops in it by a factor.
*/

class BlockadeCard: public Card {

    public: 
        BlockadeCard(); // Default constructor
        BlockadeCard(const BlockadeCard&); // Copy constructor
        Card* copy(); // Returns new blockade card.
        Order* play(); // Plays the blockade card. Returns a new order corresponding to the card's type (Blockade).
        ostream& print(ostream&); // Prints what type of card it is. Inserts string "This is a blockade card." to ostream.
        BlockadeCard& operator=(const BlockadeCard&); // Assignment operator overload
        ~BlockadeCard(); // Deconstructor

};

/*
* Reinforcement card class, extends Card. Allows player to add a certain number of extra troops.
*/

class ReinforcementCard: public Card {

    public: 
        ReinforcementCard(); // Default constructor
        ReinforcementCard(const ReinforcementCard&); // Copy constructor
        Card* copy(); // Returns new reinforcement card.
        Order* play(); // Plays the reinforcement card. Returns a new order corresponding to the card's type (Reinforcement).
        ostream& print(ostream&); // Prints what type of card it is. Inserts string "This is a reinforcement card." to ostream.
        ReinforcementCard& operator=(const ReinforcementCard&); // Assignment operator overload
        ~ReinforcementCard(); // Deconstructor

};

/*
* Bomb card class, extends Card. Allows player to halve troops in any enemy/neutral territory, rounding down.
*/

class BombCard: public Card {

    public: 
        BombCard(); // Default constructor
        BombCard(const BombCard&); // Copy constructor
        Card* copy(); // Returns new bomb card.
        Order* play(); // Plays the bomb card. Returns a new order corresponding to the card's type (Bomb).
        ostream& print(ostream&); // Prints what type of card it is. Inserts string "This is a bomb card." to ostream.
        BombCard& operator=(const BombCard&); // Assignment operator overload
        ~BombCard(); // Deconstructor

};

/*
* Diplomacy card class, extends Card. Allows player to prevent two players from attacking each other, starting the turn after the card is played.
*/

class DiplomacyCard: public Card {

    public: 
        DiplomacyCard(); // Default constructor
        DiplomacyCard(const DiplomacyCard&); // Copy constructor
        Card* copy(); // Returns new diplomacy card.
        Order* play(); // Plays the diplomacy card. Returns a new order corresponding to the card's type (Diplomacy).
        ostream& print(ostream&); // Prints what type of card it is. Inserts string "This is a diplomacy card." to ostream.
        DiplomacyCard& operator=(const DiplomacyCard&); // Assignment operator overload
        ~DiplomacyCard(); // Deconstructor

    };
