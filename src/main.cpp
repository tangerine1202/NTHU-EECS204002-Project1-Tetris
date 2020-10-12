#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Type.h"
#include "Board.h"
#include "Piece.h"
#include "Utils.h"

using namespace std;
using tetrix::Board;
using tetrix::Piece;
using utils::debug;
using utils::error;
using utils::info;
using utils::warning;

int test(fstream &fin, fstream &fout);
void move_piece(Board &board, Piece &piece, int shift);
void update_board(Board &board);

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    warning("Only one input file, one output file a time, get '" + to_string(argc - 1) + "'");
    for (int i = 1; i < argc; i++)
      warning("argv[" + to_string(i) + "]: " + argv[i]);
  }

  fstream fin, fout;
  const string input_file_name = argv[1];
  const string output_file_name = argv[2];

  fin.open(input_file_name, fstream::in);
  fout.open(output_file_name, fstream::out);

  if (fin.is_open())
    error("failed to open input file.");
  if (fout.is_open())
    error("failed to open output file.");

  if (fin.is_open() && fout.is_open())
    test(fin, fout);

  fin.close();
  fout.close();

  return 0;
}

int test(fstream &fin, fstream &fout)
{
  int height;
  int width;
  string item_id;
  int col, shift;

  // init
  fin >> height >> width;
  Board board(height, width);

  while (fin >> item_id)
  {
    if (item_id == "End" || item_id == "end")
    {
      info("The End of the input");
      break;
    }

    if (item_id == "show")
    {
      board.show();
      continue;
    }

    try
    {
      // WARN: row & col start from 1
      fin >> col >> shift;
      col -= 1;

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

  board.write_in_file(fout);

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

  // show_board(board);
}