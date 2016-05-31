#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "opennn09\source\opennn.h"

typedef std::vector<std::pair<int, int>> TMove;
OpenNN::Vector<double> kek;

std::vector<double> coefs(60,1);

using namespace OpenNN;

class IStrategy {
public:
    typedef std::vector<std::pair<int, int>> TMove;
    double scalar_product(std::vector<int>& factors) {
        double result = 0;
        for (int i = 0; i != factors.size(); ++i) {
            result += factors[i] * coefs[i];
        }
        return result;
    }
    
    std::vector<int> UpdateState(std::vector<int>& board, TMove Move) {
        std::vector<int> new_board(board);
        for (int i = 0; i < Move.size(); ++i) {
            if (new_board[Move[i].first] > 0) {
                --new_board[Move[i].first];
                if (Move[i].second != -1) ++new_board[Move[i].second];
            }
            else {
                ++new_board[Move[i].first];
                if (Move[i].second != -1) --new_board[Move[i].second];
            }
        }
        return new_board;
    }

    Matrix<double> W_1;
    TMove Decide(std::vector<TMove> GoodMoves, std::vector<int>& board) {
        int result;
        int min = INT_MAX;
        
        for (int i = 0; i != GoodMoves.size(); ++i) {
            std::vector<int> new_board = UpdateState(board, GoodMoves[i]);
            auto fact = GetFactors(new_board);

            Vector<double> input_1(fact.size());

            for (int i = 0; i != input_1.size(); ++i) {
                input_1[i] = fact[i];
            }

            auto a1 = W.dot(input_1);

            for (int i = 0; i != a1.size(); ++i) {
                a1[i] = sigmoid(a1[i]);
            }

            auto a2 = W_1.dot(a1);

            
            for (int i = 0; i != a2.size(); ++i) {
                a2[i] = sigmoid(a2[i]);
            }
            double c = W_2.dot(a2);
            if (c < min) {
                result = i;
                min = c;
            }
        }
        if (GoodMoves.size() > 0)
            return GoodMoves[result];
    }

    /*void UpdateCoefficients(std::vector<int>& first, std::vector<int>& second) {
        float c = 0, I_norm = 0;
        for (int i = 0; i != first.size(); ++i) {
            c += coefs[i] * (first[i] - second[i]);
            I_norm += (coefs[i] * (first[i] - second[i])) * (coefs[i] * (first[i] - second[i]));
        }
        I_norm = sqrt(I_norm);
        for (int i = 0; i != coefs.size(); ++i) {
            int sign = 1;
            if (c < 0) sign = -1;
            coefs[i] -= 0.01 * sign * (first[i] - second[i]);
        }
    }*/
    
    
    Matrix<double> W = Matrix<double>(60, 30, 0.1);
    Vector<double> W_2 = Vector<double>(30, 0.1);

    double y_1, y_2;

    double sigmoid(double x) {
        return (1 / (1 + exp(-x)));
    }

    void UpdateCoefficients(std::vector<int>& first, std::vector<int>& second) {
        std::ofstream first_v("first_v.txt");
        std::ofstream second_v("second_v.txt");

        for (int i = 0; i != first.size(); ++i) {
            first_v << first[i] << ' ';
            second_v << second[i] << ' ';
        }
        double y, y_;
        first_v.close();
        second_v.close();

        Vector<double> input_1("first_v.txt");
        Vector<double> input_2("second_v.txt");
        input_1 /= input_1.calculate_norm();
        input_2 /= input_2.calculate_norm();

        Vector<double> a1, a2;

        a1 = W.dot(input_1);
        a2 = W.dot(input_2);

        for (int i = 0; i != a1.size(); ++i) {
            a1[i] = sigmoid(a1[i]);
            a2[i] = sigmoid(a2[i]);
        }

        y_ = W_2.dot(a1);
        y = W_2.dot(a2);

        for (int i = 0; i != 60; ++i) {
            for (int j = 0; j != 30; ++j) {
                W[i][j] += 0.01 * (a1[j])*(1 - a1[j])*(y_ - y)*first[i];
            }
        }
        for (int i = 0; i != 30; ++i) {
            W_2[i] += 0.01 * (y_ - y) * (a1[i] - a2[i]);
        }
    }

    TMove DecideRandom(std::vector<TMove>& GoodMoves) {
        if (GoodMoves.size() > 0) {
            return GoodMoves[rand() % GoodMoves.size()];
        }
    }

    std::vector<int> GetFactors(std::vector<int>& board) {
        std::vector<int> factors(60, 0);
        for (int i = 0; i != 24; ++i) {
            if (board[i] > 0) factors[i] = board[i]; else factors[i + 24] = -board[i];
        }

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
            int friendN = 0;
            for (int i = 0; i != 24; ++i) {
                if (board[i] > 0) friendN += board[i];
            }
            factors[53] = 15 - friendN;
            friendN = 0;
        }


        int minFriend;
        for (int i = 0; i != 24; ++i) {
            if (board[i] > 0) {
                minFriend = i;
                factors[54] = minFriend;
                break;
            }
        }
        return factors;
    }

    bool iswhite;
private:

};