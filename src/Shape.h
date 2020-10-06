#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <string>
#include <utility>
#include "utils.h"

extern const int maxn;
extern const int maxm;

namespace tetrix
{
  using std::cout;
  using std::pair;
  using std::string;
  using std::to_string;
  using Point = std::pair<int, int>;
  using utils::debug;
  using utils::error;
  using utils::info;
  using utils::warning;

  class Shape
  {
  private:
  public:
    Point ref;
    Point block[4];
    Shape(string item_id, int row, int col) : ref(row, col)
    {
      if (item_id == "T1")
      {
        block[0] = std::make_pair(0, 1);
        block[1] = std::make_pair(1, 0);
        block[2] = std::make_pair(1, 1);
        block[3] = std::make_pair(1, 2);
        info("'T1' item created");
      }
      else if (item_id == "I1")
      {
        block[0] = std::make_pair(0, 0);
        block[1] = std::make_pair(1, 0);
        block[2] = std::make_pair(2, 0);
        block[3] = std::make_pair(3, 0);
        info("'I1' item created");
      }
      else if (item_id == "I2")
      {
        block[0] = std::make_pair(0, 0);
        block[1] = std::make_pair(0, 1);
        block[2] = std::make_pair(0, 2);
        block[3] = std::make_pair(0, 3);
        info("'I2' item created");
      }
      else if (item_id == "O")
      {
        block[0] = std::make_pair(0, 0);
        block[1] = std::make_pair(0, 1);
        block[2] = std::make_pair(1, 0);
        block[3] = std::make_pair(1, 1);
        info("'O' item created");
      }
      else
      {
        string str = "Unknown item_id '" + item_id + "'";
        throw str;
      }
    };
    string getRefStr()
    {
      string str = "(" + to_string(this->ref.first) + ", " + to_string(this->ref.second) + ")";
      return str;
    }
  };
} // namespace tetrix

#endif