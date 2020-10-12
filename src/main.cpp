#include <iostream>
#include <vector>
#include <string>
#include "Board.h"
#include "Piece.h"
#include "Utils.h"
#include "Type.h"

using namespace std;
using tetrix::Board;
using tetrix::Piece;
using utils::debug;
using utils::error;
using utils::info;
using utils::warning;

// void init(Board &board, int height, int width);
void move_piece(Board &board, Piece &piece, int shift);
void update_board(Board &board);

int main()
{
  int height;
  int width;
  string item_id;
  int col, shift;
  // Board board;

  // init
  cin >> height >> width;
  Board board(height, width);
  // init(board, height, width);

  while (cin >> item_id)
  {
    if (item_id == "End" || item_id == "end")
      break;

    if (item_id == "show")
    {
      board.show();
      continue;
    }

    try
    {
      // WARN: row & col start from 1
      cin >> col >> shift;
      // col -= 1;

      // init piece
      // TODO: check all start col should in the board
      Piece piece(item_id, height, col);

      // piece movement
      move_piece(board, piece, shift);

      // line falling & eliminating
      update_board(board);
    }
    catch (string msg)
    {
      error(msg);
      // return 1;
    }
  }

  board.show();

  return 0;
}

void move_piece(Board &board, Piece &piece, int shift)
{
  board.down_piece(piece);

  board.shift_piece(piece, shift);

  board.down_piece(piece);

  board.set_piece(piece);
}

void update_board(Board &board)
{
  // calculate result & set to board
  board.update();

  debug("result as following");
  board.show();
}