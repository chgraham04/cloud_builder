#include "Game.h"

#include "Game.h"
#include <iostream>

Game::Game(std::map<DeckType, Deck> decks, int lifelines)
    : m_decks(std::move(decks)), m_lifelines(lifelines) {}

void Game::run() {
    std::cout << "=== Cloud Builder ===\n";
    for (int r = 1; r <= 3; ++r) {
        RoundId roundId = static_cast<RoundId>(r);
        std::cout << "\n-- Round " << r << "--\n";
        RoundResult rr = playRound(roundId);
        std::cout << "Round " << r << " result: CS=" << rr.cloudStrength
                    << " vs RES=" << rr.resistance << "\n";

        if (rr.cloudStrength <= rr.resistance) {
            if (!resolveRound(roundId, rr)) {
                std::cout << "Your cloud dissipates... Game Over. \n";
                return;
            }
        }
        else {
            std::cout << "You advance to the next level of the troposphere! \n";
        }
    }
    std::cout << "\nYou have reached the top of the troposphere! You WIN!\n";
}

RoundResult Game::playRound(RoundId roundId) {
    RoundResult r;
    r.resistance += m_nextRoundResistanceBonus;
    m_nextRoundResistanceBonus = 0;

    std::map<DeckType, Card> drawn;
    drawPhase(roundId, r, drawn);
    printDrawn(drawn, roundId, r);
    return r;
}

void Game::drawPhase(RoundId roundId, RoundResult &r, std::map<DeckType, Card> &drawn) {
    const int idx = static_cast<int>(roundId) - 1;

    auto drawFrom = [&](DeckType dt) {
        auto& deck = m_decks.at(dt);
        auto c = deck.draw();
        if (!c) {
            return;
        }
        drawn[dt] = *c;
        applyCard(*c, roundId, r);
    };
    // R1: all 4 decks. R2: skip Nuclei. R3: skip Nuclei & Moisture.
    if (roundId == RoundId::R1) {
        drawFrom(DeckType::Nuclei);
        drawFrom(DeckType::Temperature);
        drawFrom(DeckType::Moisture);
        drawFrom(DeckType::Event);
    } else if (roundId == RoundId::R2) {
        drawFrom(DeckType::Temperature);
        drawFrom(DeckType::Moisture);
        drawFrom(DeckType::Event);
    } else {
        drawFrom(DeckType::Temperature);
        drawFrom(DeckType::Event);
    }
}

void Game::applyCard(const Card& c, RoundId roundId, RoundResult& r) {
    size_t idx = static_cast<size_t>(static_cast<int>(roundId) - 1);
    r.cloudStrength += c.csDelta(idx);
    r.resistance    += c.resDelta(idx);
}

bool Game::resolveRound(RoundId roundId, RoundResult& r) {
    // Must achieve CS > RES to proceed. If CS == RES, can lifeline OR (if R1/R2) carry +1 RES to next round and proceed.
    while (r.cloudStrength < r.resistance) {
        std::cout << "CS < RES. You must use a lifeline to redraw a card.\n";
        if (m_lifelines <= 0) return false;
        // Recreate "drawn" map only for decks active this round (to choose which to redraw):
        std::map<DeckType, Card> dummy; // In a fuller impl, track actual drawn cards per round.
        if (!tryUseLifeline(roundId, r, dummy)) return false;
    }
    if (r.cloudStrength == r.resistance) {
        if (roundId == RoundId::R3) {
            std::cout << "Tie in R3 not allowed—must use lifelines to get CS > RES.\n";
            while (r.cloudStrength <= r.resistance) {
                if (m_lifelines <= 0) return false;
                std::map<DeckType, Card> dummy;
                if (!tryUseLifeline(roundId, r, dummy)) return false;
            }
            return true;
        }
        // Offer choice: lifeline(s) now or carry +1 RES to next round
        std::cout << "CS == RES. Options:\n"
                  << "  1) Use a lifeline to redraw until CS > RES\n"
                  << "  2) Carry +1 Resistance to the next round and continue\n";
        int choice = promptInt("Choose (1-2): ", 1, 2);
        if (choice == 2) {
            m_nextRoundResistanceBonus += 1;
            return true;
        }
        // else use lifelines until strictly greater
        while (r.cloudStrength <= r.resistance) {
            if (m_lifelines <= 0) return false;
            std::map<DeckType, Card> dummy;
            if (!tryUseLifeline(roundId, r, dummy)) return false;
        }
    }
    return true;
}

bool Game::tryUseLifeline(RoundId roundId, RoundResult& r, std::map<DeckType, Card>& /*drawn*/) {
    std::cout << "Lifelines remaining: " << m_lifelines << "\n";
    // Choose a deck (limited by the round’s available decks)
    DeckType choice = promptDeckChoiceForLifeline(/*drawn*/{}, roundId);
    auto& deck = m_decks.at(choice);

    // Simple redraw policy: “bottom insert” last hypothetical card and draw a fresh one
    auto newCard = deck.draw();
    if (!newCard) {
        std::cout << "That deck is empty. Choose another.\n";
        return true; // let user try again without spending lifeline
    }

    // Apply ONLY the delta difference: assume we “remove” an old card and “add” new one.
    // For simplicity, we’ll just add the new one (design choice). More exact modeling can
    // track previously drawn cards and reverse their deltas.
    applyCard(*newCard, roundId, r);

    --m_lifelines;
    std::cout << "Redrew [" << newCard->name << "]. New CS=" << r.cloudStrength
              << ", RES=" << r.resistance << "\n";
    return true;
}

int Game::promptInt(const std::string& q, int lo, int hi) {
    int v;
    for (;;) {
        std::cout << q;
        if (std::cin >> v && v >= lo && v <= hi) return v;
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "Invalid input.\n";
    }
}

DeckType Game::promptDeckChoiceForLifeline(const std::map<DeckType, Card>&, RoundId roundId) {
    std::vector<std::pair<int, DeckType>> options;
    int i = 1;
    auto add = [&](DeckType dt, const char* label){
        options.push_back({i, dt});
        std::cout << "  " << i++ << ") " << label << "\n";
    };

    std::cout << "Choose a deck to redraw:\n";
    if (roundId == RoundId::R1) add(DeckType::Nuclei, "Condensation Nuclei");
    if (roundId != RoundId::R3) add(DeckType::Moisture, "Moisture");
    add(DeckType::Temperature, "Temperature");
    add(DeckType::Event, "Event");

    int c = promptInt("Pick: ", 1, static_cast<int>(options.size()));
    return options[c-1].second;
}

void Game::printDrawn(const std::map<DeckType, Card>& drawn, RoundId roundId, const RoundResult& r) const {
    std::cout << "Round draws applied. CS=" << r.cloudStrength << ", RES=" << r.resistance << "\n";
}