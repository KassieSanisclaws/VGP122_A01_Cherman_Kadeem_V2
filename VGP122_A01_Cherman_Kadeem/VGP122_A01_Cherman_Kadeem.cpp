// VGP122_A01_Cherman_Kadeem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

enum class Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

class Card {
public:
    Rank rank;

    Card(Rank r) : rank(r) {}
};

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck() {
        for (int i = 0; i < 6; ++i) {
            for (int j = 2; j <= 14; ++j) {
                cards.push_back(Card(static_cast<Rank>(j)));
            }
        }
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card dealCard() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};

class Player {
private:
    std::vector<Card> hand;
    int totalPoints;

public:
    Player() : totalPoints(0) {}

    void addToHand(Card card) {
        hand.push_back(card);
        totalPoints += getCardValue(card);
    }

    int getTotalPoints() const {
        return totalPoints;
    }

    bool hasBlackjack() const {
        return (hand.size() == 2 && totalPoints == 21);
    }

    bool isBusted() const {
        return (totalPoints > 21);
    }

    int getCardValue(const Card& card) const {
        int value = static_cast<int>(card.rank);
        return (value <= 10) ? value : 10; // Face cards are worth 10
    }

    void clearHand() {
        hand.clear();
        totalPoints = 0;
    }

    void displayHand(bool showAllCards) const {
        std::cout << "Player's Hand: ";
        for (const auto& card : hand) {
            if (showAllCards || &card == &hand.front()) {
                std::cout << getCardValue(card) << " ";
            }
            else {
                std::cout << "X "; // Hide face-down card
            }
        }
        std::cout << std::endl;
    }
};

class BlackjackGame {
private:
    Player player;
    Player dealer;
    Deck deck;
    int credits;

public:
    BlackjackGame(int initialCredits) : credits(initialCredits) {}

    void play() {
        while (credits > 0) {
            std::cout << "Credits: " << credits << std::endl;
            int bet = getBet();

            deck.shuffle();
            initializeHands();

            // Player's turn
            playerTurn();

            // Dealer's turn
            dealerTurn();

            // Determine winner and update credits
            int result = determineWinner();
            updateCredits(result, bet);

            // Ask to play again
            if (!playAgain()) break;
        }

        std::cout << "Game over. Thanks for playing!" << std::endl;
    }

private:
    int getBet() {
        int bet;
        do {
            std::cout << "Place your bet (1-" << credits << "): ";
            std::cin >> bet;
        } while (bet < 1 || bet > credits);
        return bet;
    }

    void initializeHands() {
        player.clearHand();
        dealer.clearHand();

        player.addToHand(deck.dealCard());
        player.addToHand(deck.dealCard());

        dealer.addToHand(deck.dealCard());
        dealer.addToHand(deck.dealCard());
    }

    void playerTurn() {
        while (true) {
            player.displayHand(false);

            char choice;
            do {
                std::cout << "Choose (H)it, (S)tand, (D)ouble Down, (P)ass: ";
                std::cin >> choice;
            } while (choice != 'H' && choice != 'S' && choice != 'D' && choice != 'P');

            if (choice == 'H') {
                player.addToHand(deck.dealCard());
                if (player.isBusted()) {
                    std::cout << "Busted! You lose!" << std::endl;
                    return;
                }
            }
            else if (choice == 'D') {
                // Implement double down
            }
            else if (choice == 'P') {
                // Implement split
            }
            else { // Stand or Pass
                break;
            }
        }
    }

    void dealerTurn() {
        dealer.displayHand(true);

        while (dealer.getTotalPoints() < 17) {
            dealer.addToHand(deck.dealCard());
            dealer.displayHand(true);
        }
    }

    int determineWinner() {
        if (player.hasBlackjack() && !dealer.hasBlackjack()) {
            std::cout << "Blackjack! You win!" << std::endl;
            return 1;
        }
        else if (!player.hasBlackjack() && dealer.hasBlackjack()) {
            std::cout << "Dealer has Blackjack. You lose!" << std::endl;
            return -1;
        }
        else if (player.isBusted()) {
            std::cout << "Busted! You lose!" << std::endl;
            return -1;
        }
        else if (dealer.isBusted() || player.getTotalPoints() > dealer.getTotalPoints()) {
            std::cout << "You win!" << std::endl;
            return 1;
        }
        else if (player.getTotalPoints() < dealer.getTotalPoints()) {
            std::cout << "You lose!" << std::endl;
            return -1;
        }
        else {
            std::cout << "It's a tie!" << std::endl;
            return 0;
        }
    }

    void updateCredits(int result, int bet) {
        if (result == 1) {
            credits += bet;
        }
        else if (result == -1) {
            credits -= bet;
        }
    }

    bool playAgain() {
        char choice;
        do {
            std::cout << "Play again? (Y/N): ";
            std::cin >> choice;
        } while (choice != 'Y' && choice != 'N');

        return (choice == 'Y');
    }
};

int main() {
    BlackjackGame game(1000);
    game.play();
    return 0;
}
