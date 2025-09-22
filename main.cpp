// main.cpp
#include "Game.h"
#include <random>

// forward-declared in Data.cpp:
std::map<DeckType, Deck> buildAllDecks(std::mt19937& rng);

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());

    auto decks = buildAllDecks(rng);
    Game game(std::move(decks), /*lifelines=*/3);
    game.run();
    return 0;
}
