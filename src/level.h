#include <algorithm> // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

private:
    int n;

    std::vector<State> ParseLine(std::string line);
};