#pragma once
#include <vector>
#include <iostream>
#include "Strategy.h"

class Referee {
public:
    std::vector<int> Board{ 15,0,0,0,0,0,/* */0,0,0,0,0,0,/* */-15,0,0,0,0,0,/* */0,0,0,0,0,0 };
    bool isfirst_white = true;
    bool isfirst_black = true;
    std::pair<int, int> GetDices() {
        std::pair<int, int> Dices;
        Dices.first = rand() % 6 + 1;
        Dices.second = rand() % 6 + 1;
        return Dices;
    }

    typedef std::vector<std::pair<int, int>> TMove;

    bool badDices(std::pair<int, int> Dices) {
        if (Dices.first == Dices.second && (Dices.first == 6 || Dices.first == 3 || Dices.first == 4))
            return true;
        else
            return false;
    }

    std::vector<TMove> Analyze(std::vector<TMove>& result, bool iswhite, std::pair<int, int> Dices) {
        std::vector<TMove> new_result;
        std::vector<TMove> final_result;
        int max = 0;
        for (TMove j : result) {
            int k = 0;
            bool flag = true;
            bool isfirst;
            if (iswhite) isfirst = isfirst_white;
            else isfirst = isfirst_black;
            for (auto i : j) {
                if (iswhite) {
                    if (isWhiteHead(i.first)) {
                        ++k;
                    }
                }
                else {
                    if (isBlackHead(i.first)) {
                        ++k;
                    }
                }
                if (k > 1) {
                    if (isfirst && badDices(Dices) && k == 2) {
                        flag = true;
                    }
                    else
                        flag = false;
                }
            }
            if (flag) {
                new_result.push_back(j);
            }
            
        }
        for (TMove j : new_result) {
            if (j.size() > max)
                max = j.size();
        }

        for (TMove j : new_result) {
            if (j.size() == max)
                final_result.push_back(j);
        }
        return final_result;
    }

    void Rotate(std::vector<int>& board) {
        std::vector<int> new_board(board);
        std::reverse(new_board.begin(), new_board.begin() + 13);
        std::reverse(new_board.begin() + 12, new_board.end());
        std::reverse(new_board.begin(), new_board.end());
    }

    std::vector<TMove> minus12(std::vector<TMove>& result) {
        for (TMove move : result) {
            for (std::pair<int, int> fromto : move) {
                fromto.first -= 12;
            }
        }
    }

    void DoubleDiceMoves(std::vector<int> dicespread, std::vector<TMove>& result, std::vector<int> board, TMove t, bool iswhite) {
        if (!dicespread.empty()) {
            int DiceValue = dicespread[dicespread.size() - 1];
            if (iswhite) {
                for (int i = 0; i < 24; ++i) {
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
                    }
                }
            }
            else {
                Rotate(board);
                DoubleDiceMoves(dicespread, result, board, t, true);
            }
            if (!iswhite)
                minus12(result);
        }
    }

    bool canMove(int from, int to, std::vector<int> board, bool iswhite) {
        if (iswhite) {
            if (to <= 23) {
                if (board[to] < 0) return false;
                board[to]++;
                int k = 0;
                for (int i = 0; i != 24; ++i) {
                    if (board[i] > 0) {
                        ++k;
                    }
                    else {
                        k = 0;
                    }
                    if (k == 6 && noBlacksFurther(i, board)) return false;
                }
                return true;
            }
            else {
                if (!AllInBase(iswhite)) {
                    return false;
                }
                else {
                    if (to == 24 || noWhite(from, board)) {
                        return true;
                    }
                    else return false;
                }
            }
        }
        else {
            if (to <= 35) {
                if (board[to % 24] > 0) return false;
                board[to % 24]--;
                int k = 0;
                for (int i = 0; i != 24; ++i) {
                    if (board[i] < 0) {
                        ++k;
                    }
                    else {
                        k = 0;
                    }
                    if (k == 6 && noWhitesFurther(i, board)) return false;
                }
                return true;
            }
            else {
                if (to == 36 || noBlack(from, board))
                    return true;
                else
                    return false;
            }
        }
    }

    bool noWhite(int from, std::vector<int> board) {
        for (int i = 18; i != from; ++i) {
            if (board[i] > 0) return false;
        }
        return true;
    }

    bool noBlack(int from, std::vector<int> board) {
        for (int i = 6; i != from % 24; ++i) {
            if (board[i] < 0) return false;
        }
        return true;
    }

    bool noBlacksFurther(int i, std::vector<int> board) {
        for (int k = i; i != 36; ++i) {
            if (board[i % 24] < 0)
                return false;
        }
        return true;
    }

    bool noWhitesFurther(int i, std::vector<int> board) {
        for (int k = i; i != 24; ++i) {
            if (board[i] > 0)
                return false;
        }
        return true;
    }

    int sumWhite() {
        int sum = 0;
        for (int i : Board) {
            if (i > 0)
                sum += i;
        }
        return sum;
    }
    int sumBlack() {
        int sum = 0;
        for (int i : Board) {
            if (i < 0)
                sum += i;
        }
        return sum;
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
                if (Move[i].second < 24) ++Board[Move[i].second];
            }
            else {
                ++Board[Move[i].first];
                if (Move[i].second < 24) --Board[Move[i].second];
            }
        }
    }

    void make_game(IStrategy white, IStrategy black) {
        white.iswhite = true;
        black.iswhite = false;
        while (sumWhite() != 0 || sumBlack() != 0) {
            std::pair<int, int> Dices = GetDices();
            TMove Decision1 = white.Decide(GetGoodMoves(Dices, white.iswhite));
            UpdateState(white.Decide(GetGoodMoves(Dices, white.iswhite)));
            std::cout << "Dices for player1: " << Dices.first << ' ' << Dices.second << ' ';
            std::cout << "And he decided to Move ";
            for (auto i : Decision1) {
                std::cout << '|' << i.first << "->" << i.second << '|';
            }
            std::cout << '\n';
            isfirst_white = false;
            PrintBoard();
            Dices = GetDices();
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
            isfirst_black = false;
        }
    }

    bool AllInBase(bool iswhite) {
        int sum = 0;
        if (iswhite) {
            for (int i = 18; i < 24; ++i) {
                if (Board[i] > 0)
                    sum += Board[i];
            }
            return (sum == 15);
        }
        else {
            for (int i = 6; i < 12; ++i) {
                if (Board[i] < 0)
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

// TO DO: дописать фигню
