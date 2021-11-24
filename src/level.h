#pragma once
#include <algorithm> // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <SDL.h>

class Level
{
public:
    Level(int);
    enum class State
    {
        kEmpty,
        kObstacle,
        kStart
    };
    //void Level::RenderLevel(const std::vector<std::vector<State>>);
    std::vector<std::vector<State>> ReadBoardFile();
    void addObstacleToLevel(SDL_Rect&);
    bool obsCollWithHead(SDL_Rect&);


private:
    int n;
    std::vector<SDL_Rect> obstacles;
    std::vector<State> ParseLine(std::string line);
};