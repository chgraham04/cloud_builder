#include "Deck.h"
#include <algorithm>

Deck::Deck(DeckType type, std::vector<Card> cards)
: m_type(type), m_cards(std::move(cards)) {}

void Deck::shuffle(std::mt19937& rng) {
    std::shuffle(m_cards.begin(), m_cards.end(), rng);
    m_top = 0;
}
std::optional<Card> Deck::draw() {
    if (empty()) return std::nullopt;
    return m_cards[m_top++];
}
void Deck::bottomInsert(const Card& c) {
    m_cards.push_back(c);
}
bool Deck::empty() const {
    return m_cards.empty() || m_top >= m_cards.size();
}
