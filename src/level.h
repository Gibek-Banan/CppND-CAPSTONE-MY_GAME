#pragma once
#include <algorithm> // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <SDL.h>
#include "params.h"

class Level
{
public:
    Level(int);
    enum class State
    {
        kEmpty,
        kObstacle,
        kClosed,
        kPath,
        kStart,
        kFinish
    };
    void updatePath(int initX, int initY, int goalX, int goalY);
    void PrintBoard();
    bool obsCollWithHead(SDL_Rect &);


    std::vector<std::vector<Level::State>> board;
    std::vector<SDL_Rect> obstacles;
    std::vector<SDL_Rect> path;
    SDL_Rect finish;

private:
    int n;
    const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    void addObstaclesToVector();
    std::vector<std::vector<State>> ReadBoardFile();
    std::vector<State> ParseLine(std::string line);
    bool static Compare(const std::vector<int> a, const std::vector<int> b);
    void CellSort(std::vector<std::vector<int>> *v);
    int Heuristic(int x1, int y1, int x2, int y2);
    bool CheckValidCell(int x, int y);
    void AddToOpen(int x, int y, int g, int h, std::vector<std::vector<int>> &openlist);
    void ExpandNeighbors(const std::vector<int> &current, int goal[2], std::vector<std::vector<int>> &openlist);
    void Search(int initX, int initY, int goalX, int goalY);
    void addPathToVector();
    std::string CellString(State cell);
};