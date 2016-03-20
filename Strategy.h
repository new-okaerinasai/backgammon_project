#pragma once
#include <vector>
#include <iostream>


namespace ruslan1 {
    class IStrategy {
    public:
        typedef std::vector<std::pair<int, int>> TMove;

        TMove Decide(std::vector<TMove> GoodMoves) {
            if (!GoodMoves.empty())
                return GoodMoves[0];
        }
        bool iswhite;
    private:

    };
}