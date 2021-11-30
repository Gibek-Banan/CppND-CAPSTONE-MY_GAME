#include "level.h"
#include <vector>
#include "collision.h"
#include <algorithm>
using std::abs;
using std::cout;
using std::sort;
using std::string;
using std::vector;

Level::Level(int n = 1) : n(n)
{
    board = ReadBoardFile();
    addObstaclesToVector();
}
void Level::addObstaclesToVector()
{
    SDL_Rect block;
    block.w = Params::kScreenWidth / Params::kGridWidth;
    block.h = Params::kScreenHeight / Params::kGridHeight;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            if (board[i][j] == State::kObstacle)
            {
                block.x = j * block.w;
                block.y = i * block.h;
                obstacles.emplace_back(block);
            }
        }
    }
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
            row.emplace_back(State::kEmpty);
        }
        if (n == 1)
        {
            row.emplace_back(State::kObstacle);
        }
    }
    return row;
}
std::vector<std::vector<Level::State>> Level::ReadBoardFile()
{
    //std::string path = "level" + std::to_string(n) + ".txt";
    std::string path = "/home/workspace/CppND-CAPSTONE-MY_GAME/src/level" + std::to_string(n) + ".txt";

    std::ifstream myfile(path);
    std::vector<std::vector<State>> board{};
    if (myfile)
    {
        std::string line;
        while (getline(myfile, line))
        {
            std::vector<State> row = ParseLine(line);
            board.emplace_back(row);
        }
    }
    else
    {
        std::cout << "Cannot read file from path: " << path << std::endl;
    }

    return board;
}

bool Level::Compare(const vector<int> a, const vector<int> b)
{
    int f1 = a[2] + a[3]; // f1 = g1 + h1
    int f2 = b[2] + b[3]; // f2 = g2 + h2
    return f1 > f2;
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void Level::CellSort(vector<vector<int>> *v)
{
    sort(v->begin(), v->end(), Level::Compare);
}

// Calculate the manhattan distance
int Level::Heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x2 - x1) + abs(y2 - y1);
}

/**
 * Check that a cell is valid: on the board, not an obstacle, and clear.
 */
bool Level::CheckValidCell(int x, int y)
{
    bool on_grid_x = (x >= 0 && x < board.size());
    bool on_grid_y = (y >= 0 && y < board[0].size());
    if (on_grid_x && on_grid_y)
        return board[x][y] == State::kEmpty;
    return false;
}

/**
 * Add a node to the open list and mark it as open.
 */
void Level::AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist)
{
    // Add node to open vector, and mark board cell as closed.
    openlist.emplace_back(vector<int>{x, y, g, h});
    board[x][y] = State::kClosed;
}

/**
 * Expand current nodes's neighbors and add them to the open list.
 */
void Level::ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist)
{
    // Get current node's data.
    int x = current[0];
    int y = current[1];
    int g = current[2];

    // Loop through current node's potential neighbors.
    for (int i = 0; i < 4; i++)
    {
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];

        // Check that the potential neighbor's x2 and y2 values are on the board and not closed.
        if (CheckValidCell(x2, y2))
        {
            // Increment g value and add neighbor to open list.
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist);
        }
    }
}

bool Level::obsCollWithHead(SDL_Rect &head)
{
    for (auto &obst : obstacles)
    {
        if (Collision::check_collision(obst, head))
        {
            return true;
        }
    }
    return false;
}

void Level::updatePath(int initX, int initY, int goalX, int goalY)
{
    Search(initX, initY, goalX, goalY);
    addPathToVector();
}

void Level::addPathToVector()
{
    SDL_Rect block;
    block.w = Params::kScreenWidth / Params::kGridWidth;
    block.h = Params::kScreenHeight / Params::kGridHeight;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            if (board[i][j] == State::kPath || board[i][j] == State::kFinish)
            {
                block.x = j * block.w;
                block.y = i * block.h;
                path.emplace_back(block);
            }
            if (board[i][j] == State::kFinish)
            {
                block.x = j * block.w;
                block.y = i * block.h;
                finish=block;
            }
        }
    }
}

void Level::Search(int initX, int initY, int goalX, int goalY)
{
    board = ReadBoardFile();
    int init[2] = {initX, initY};
    int goal[2] = {goalX, goalY};
    // Create the vector of open nodes.
    vector<vector<int>> open{};

    // Initialize the starting node.
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0], goal[1]);
    AddToOpen(x, y, g, h, open);

    while (open.size() > 0)
    {
        // Get the next node
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        board[x][y] = State::kPath;

        // Check if we're done.
        if (x == goal[0] && y == goal[1])
        {
            board[init[0]][init[1]] = State::kStart;
            board[goal[0]][goal[1]] = State::kFinish;
            return;
        }
        // If we're not done, expand search to current node's neighbors.
        ExpandNeighbors(current, goal, open);
    }
    std::cout << "No path found!" << std::endl;
    board = ReadBoardFile();
}

string Level::CellString(Level::State cell)
{
    switch (cell)
    {
    case State::kObstacle:
        return "B   ";
    case State::kPath:
        return "P   ";
    case State::kStart:
        return "S   ";
    case State::kFinish:
        return "F   ";
    default:
        return "0   ";
    }
}

void Level::PrintBoard()
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
    cout << "-----------------------------------------------------------------------------------------------------------------" << std::endl;
}