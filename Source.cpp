#include <algorithm>
#include <iostream>
#include <vector>
#include "Referee.h"
#include "Strategy.h"

int main() {
    ruslan::Referee Referee1;
    ruslan1::IStrategy player1, player2;
    Referee1.make_game(player1, player2);
    for (auto i : Referee1.GetGoodMoves(std::make_pair(5, 6), true)) {
        std::cout << '|';
        for (auto j : i)
            std::cout << j.first << ' ' << j.second << ' ';
        std::cout << '|' << '\n';
    }
}
