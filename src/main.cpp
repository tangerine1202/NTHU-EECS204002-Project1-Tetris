#include <iostream>
#include <vector>
#include <string>
#include "Board.h"
#include "Shape.h"
#include "utils.h"

using namespace std;
using std::cout;
using std::pair;
using std::string;
using std::to_string;
using tetrix::Board;
using tetrix::Shape;
using utils::debug;
using utils::error;
using utils::info;
using utils::warning;

void update(Board &board, string item_id, int height, int col, int mov);

int main()
{
  int height;
  int width;
  string item_id;
  int col, mov;

  // init
  cin >> height >> width;
  Board board(height, width);

  while (cin >> item_id)
  {
    if (item_id == "End")
      break;

    try
    {
      cin >> col >> mov;
      update(board, item_id, height, col, mov);
    }
    catch (const char *msg)
    {
      string str(msg);
      error(str);
    }
    catch (string msg)
    {
      error(msg);
    }
  }

  board.show();

  return 0;
}

void update(Board &board, string item_id, int height, int col, int mov)
{
  // 1. Create according Shape
  // TODO: check all start col should in the board
  Shape item(item_id, height, col);

  // 2. item fall down
  info("Item first falling started");
  board.fallItem(item);
  info("Item first falling ended");

  // 3. item move horizontially
  info("Item moving started");
  board.moveItem(item, mov);
  info("Item moving ended");

  // 4. item fall down again
  info("Item second falling started");
  board.fallItem(item);
  info("Item second falling ended");

  // 5. set item on board
  info("Item setting started");
  board.setItem(item);
  info("Item setting ended");

  // 6. fall line
  info("Update board started");
  board.findEliminate(0);
  info("Update board ended");

  // 7. check out of bound
  info("Out of Bound Checking started");
  const bool haveOutOfBound = board.checkOutOfBound();
  if (haveOutOfBound)
  {
    throw "Item out of the board (game over)";
  }
  info("Out of Bound Checking ended");
}