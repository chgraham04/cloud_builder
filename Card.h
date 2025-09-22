#pragma once
#include <string>
#include <array>
#include <optional>
#include <cstdint>

enum class DeckType { Nuclei, Temperature, Moisture, Event };
enum class RoundId : uint8_t { R1 = 1, R2 = 2, R3 = 3 };

struct CardEffect {
    // index from 0, 1, and 2 to rounds 1, 2, and 3
    std::array<int, 3> cloudStrengthDelta{ 0, 0, 0 };
    std::array<int, 3> resistanceDelta{ 0, 0, 0 };
};

struct Card {
    std::string name;
    DeckType deck;
    CardEffect effect;

    // helper funcs to get deltas for given round index (0,1,2):
    int csDelta(size_t rIndex) const { return effect.cloudStrengthDelta[rIndex]; }
    int resDelta(size_t rIndex) const { return effect.resistanceDelta[rIndex]; }
};