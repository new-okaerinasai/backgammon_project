#pragma once
#include <vector>
#include <iostream>
#include "Strategy.h"
#include <fstream>
#include "opennn09\source\opennn.h"

class Referee {
public:
    std::vector<int> Board{ 15,0,0,0,0,0,0,0,0,0,0,0,-15,0,0,0,0,0,0,0,0,0,0,0 };
    bool isfirst_white = true;
    bool isfirst_black = true;
    std::pair<int, int> GetDices() {
        std::pair<int, int> Dices;
        Dices.first = rand() % 6 + 1;
        Dices.second = rand() % 6 + 1;
        return Dices;
    }

    typedef std::vector<std::pair<int, int>> TMove;

    std::vector<TMove> Analyze(std::vector<TMove>& result, bool iswhite, std::pair<int, int> Dices) {
        std::vector<TMove> new_result;
        std::vector<TMove> final_result;
        int max = 0;
        for (TMove j : result) {
            if (j.size() > max)
                max = j.size();
        }

        for (TMove j : result) {
            if (j.size() == max)
                final_result.push_back(j);
        }
        if (!iswhite)
            minus12(final_result);
        return final_result;
    }

    void Rotate(std::vector<int>& board) {
        auto it = board.begin();
        std::advance(it, 12);
        std::reverse(board.begin(), it);
        std::reverse(it, board.end());
        std::reverse(board.begin(), board.end());
    }

    void minus12(std::vector<TMove>& result) {
        for (int i = 0; i != result.size(); ++i) {
            for (int j = 0; j != result[i].size(); ++j) {
                if (result[i][j].first < 12) result[i][j].first += 12; else result[i][j].first -= 12;
                if (result[i][j].second != -1)
                    if (result[i][j].second < 12) result[i][j].second += 12; else result[i][j].second -= 12;
            }
        }
    }

    void DoubleDiceMoves(std::vector<int> dicespread, std::vector<TMove>& result, std::vector<int> board, TMove t, bool iswhite) {
        if (!dicespread.empty()) {
            int DiceValue = dicespread.back();
            int begin;
            if (iswhite) {
                if (dicespread.size() == 2 && dicespread[0] != dicespread[1] || dicespread.size() == 4) begin = 0; 
                else begin = 1;
                if (AllInBase(iswhite, board)) begin = 18;
                for (int i = begin; i < 24; ++i) {
                    int target = i + DiceValue;
                    if (board[i] > 0) {
                        if (canMove(i, target, board, iswhite)) {
                            t.push_back(std::make_pair(i, target));
                            result.push_back(t);
                            --board[i];
                            if (target < 24) ++board[target];
                            dicespread.pop_back();
                            DoubleDiceMoves(dicespread, result, board, t, iswhite);
                            dicespread.push_back(DiceValue);
                            t.pop_back();
                            ++board[i];
                            if (target < 24) --board[target];
                        }
                        if (AllInBase(iswhite, board) && canLeave(board, iswhite, i, DiceValue)) {
                            t.push_back(std::make_pair(i, -1));
                            result.push_back(t);
                            --board[i];
                            dicespread.pop_back();
                            DoubleDiceMoves(dicespread, result, board, t, iswhite);
                            dicespread.push_back(DiceValue);
                            t.pop_back();
                            ++board[i];
                        }
                    }
                }
            }
            else {
                Rotate(board);
                for (int i = 0; i != 24; ++i) {
                    board[i] = -board[i];
                }
                DoubleDiceMoves(dicespread, result, board, t, true);
            }
        }
    }

    bool canMove(int from, int to, std::vector<int> board, bool iswhite) {
        if (iswhite) {
            if (to <= 23) {
                if (board[to] < 0) return false;
            }
            if (to > 23) return false;
        }
        board[to]++;
        board[from]--;
        if (somethingBlocked(board, iswhite)) return false;
        return true;
    }

    bool somethingBlocked(std::vector<int> board, bool iswhite) {
        Rotate(board);
        for (int i = 0; i != 24; ++i) {
            board[i] = -board[i];
        }
        int i;
        int inarow = 0;
        for (i = 0; i != 24; ++i) {
            if (board[i] < 0) inarow++; else inarow = 0;
            if (inarow == 6) break;
        }
        if (i == 24) return false;
        for (int j = i; j != 24; ++j) {
            if (board[j] > 0) return false;
        }
        return true;
    }

    std::ofstream dataset;

    bool canLeave(std::vector<int>& board, bool iswhite, int from, int DiceValue) {
        int target = from + DiceValue;
        if (target == 24) return true;
        int pos = -1;
        for (int i = 18; i != from; ++i) {
            if (board[i] > 0) {
                pos = i;
                break;
            }
        }
        if (pos == -1 && target > 24) return true;
        return false;
    }

    std::vector<TMove> GetGoodMoves(std::pair<int, int> Dices, bool iswhite) {
        std::vector<TMove> result;
        if (Dices.first != Dices.second) {
            TMove t;
            if (iswhite)
                isfirst_white = false;
            else
                isfirst_black = false;
            std::vector<int> dicespread1(2);
            dicespread1[0] = Dices.first;
            dicespread1[1] = Dices.second;
            std::vector<int> dicespread2(2);
            dicespread2[0] = Dices.second;
            dicespread2[1] = Dices.first;
            DoubleDiceMoves(dicespread1, result, Board, t, iswhite);
            t.clear();
            DoubleDiceMoves(dicespread2, result, Board, t, iswhite);
        }
        else {
            TMove t;
            std::vector<int> dicespread;
            dicespread.push_back(Dices.first);
            dicespread.push_back(Dices.first);
            dicespread.push_back(Dices.first);
            dicespread.push_back(Dices.first);
            DoubleDiceMoves(dicespread, result, Board, t, iswhite);
        }
        return Analyze(result, iswhite, Dices);
    }

    void UpdateState(TMove Move) {
        for (int i = 0; i < Move.size(); ++i) {
            if (Board[Move[i].first] > 0) {
                --Board[Move[i].first];
                if (Move[i].second != -1) ++Board[Move[i].second];
            }
            else {
                ++Board[Move[i].first];
                if (Move[i].second != -1) --Board[Move[i].second];
            }
        }
    }

    bool noBlacks() {
        int sum = 0;
        for (int i = 0; i != 24; ++i) {
            if (Board[i] < 0) sum++;
        }
        return sum == 0;
    }

    bool noWhites() {
        int sum = 0;
        for (int i = 0; i != 24; ++i) {
            if (Board[i] > 0) sum++;
        }
        return sum == 0;
    }

    bool make_game(IStrategy& white, IStrategy& black) {
        while (!noWhites() && !noBlacks()) {
            std::pair<int, int> Dices = GetDices();
            //std::vector<int> before = white.GetFactors(Board);
            TMove Decision1 = white.Decide(GetGoodMoves(Dices, white.iswhite), Board);
            UpdateState(Decision1);
            if (noWhites()) return 0;
            Dices = GetDices();
            TMove Decision2 = black.DecideRandom(GetGoodMoves(Dices, black.iswhite));
            UpdateState(Decision2);
            if (noBlacks()) return 1;
            //std::vector<int> after = white.GetFactors(Board);
            //white.UpdateCoefficients(before, after);
        }
    }

    void Educate() {
        IStrategy player1, player2;
        player1.iswhite = true;
        player2.iswhite = false;
        
        for (int i = 0; i != 2000; ++i) {
            make_game(player1, player2);
            Board = { 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        }
        player1.W.save("matrix_coefs.dat");
        player1.W_2.save("lin_coefs.dat");
    }

    double make_100_games(IStrategy white, IStrategy black, const std::vector<int>& board/*, bool shouldWin*/) {
        white.iswhite = true;
        black.iswhite = false;
        int player1 = 0;
        int player2 = 0;
        srand(3228);
        Board = board;
        white.W.load("syn1.dat");
        white.W_1.load("syn2.dat");
        white.W_2.load("syn3.dat");
        for (int i = 0; i != 2000; ++i) {
            if (i % 100 == 0)
                std::cout << i << '\n';
            if (make_game(white, black)) player1++; else player2++;
            Board = board;
        }
        /*if (shouldWin) return (double)(player2) / (player2 + player1);
        else */return (double)(player1) / (player2 + player1);
    }

    /*void work() {
        std::ifstream dataset("dataset.txt");
        std::ofstream out("out.txt");
        IStrategy first, second;
        int order;
        std::vector<int> board(24), factors(60, 0);
        while (dataset) {
            dataset >> order;
            for (int i = 0; i != 24; ++i) {
                dataset >> board[i];
                if (board[i] > 0) factors[i] = board[i]; else factors[i + 24] = -board[i];
            }

            if (order == 0) {
                out << make_100_games(second, first, board, false) << ' ';
            }
            else {
                out << make_100_games(first, second, board, true) << ' ';
            }

            if (order == 1) {
                int k4 = 0, k2 = 0, k3 = 0, k5 = 0, k6 = 0;
                for (int i = 0; i != 24; ++i) {
                    if (board[i] > 0) {
                        ++k4;
                        ++k3;
                        ++k2;
                        ++k5;
                        ++k6;
                    }
                    if (k4 == 4) { ++factors[50]; k4 = 0; }
                    if (k2 == 2) { ++factors[48]; k2 = 0; }
                    if (k3 == 3) { ++factors[49]; k3 = 0; }
                    if (k5 == 5) { ++factors[51]; k5 = 0; }
                    if (k6 == 6) { ++factors[52]; k6 = 0; }
                }
            }
            else {
                int k4 = 0, k2 = 0, k3 = 0, k5 = 0, k6 = 0;
                for (int i = 0; i != 24; ++i) {
                    if (board[i] < 0) {
                        ++k4;
                        ++k3;
                        ++k2;
                        ++k5;
                        ++k6;
                    }
                    if (k4 == 4) { ++factors[50]; k4 = 0; }
                    if (k2 == 2) { ++factors[48]; k2 = 0; }
                    if (k3 == 3) { ++factors[49]; k3 = 0; }
                    if (k5 == 5) { ++factors[51]; k5 = 0; }
                    if (k6 == 6) { ++factors[52]; k6 = 0; }
                }
            }

            int friendN;

            if (order == 1) {
                for (int i = 0; i != 24; ++i) {
                    if (board[i] > 0) friendN += board[i];
                }
                factors[53] = 15 - friendN;
                friendN = 0;
            }
            else {
                for (int i = 0; i != 24; ++i) {
                    if (board[i] < 0) friendN += -board[i];
                }
                factors[53] = 15 - friendN;
                friendN = 0;
            }

            
            int minFriend;
            if (order == 1) {
                for (int i = 0; i != 24; ++i) {
                    if (board[i] > 0) {
                        minFriend = i;
                        break;
                        factors[54] = minFriend;
                    }
                }
            }
            else {
                for (int i = 0; i != 24; ++i) {
                    if (board[i] < 0) {
                        minFriend = i;
                        break;
                        factors[54] = minFriend;
                    }
                }
            }

            for (auto factor : factors) {
                out << factor << ' ';
            }

            out << '\n';
            factors = std::vector<int>(60, 0);
        }
    }*/

    bool AllInBase(bool iswhite, std::vector<int> board) {
        int sum = 0;
        for (int i = 0; i != 18; ++i) {
            if (board[i] > 0) ++sum;
        }
        return sum == 0;
    }

    void PrintBoard() {
        for (int i = 0; i < 24; ++i) {
            dataset << Board[i] << ' ';
        }
        dataset << '\n';
    }
};
