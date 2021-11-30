//#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "level.h"
#include "fstream"
#include <ctime>
#include "params.h"


int main() {
  // constexpr std::size_t kFramesPerSecond{60};
  // constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  // constexpr std::size_t {640};
  // constexpr std::size_t kScreenHeight{640};
  // constexpr std::size_t kGridWidth{32};
  // constexpr std::size_t kGridHeight{32};
  
  //generete map
  std::ofstream newMap("/home/workspace/CppND-CAPSTONE-MY_GAME/src/level2.txt");
  srand(time(NULL));
  int zeroOrOne;
  std::string line;
  if(newMap.is_open())
  {
    for(int i =0;i<32;i++)
    {
      line="";
      for(int j =0;j<32;j++)
      {
        zeroOrOne=rand()%2;
        if((rand()%10)!=0)
        zeroOrOne = 0;
        line.append(std::to_string(zeroOrOne));
        line.append(",");
      }
      newMap<<line<<std::endl;
    }
    newMap.close();
  }
  else
  {
    std::cout<<"Unable to create new map";
    return -1;
  }
  Level level(1);

  Renderer renderer(level);
  Controller controller;
  Game game(level);
  game.Run(controller, renderer);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}