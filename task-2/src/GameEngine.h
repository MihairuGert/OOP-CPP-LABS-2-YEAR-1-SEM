#pragma once
#include "Grid.h"

class GameEngine {
private:
    int countNeighbors(Grid& grid, int x, int y);
    std::vector<int> birthCondition;
    std::vector<int> survivalCondition;
    bool isSurvived(int num);
    bool isBorn(int num);
public:
    GameEngine(const std::vector<int>& birthCondition, const std::vector<int>& survivalCondition);
    void computeNext(Grid& grid1, Grid& grid2);
    void computeIterations(Grid& grid1, Grid& grid2, int iterations = 1);
};