#pragma once
#include "Card.h"
#include <vector>
#include <random>
#include <optional>

class Deck {
public:
    Deck(DeckType type, std::vector<Card> cards);

    void shuffle(std::mt19937 &rng);

    std::optional<Card> draw();

    void bottomInsert(const Card& c);

    DeckType type() const { return m_type; }
    bool empty() const;

private:
    DeckType m_type;
    std::vector<Card> m_cards;
    size_t m_top = 0; // index of next; usse ring buffer style
};