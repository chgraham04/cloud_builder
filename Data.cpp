// Data.cpp
#include "Deck.h"
#include <random>
#include <vector>
#include <map>

// --- helper: make 4 copies of every card in a list ---
static std::vector<Card> fourOfEach(const std::vector<Card>& base) {
    std::vector<Card> out;
    out.reserve(base.size() * 4);
    for (const auto& c : base) {
        for (int i = 0; i < 4; ++i) out.push_back(c);
    }
    return out;
}

// quick constructor for cards
static Card C(const char* name, DeckType d,
              int cs1,int res1, int cs2,int res2, int cs3,int res3) {
    Card c;
    c.name = name;
    c.deck = d;
    c.effect.cloudStrengthDelta = {cs1, cs2, cs3};
    c.effect.resistanceDelta    = {res1, res2, res3};
    return c;
}

std::map<DeckType, Deck> buildAllDecks(std::mt19937& rng) {
    // Base (unique) card definitions per deck
    std::vector<Card> nuclei = {
        C("Sea Salt",      DeckType::Nuclei,     +1,0,   0,0,   0,0),
        C("Dust",          DeckType::Nuclei,      0,0,   0,0,   0,0),
        C("Volcanic Ash",  DeckType::Nuclei,     +1,0,   0,0,   0,+1),
        C("Soot",          DeckType::Nuclei,      0,+1,  0,+1, -1,0),
    };

    std::vector<Card> temperature = {
        C("Warm Air Above",       DeckType::Temperature, 0,+1, 0,+1, 0,+1),
        C("Temperature Inversion",DeckType::Temperature, 0,+2, 0,+2, 0,+2),
        C("Surface Heating",      DeckType::Temperature, +1,0, +1,0, +1,0),
        C("Cool Air Above",       DeckType::Temperature, +1,0, +1,0, +1,0),
        C("Extreme Surface Heat", DeckType::Temperature, +2,0, +2,0, +2,0),
    };

    std::vector<Card> moisture = {
        C("Mixed Humidity",      DeckType::Moisture, 0,0,  0,0,  0,0),
        C("Dry Air Approaches",  DeckType::Moisture, 0,+1, 0,+1, 0,+1),
        C("Low Humidity",        DeckType::Moisture, 0,+2, 0,+2, 0,+2),
        C("Dew Point Reached",   DeckType::Moisture, +1,0, +1,0, +1,0),
        C("High Humidity",       DeckType::Moisture, +2,0, +2,0, +2,0),
    };

    std::vector<Card> event = {
        C("Orographic Lifting", DeckType::Event, +1,0, +1,0, +1,0),
        C("Cold Front",         DeckType::Event, +2,0, +2,0, +2,0),
        C("Heavy Rainfall",     DeckType::Event, 0,+2,0,+2,0,+2),
        C("Light Drizzle",      DeckType::Event, 0,+1,0,+1,0,+1),
        C("Lightning",          DeckType::Event, 0,0,  0,0,  0,0),
    };

    // Expand to 4-of-each before creating Decks
    std::map<DeckType, Deck> decks{
        {DeckType::Nuclei,      Deck(DeckType::Nuclei,      fourOfEach(nuclei))},
        {DeckType::Temperature, Deck(DeckType::Temperature, fourOfEach(temperature))},
        {DeckType::Moisture,    Deck(DeckType::Moisture,    fourOfEach(moisture))},
        {DeckType::Event,       Deck(DeckType::Event,       fourOfEach(event))},
    };

    // Shuffle all decks
    for (auto& [_, d] : decks) d.shuffle(rng);
    return decks;
}
