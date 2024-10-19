#include "gtest/gtest.h"
#include "GameEngine.h"

TEST(GameEngineTest, computeNext) {
    Grid grid1 = Grid(10,10);
    Grid grid2 = Grid(10,10);
    grid1.setElement(4, 4);
    grid1.setElement(5,4);
    grid1.setElement(3,4);
    GameEngine gameEngine = GameEngine({3}, {2,3});
    gameEngine.computeNext(grid1,grid2);
    EXPECT_EQ(grid2.getElement(4,4), true);
    EXPECT_EQ(grid2.getElement(4,3), true);
    EXPECT_EQ(grid2.getElement(4,5), true);
}

TEST(GameEngineTest, computeIterations1) {
    Grid grid1 = Grid(10,10);
    Grid grid2 = Grid(10,10);
    grid1.setElement(4, 4);
    grid1.setElement(5,4);
    grid1.setElement(4,5);
    grid1.setElement(5,5);
    GameEngine gameEngine = GameEngine({3}, {2,3});
    gameEngine.computeIterations(grid1,grid2, 1000);
    EXPECT_EQ(grid2.getElement(4,4), true);
    EXPECT_EQ(grid2.getElement(4,5), true);
    EXPECT_EQ(grid2.getElement(5,4), true);
    EXPECT_EQ(grid2.getElement(5,5), true);
}

TEST(GameEngineTest, computeIterations2) {
    Grid grid1 = Grid(10,10);
    Grid grid2 = Grid(10,10);
    grid1.setElement(4, 4);
    grid1.setElement(5,4);
    grid1.setElement(3,4);
    GameEngine gameEngine = GameEngine({3}, {2,3});
    gameEngine.computeIterations(grid1,grid2, 2);
    EXPECT_EQ(grid2.getElement(4,4), true);
    EXPECT_EQ(grid2.getElement(5,4), true);
    EXPECT_EQ(grid2.getElement(3,4), true);
}

TEST(GameEngineTest, toroidalUniverseTest) {
    Grid grid1 = Grid(7,7);
    Grid grid2 = Grid(7,7);
    grid1.setElement(6, 6);
    grid1.setElement(5,6);
    grid1.setElement(4,6);
    GameEngine gameEngine = GameEngine({3}, {2,3});
    gameEngine.computeIterations(grid1,grid2);
    EXPECT_EQ(grid2.getElement(5,6), true);
    EXPECT_EQ(grid2.getElement(5,5), true);
    EXPECT_EQ(grid2.getElement(5,0), true);
}
