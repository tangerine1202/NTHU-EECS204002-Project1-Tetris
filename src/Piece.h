#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <string>
#include <utility>
#include "Utils.h"
#include "Type.h"

extern const int maxn;
extern const int maxm;

namespace tetris
{
  using std::cout;
  using std::pair;
  using std::string;
  using std::to_string;
  using utils::debug;
  using utils::error;
  using utils::info;
  using utils::warning;

  class Piece
  {
  private:
    DataType preprocess(const string bin_in_string)
    {
      DataType res(bin_in_string);
      res <<= (maxm - 4);
      return res;
    }

  public:
    Point ref;
    DataType block[4];
    Piece(const string item_id, const int row, const int col) : ref(row, col)
    {
      if (item_id == "T1")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("1110");
        block[0] = preprocess("0100");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "T2")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0100");
        block[1] = preprocess("1100");
        block[0] = preprocess("0100");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "T3")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("0100");
        block[0] = preprocess("1110");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "T4")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("1000");
        block[1] = preprocess("1100");
        block[0] = preprocess("1000");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "L1")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("1000");
        block[1] = preprocess("1000");
        block[0] = preprocess("1100");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "L2")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("1110");
        block[0] = preprocess("1000");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "L3")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("1100");
        block[1] = preprocess("0100");
        block[0] = preprocess("0100");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "L4")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("0010");
        block[0] = preprocess("1110");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "J1")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0100");
        block[1] = preprocess("0100");
        block[0] = preprocess("1100");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "J2")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("1000");
        block[0] = preprocess("1110");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "J3")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("1100");
        block[1] = preprocess("1000");
        block[0] = preprocess("1000");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "J4")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("1110");
        block[0] = preprocess("0010");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "S1")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("0110");
        block[0] = preprocess("1100");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "S2")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("1000");
        block[1] = preprocess("1100");
        block[0] = preprocess("0100");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "Z1")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("1100");
        block[0] = preprocess("0110");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "Z2")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0100");
        block[1] = preprocess("1100");
        block[0] = preprocess("1000");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "I1")
      {
        block[3] = preprocess("1000");
        block[2] = preprocess("1000");
        block[1] = preprocess("1000");
        block[0] = preprocess("1000");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "I2")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("0000");
        block[0] = preprocess("1111");
        info("'" + item_id + "' piece created");
      }
      else if (item_id == "O")
      {
        block[3] = preprocess("0000");
        block[2] = preprocess("0000");
        block[1] = preprocess("1100");
        block[0] = preprocess("1100");
        info("'" + item_id + "' piece created");
      }
      else
      {
        string str = "Failed to create Piece. Unknown item_id '" + item_id + "'";
        throw str;
      }
    };
    const string get_ref_str() const
    {
      string str = "(" + to_string(this->ref.first) + ", " + to_string(this->ref.second) + ")";
      return str;
    }
  };
} // namespace tetris

#endif