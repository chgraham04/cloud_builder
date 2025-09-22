#ifndef GAME_H
#define GAME_H

#pragma once
#include "Deck.h"
#include <map>

struct RoundResult {
    int cloudStrength = 0;
    int resistance = 0;
};

class Game {
public:
    Game(std::map<DeckType, Deck> decks, int lifelines = 3);

    void run();

private:
    RoundResult playRound(RoundId roundId);
    void drawPhase(RoundId roundId, RoundResult &r, std::map<DeckType, Card> &drawn);
    void applyCard(const Card &c, RoundId roundId, RoundResult &r);

    // lifelines
    bool resolveRound(RoundId roundId, RoundResult &r);
    bool canCarryTieToNext(RoundId roundId) const;
    bool tryUseLifeline(RoundId roundId, RoundResult &r, std::map<DeckType, Card> &drawn);

    // CLI helper funcs
    int promptInt(const std::string &q, int lo, int hi);
    DeckType promptDeckChoiceForLifeline(const std::map<DeckType, Card> &drawn, RoundId roundId);
    void printDrawn(const std::map<DeckType, Card> &drawn, RoundId roundId, const RoundResult &r) const;

private:
    std::map<DeckType, Deck> m_decks;
    int m_lifelines;
    int m_nextRoundResistanceBonus = 0;
};

#endif //GAME_H
