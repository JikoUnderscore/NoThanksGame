#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <array>
#include <functional>
#include <format>

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/ranges.h".

#define let const auto
#define not !

#define Input std::cin

using std::vector;
using std::array;
using std::string;

constexpr auto CARDS = 33;	// 3 - 35;
constexpr auto TOKENS = 55;

//	Winner is the pearson win the lowest points
//	points = cards sum - tokesns


struct Player {
    int32_t tokens;
    vector<int32_t> cards;
};

struct Card {
    int32_t tokens;
    int32_t value;
};


vector<Card> make_deck() {
    vector<Card> deck;

    for (size_t i = 3; i < CARDS + 3; i++) {
        deck.emplace_back(0, i);
    }

    for (size_t i = 0; i < 9; i++) {
        let n = rand() % deck.size();
        std::swap(deck[n], deck.back());
        deck.pop_back();
    }
    assert(deck.size() == 24);

    std::random_device rd;
    std::default_random_engine gen(rd());

    std::ranges::shuffle(deck, gen);

    return deck;
}


int main() {
    auto deck = make_deck();

    constexpr int32_t tokents_per_player = 11;
    array players{
        Player{.tokens = tokents_per_player},
        Player{.tokens = tokents_per_player},
        Player{.tokens = tokents_per_player},
        Player{.tokens = tokents_per_player}
    };

    while (true) { // deck_done
        for (size_t i = 0; i < players.size(); i++) {
            string input;

            fmt::print("----------------------\n\n");

            while (true) { // exit_draw
                fmt::print("Plater {} with tokens: {} and cards: {} \n\n", i, players[i].tokens, players[i].cards);

                if (deck.empty()) {
                    goto deck_done;
                }
                auto& card = deck.back();

                fmt::print("Drawn card: {} with tokens {} \n\n", card.value, card.tokens);
                fmt::print("[T] Take card, [P] pass card\n");
                Input >> input;

                switch (input[0]) {
                    case 'T':
                    case 't':
                    {
                        deck.pop_back();
                        players[i].cards.push_back(card.value);
                        players[i].tokens += card.tokens;
                    } break;
                    case 'P':
                    case 'p':
                    {
                        players[i].tokens -= 1;
                        card.tokens += 1;
                        goto exit_draw;
                    }// break; no beak needed goto exits the loop 
                    default:
                        break;
                }

            }
            exit_draw:;
            system("CLS");
        }
    }
    deck_done:;



    fmt::print("END\n");
    int32_t winner{ INT32_MAX };
    int winner_index{ 0 };

    for (size_t i = 0; i < players.size(); i++) {
        fmt::print("Player {} with tokens: {} and cards: ", i, players[i].tokens);
        std::ranges::sort(players[i].cards);
        fmt::print("{} \n", players[i].cards);

        int32_t score{ 0 };
        int32_t last_card{ -1 };

        for (let card_value : players[i].cards) {
            if (last_card + 1 == card_value) {
            } else {
                score += card_value;
            }
            last_card = card_value;
        }

        let p_socre{ score - players[i].tokens };

        if (p_socre < winner) {
            winner = p_socre;
            winner_index = i;
        } 

        fmt::print("SCORE {}\n", p_socre);
    }

    fmt::print("\nwinner index {} with score {}\n", winner_index, winner);
}

