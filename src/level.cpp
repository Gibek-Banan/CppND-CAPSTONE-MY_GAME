#include "level.h"
#include <vector>
#include "collision.h"

Level::Level(int n = 1) : n(n)
{
}
std::vector<Level::State> Level::ParseLine(std::string line)
{
    std::istringstream sline(line);
    int n;
    char c;
    std::vector<State> row;
    while (sline >> n >> c && c == ',')
    {
        if (n == 0)
        {
            row.push_back(State::kEmpty);
        }
        if (n == 1)
        {
            row.push_back(State::kObstacle);
        }
    }
    return row;
}
std::vector<std::vector<Level::State>> Level::ReadBoardFile()
{
    //std::string path = "level" + std::to_string(n) + ".txt";
    std::string path = "/home/workspace/CppND-Capstone-Snake-Game/src/level" + std::to_string(n) + ".txt";

    std::ifstream myfile(path);
    std::vector<std::vector<State>> board{};
    if (myfile)
    {
        std::string line;
        while (getline(myfile, line))
        {
            std::vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    else
    {
        std::cout << "File hasn't been loaded" << std::endl;
    }

    return board;
}
void Level::addObstacleToLevel(SDL_Rect& obs)
{
    obstacles.emplace_back(obs);
}

bool Level::obsCollWithHead(SDL_Rect& head)
{
    for(const auto& obst : obstacles)
    {
        if(Collision::check_collision(obst,head))
        {
            return true;
        }
    }
    return false;
}
// void Level::RenderLevel(const std::vector<std::vector<State>> board)
// {
// }