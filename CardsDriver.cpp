// Written by Adam Blevins, 40255384
#pragma once
#include "Cards.h"
#include <iostream>
#include "Orders.h"

using namespace std;

int main() {
    
    // The following are constants for how many of each type of card should be added to the deck. Default is currently 2 for each type, meaning 10 cards total in the deck.
    const int NUM_OF_BOMB_CARDS = 2;
    const int NUM_OF_DIPLOMACY_CARDS = 2;
    const int NUM_OF_AIRLIFT_CARDS = 2;
    const int NUM_OF_BLOCKADE_CARDS = 2;
    const int NUM_OF_REINFORCEMENT_CARDS = 2;

    // Creates pointer called deck to a new Deck object. This serves as the deck for the game. 
    Deck* deck = new Deck();

    // Creates default number of bomb cards and adds them to the deck
    for(int i = 0; i < NUM_OF_BOMB_CARDS; i++){
        deck->addCard(new BombCard());
    }

    // Creates default nubmer of diplomacy cards and adds them to the deck
    for(int i = 0; i < NUM_OF_DIPLOMACY_CARDS; i++){
        deck->addCard(new DiplomacyCard());
    }

    // Creates default nubmer of airlift cards and adds them to the deck
    for(int i = 0; i < NUM_OF_AIRLIFT_CARDS; i++){
        deck->addCard(new AirliftCard());
    }

    // Creates default nubmer of blockade cards and adds them to the deck
    for(int i = 0; i < NUM_OF_BLOCKADE_CARDS; i++){
        deck->addCard(new BlockadeCard());
    }

    // Creates default nubmer of reinforcement cards and adds them to the deck
    for(int i = 0; i < NUM_OF_REINFORCEMENT_CARDS; i++){
        deck->addCard(new ReinforcementCard());
    }

    // Creates pointer to a hand that will be later assigned to a player.
    Hand* hand1 = new Hand();

    cout << "Starting setup:" << endl;

    hand1->print(cout);
    cout << endl;
    
    deck->print(cout);
    cout << endl;

    // Draws all cards to the created hand.
    while(deck->getSize() > 0){
        deck->drawToHand(hand1);
    }

    cout << "After adding all cards to one hand:" << endl;
    hand1->print(cout);
    cout << endl;
    
    deck->print(cout);
    cout << endl;

    // Plays each card in the hand.
    while(hand1->getSize() > 0){
        hand1->playCard(0, deck);
    }

    cout << "After playing all cards in the hand:" << endl;

    hand1->print(cout);
    cout << endl;
    
    deck->print(cout);
    cout << endl;
};



