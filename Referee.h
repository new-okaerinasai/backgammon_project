#pragma once
#include <vector>
#include <iostream>
#include "Strategy.h"

namespace ruslan {
    class Referee {
    public:
        std::vector<int> Board{ 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        bool isfirst = true;
        std::pair<int, int> GetDices() {
            srand(6);
            std::pair<int, int> Dices;
            Dices.first = rand() % 6 + 1;
            Dices.second = rand() % 6 + 1;
            return Dices;
        }

        typedef std::vector<std::pair<int, int>> TMove;

        std::vector<TMove> Analyze(std::vector<TMove>& result, bool iswhite) {
            std::vector<TMove> new_result;
            for (auto j = result.begin(); j != result.end(); ++j) {
                int k = 0;
                bool flag = true;
                for (auto i = (*j).begin(); i != (*j).end(); ++i) {
                    if (iswhite) {
                        if (isWhiteHead((*i).first)) {
                            ++k;
                        }
                    }
                    else {
                        if (isBlackHead((*i).second)) {
                            ++k;
                        }
                    }
                    if (!isfirst) {
                        if (k > 1)
                            flag = false;
                    }
                    else {
                        if (k > 2)
                            flag = false;
                    }
                }
                if (flag) {
                    new_result.push_back(*j);
                }
            }
            return new_result;
        }

        void DoubleDiceMoves(std::vector<int> dicespread, std::vector<TMove>& result, std::vector<int> board, TMove t, bool iswhite) {
            if (!dicespread.empty()) {
                int DiceValue = dicespread[dicespread.size() - 1];
                if (iswhite) {
                    for (int i = 0; i < 18; ++i) {
                        if (board[i] > 0) {
                            if (board[i + dicespread[dicespread.size() - 1]] >= 0) {
                                t.push_back(std::make_pair(i, i + dicespread[0]));
                                --board[i];
                                ++board[i + dicespread[dicespread.size() - 1]];
                                dicespread.pop_back();
                                DoubleDiceMoves(dicespread, result, board, t, iswhite);
                                dicespread.push_back(DiceValue);
                                t.pop_back();
                                ++board[i];
                                --board[i + dicespread[dicespread.size() - 1]];
                            }
                        }
                    }
                }
                else {
                    for (size_t i = 12; i < 30; ++i) {
                        if (board[i % 24] < 0) {
                            if (board[(i + dicespread[dicespread.size() - 1]) % 24] <= 0) {
                                t.push_back(std::make_pair(i % 24, (i + dicespread[dicespread.size() - 1]) % 24));
                                ++board[i % 24];
                                --board[(i + dicespread[dicespread.size() - 1]) % 24];
                                dicespread.pop_back();
                                DoubleDiceMoves(dicespread, result, board, t, iswhite);
                                t.pop_back();
                                dicespread.push_back(DiceValue);
                                --board[i % 24];
                                ++board[(i + dicespread[dicespread.size() - 1]) % 24];
                            }
                        }
                    }
                }
            }
            else
                result.push_back(t);
        }

        std::vector<TMove> GetGoodMoves(std::pair<int, int> Dices, bool iswhite) {
            std::vector<TMove> result;
            if (Dices.first != Dices.second) {
                TMove t;
                isfirst = false;
                std::vector<int> dicespread1(2);
                dicespread1[0] = Dices.first;
                dicespread1[1] = Dices.second;
                std::vector<int> dicespread2(2);
                dicespread2[0] = Dices.second;
                dicespread2[1] = Dices.first;
                if (iswhite) {
                    for (size_t i = 0; i < 18; ++i) {
                        std::vector<int> board_copy(Board);
                        TMove t;
                        if (!isWhiteHead(i) && board_copy[i] > 0) {
                            if (board_copy[i + dicespread1[0]] >= 0) {
                                t.push_back(std::make_pair(i, i + dicespread1[0]));
                                board_copy[i]--;
                                board_copy[i + dicespread1[0]]++;
                                for (size_t j = 0; j < 18; ++j) {
                                    if (!isWhiteHead(j) && board_copy[j] > 0) {
                                        if (board_copy[j + dicespread1[1]] >= 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j, j + dicespread1[1]));
                                            result.push_back(t1);
                                        }
                                    }
                                    if (isWhiteHead(j) && board_copy[j] > 0) {
                                        if (board_copy[j + dicespread1[1]] >= 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j, j + dicespread1[1]));
                                            result.push_back(t1);
                                        }
                                    }
                                }
                            }
                        }
                        if (isWhiteHead(i) && board_copy[i] > 0) {
                            if (board_copy[i + dicespread1[0]] >= 0) {
                                t.push_back(std::make_pair(i, i + dicespread1[0]));
                                board_copy[i]--;
                                board_copy[i + dicespread1[0]]++;
                                for (size_t j = 1; j < 18; ++j) {
                                    if (board_copy[j + dicespread1[1]] >= 0 && board_copy[j] > 0) {
                                        TMove t1(t);
                                        t1.push_back(std::make_pair(j, j + dicespread1[1]));
                                        result.push_back(t1);
                                    }
                                }
                            }
                        }
                    }
                }
                else {
                    for (size_t i = 12; i < 30; ++i) {
                        std::vector<int> board_copy(Board);
                        TMove t;
                        if (!isBlackHead(i % 24) && board_copy[i % 24] < 0) {
                            if (board_copy[(i + dicespread1[0]) % 24] <= 0) {
                                t.push_back(std::make_pair(i % 24, (i + dicespread1[0]) % 24));
                                board_copy[i % 24]++;
                                board_copy[(i + dicespread1[0]) % 24]--;
                                for (size_t j = 12; j < 30; ++j) {
                                    if (!isBlackHead(j % 24) && board_copy[j % 24] < 0) {
                                        if (board_copy[(j + dicespread1[1]) % 24] >= 0 && board_copy[j % 24] < 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j % 24, (j + dicespread1[1]) % 24));
                                            result.push_back(t1);
                                        }
                                    }
                                    if (isBlackHead(j % 24) && board_copy[j % 24] < 0) {
                                        if (board_copy[(j + dicespread1[1]) % 24] <= 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j, j + dicespread1[1]));
                                            result.push_back(t1);
                                        }
                                    }
                                }
                            }
                        }
                        
                        if (isBlackHead(i % 24) && board_copy[i % 24] < 0) {
                            if (board_copy[(i + dicespread1[0]) % 24] <= 0) {
                                t.push_back(std::make_pair(i % 24, (i + dicespread1[0]) % 24));
                                board_copy[i % 24]++;
                                board_copy[(i + dicespread1[0])]--;
                                for (size_t j = 13; j < 30; ++j) {
                                    if (board_copy[(j + dicespread1[1]) % 24] <= 0 && board_copy[j % 24] < 0) {
                                        TMove t1(t);
                                        t1.push_back(std::make_pair(j % 24, (j + dicespread1[1]) % 24));
                                        result.push_back(t1);
                                    }
                                }
                            }
                        }
                    }
                }
                if (iswhite) {
                    iswhite = false;
                    for (size_t i = 0; i < 18; ++i) {
                        std::vector<int> board_copy(Board);
                        TMove t;
                        if (!isWhiteHead(i) && board_copy[i] > 0) {
                            if (board_copy[i + dicespread2[0]] >= 0) {
                                t.push_back(std::make_pair(i, i + dicespread2[0]));
                                board_copy[i]--;
                                board_copy[i + dicespread2[0]]++;
                                for (size_t j = 0; j < 18; ++j) {
                                    if (!isWhiteHead(j) && board_copy[j] > 0) {
                                        if (board_copy[j + dicespread2[1]] >= 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j, j + dicespread2[1]));
                                            result.push_back(t1);
                                        }
                                    }
                                    if (isWhiteHead(j) && board_copy[j % 24] > 0) {
                                        if (board_copy[j + dicespread2[1]] >= 0 && board_copy[j] > 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j, j + dicespread2[1]));
                                            result.push_back(t1);
                                        }
                                    }
                                }
                            }
                        }
                        if (isWhiteHead(i) && board_copy[i] > 0) {
                            if (board_copy[i + dicespread2[0]] >= 0) {
                                t.push_back(std::make_pair(i, i + dicespread2[0]));
                                board_copy[i]--;
                                board_copy[i + dicespread2[0]]++;
                                for (size_t j = 1; j < 18; ++j) {
                                    if (board_copy[j + dicespread2[1]] >= 0 && board_copy[j] > 0) {
                                        TMove t1(t);
                                        t1.push_back(std::make_pair(j, j + dicespread2[1]));
                                        result.push_back(t1);
                                    }
                                }
                            }
                        }
                    }
                }
                else {
                    iswhite = true;
                    for (size_t i = 12; i < 30; ++i) {
                        std::vector<int> board_copy(Board);
                        TMove t;
                        if (!isBlackHead(i % 24) && board_copy[i % 24] < 0) {
                            if (board_copy[(i + dicespread2[0]) % 24] <= 0) {
                                t.push_back(std::make_pair(i % 24, (i + dicespread2[0]) % 24));
                                board_copy[i % 24]++;
                                board_copy[(i + dicespread2[0]) % 24]--;
                                for (size_t j = 12; j < 30; ++j) {
                                    if (!isBlackHead(j % 24) && board_copy[j % 24] < 0) {
                                        if (board_copy[(j + dicespread2[1]) % 24] <= 0 && board_copy[j % 24] < 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j % 24, (j + dicespread2[1]) % 24));
                                            result.push_back(t1);
                                        }
                                    }
                                    if (isBlackHead(j % 24) && board_copy[j % 24] < 0) {
                                        if (board_copy[(j + dicespread2[1]) % 24] <= 0) {
                                            TMove t1(t);
                                            t1.push_back(std::make_pair(j, j + dicespread2[1]));
                                            result.push_back(t1);
                                        }
                                    }
                                }
                            }
                        }
                        if (isBlackHead(i % 24) && board_copy[i % 24] < 0) {
                            if (board_copy[(i + dicespread2[0]) % 24] <= 0) {
                                t.push_back(std::make_pair(i % 24, (i + dicespread2[0]) % 24));
                                board_copy[i % 24]++;
                                board_copy[(i + dicespread2[0])]--;
                                for (size_t j = 13; j < 30; ++j) {
                                    if (board_copy[(j + dicespread2[1]) % 24] <= 0 && board_copy[j % 24] < 0) {
                                        TMove t1(t);
                                        t1.push_back(std::make_pair(j % 24, (j + dicespread2[1]) % 24));
                                        result.push_back(t1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else {
                std::vector<int> dicespread;
                dicespread.push_back(Dices.first);
                dicespread.push_back(Dices.first);
                dicespread.push_back(Dices.first);
                dicespread.push_back(Dices.first);
                TMove t;
                DoubleDiceMoves(dicespread, result, Board, t, iswhite);
            }
            return Analyze(result, iswhite);
        }

        void UpdateState(TMove Move) {
            for (int i = 0; i < Move.size(); ++i) {
                if (Board[Move[i].first] > 0) {
                    --Board[Move[i].first];
                    ++Board[Move[i].second];
                }
                else {
                    ++Board[Move[i].first];
                    --Board[Move[i].second];
                }
            }
        }

        void make_game(ruslan1::IStrategy white, ruslan1::IStrategy black) {
            white.iswhite = true;
            black.iswhite = false;
            while (!AllInBase(white) || !AllInBase(black)) {
                std::pair<int, int> Dices = GetDices();
                TMove Decision1 = white.Decide(GetGoodMoves(Dices, white.iswhite));
                UpdateState(white.Decide(GetGoodMoves(Dices, white.iswhite)));
                std::cout << "Dices for player1: " << Dices.first << ' ' << Dices.second << ' ';
                std::cout << "And he decided to Move ";
                for (auto i : Decision1) {
                    std::cout << '|' << i.first << "->" << i.second << '|';
                }
                std::cout << '\n';

                PrintBoard();
                TMove Decision2 = white.Decide(GetGoodMoves(Dices, black.iswhite));

                UpdateState(black.Decide(GetGoodMoves(Dices, black.iswhite)));
                std::cout << "Dices for player2: " << Dices.first << ' ' << Dices.second << ' ';
                std::cout << "And he decided to Move ";
                for (auto i : Decision2) {
                    std::cout << '|' << i.first << "->" << i.second << '|';
                }
                std::cout << '\n';
                PrintBoard();
                std::cout << '\n';
                isfirst = false;
            }
            while (AllInBase(white) && !AllInBase(black)) {

            }
        }

        bool AllInBase(ruslan1::IStrategy player) {
            int sum = 0;
            if (player.iswhite) {
                for (int i = 18; i < 24; ++i) {
                    sum += Board[i];
                }
                return (sum == 15);
            }
            else {
                for (int i = 6; i < 12; ++i) {
                    std::cout << Board[i];
                    sum += Board[i];
                }
                return (sum == -15);
            }
        }

        void PrintBoard() {
            for (int i = 0; i < 24; ++i) {
                std::cout << Board[i] << ' ';
            }
            std::cout << '\n';
        }
    private:
        
        bool isWhiteHead(int i) {
            return (i == 0);
        }

        bool isBlackHead(int i) {
            return (i == 12);
        }
    };
}
