#pragma once
#include <vector>
#include <iostream>
#include <fstream>

typedef std::vector<std::pair<int, int>> TMove;
std::vector<double> coefs{ 1.0 ,
-1.0228723239e-16 ,
-3.95113347137e-16 ,
-1.95951153519e-16 ,
-8.21015815398e-16 ,
-7.52363659614e-16 ,
-4.97542541319e-16 ,
-2.51653640675e-16 ,
-2.88465785949e-16 ,
-2.61101009317e-16 ,
-1.3538833312e-16 ,
-4.45033860234e-16 ,
-4.28159486521e-16 ,
-8.00384120701e-16 ,
-3.71546651198e-16 ,
-4.3706475665e-16 ,
-5.28393008886e-16 ,
-6.22677446683e-16 ,
-7.43917006252e-16 ,
-6.36432217213e-16 ,
-2.56965397621e-16 ,
-1.6522688181e-16 ,
-8.86531008108e-17 ,
-6.11345158612e-17 ,
1.26049591249e-17 ,
-4.90312728859e-16 ,
-1.89691972255e-16 ,
-2.57674333234e-16 ,
-3.65621247639e-16 ,
-2.55391126908e-16 ,
-4.32322308371e-16 ,
-3.60219550871e-16 ,
-1.20172264338e-16 ,
4.20397982614e-17 ,
-1.07548314308e-16 ,
-1.25916385404e-16 ,
-2.68410383525e-17 ,
9.14227850853e-17 ,
-2.33681337017e-16 ,
-5.51160135162e-16 ,
-6.68210030808e-16 ,
-5.92248339343e-16 ,
-2.66901495475e-16 ,
-1.10280897873e-16 ,
-6.91295267943e-17 ,
9.9833472131e-17 ,
5.38467273391e-17 ,
-2.36504493063e-16 ,
-3.93136381364e-16 ,
5.98744651328e-16 ,
1.0061796466e-15 ,
1.45529061545e-16 ,
9.71829656949e-16 ,
-1.48534322157e-15 ,
6.11060677994e-17 ,
0.0 ,
0.0 ,
0.0 ,
0.0 ,
0.0 ,
0.0 };
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

    TMove Decide(std::vector<TMove> GoodMoves, std::vector<int>& board) {
        int result;
        int max = INT_MIN;
        for (int i = 0; i != GoodMoves.size(); ++i) {
            std::vector<int> new_board = UpdateState(board, GoodMoves[i]);
            auto c = scalar_product(GetFactors(new_board));
            if (c > max) {
                result = i;
                max = c;
            }
        }
        if (GoodMoves.size() > 0)
            return GoodMoves[result];
    }

    void UpdateCoefficients(std::vector<int>& first, std::vector<int>& second) {
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
            if (I_norm != 0) {
                coefs[i] /= I_norm;
            }
        }
    }

    TMove DecideRandom(std::vector<TMove> GoodMoves) {
        if (GoodMoves.size() > 0) {
            int ind = rand() % GoodMoves.size();
            return GoodMoves[ind];
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