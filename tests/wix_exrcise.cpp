// g++ prog.cpp -o prog -Wall -std=c++17

/*
  - We have a NxN grid
  - Turtle initial point is bottom left (1,1) looking north
  - Turtle movement is one of: Forward one step (F), Turn left (L), Turn right (R). 
  - Grid has 4 directions: E/W/S/N.
            N        
     _______________
    |   |   |   |   |
    |___|___|___|___|
    |   |   |   |   |
    |___|___|___|___|
 W  |   |   |   |   | E
    |___|___|___|___|
    | ^ |   |   |   |
    |_O_|___|___|___|
    
           S

  - Implement a function that receives the board size and the movement string and returns the final point and direction.


  Examples:

  input: board size is 5, movement string: "FFFRRFLF"
  output: (2,3,E)

  input: board size is 10, movement string: "FLLFLL"
  output: (1,1,N)
 */
#include <iostream>
#include <utility>
#include <string>
#include <map>

 
using namespace std;

class Matrix {
private:
  int boardDim;
  std::pair<int, int> coordinates = {1,1} ;
  char currentDir = 'N';
  std::map<std::pair<char, char>, char> direction = {
                                                        {std::make_pair('N', 'F'), 'N'}, 
                                                        {std::make_pair('N', 'R'), 'E'},
                                                        {std::make_pair('N', 'L'), 'W'},
                                                        {std::make_pair('S', 'F'), 'S'},
                                                        {std::make_pair('S', 'R'), 'W'},
                                                        {std::make_pair('S', 'L'), 'E'},
                                                        {std::make_pair('W', 'F'), 'W'}, 
                                                        {std::make_pair('W', 'R'), 'N'},
                                                        {std::make_pair('W', 'L'), 'S'},
                                                        {std::make_pair('E', 'F'), 'E'},
                                                        {std::make_pair('E', 'R'), 'S'},
                                                        {std::make_pair('E', 'L'), 'N'}
                                                    };
  std::map<char, std::pair<int, int>> advance = {
                                                    {'N', std::make_pair(0, 1)},
                                                    {'S', std::make_pair(0, -1)},
                                                    {'E', std::make_pair(1, 0)},
                                                    {'W', std::make_pair(-1, 0)}
                                                };

public:
  Matrix(const int n) : boardDim(n) {}

  void cmd(const char x) {
        currentDir = direction.at(std::make_pair(currentDir, x));
        if (x == 'F') {
            std::pair<int, int> tmp = advance.at(currentDir);
            coordinates.first = std::max(std::min(coordinates.first + tmp.first, boardDim), 1);
            coordinates.second = std::max(std::min(coordinates.second + tmp.second, boardDim), 1);
        }
  }

  void display() {
      std::cout << "(" << coordinates.first << "," << coordinates.second << "," << currentDir << ")\n";
  }

};

// To execute C++, please define "int main()"
int main() {
  
  Matrix mat(4);
  std::string test ; // = "FFFRFFRFFRF"; 
  std::getline(std::cin, test);
  for(auto &c : test) {
     mat.cmd(c);
  }
  mat.display();
  return 0;
}