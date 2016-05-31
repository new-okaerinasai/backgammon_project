#include <algorithm>
#include <iostream>
#include <vector>
#include "Referee.h"
#include "Strategy.h"
#include <ctime>
#include <fstream>
#include <string>
#include "opennn09\source\opennn.h"

int main() {
    Referee Referee1;
    std::vector<int> Board{ 6,0,0,0,2,-1,1,0,0,0,0,-3,-8,0,0,1,1,-1,-1,-1,1,2,1,0 };
    IStrategy player1, player2;
    clock_t timer = clock();
    //std::cout << Referee1.make_100_games(player1, player2, std::vector<int> { 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    //Referee1.Educate();
    player1.iswhite = false;
    Referee1.Visualize(player1);
}
