#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Type.h"
#include "Board.h"
#include "Piece.h"
#include "Utils.h"

using namespace std;
using tetris::Board;
using tetris::Piece;
using utils::debug;
using utils::error;
using utils::info;
using utils::warning;

int test(ofstream &fout);
void move_piece(Board &board, Piece &piece, int shift);
void update_board(Board &board);

int main(int argc, char *argv[])
{
  ifstream fin;
  ofstream fout;
  string input_file_name;
  string output_file_name;
  bool isInputFromFile;

  // set I/O
  if (argc <= 1)
  {
    output_file_name = "output.final";

    info("input manually");
    info("output file: " + output_file_name);

    info("'cin.rdbuf' to 'cin'");

    fout.open(output_file_name, fstream::out);

    if (!fout.is_open())
      error("failed to open output file.");

    isInputFromFile = false;
  }
  else
  {
    input_file_name = argv[1];
    output_file_name = argc >= 3 ? argv[2] : input_file_name + ".final";

    info("input file: " + input_file_name);
    info("output file: " + output_file_name);

    fin.open(input_file_name, fstream::in);
    fout.open(output_file_name, fstream::out);

    if (!fin.is_open())
      error("failed to open input file.");
    else
    {
      cin.rdbuf(fin.rdbuf());
      info("'cin.rdbuf' from 'fin'");
    }

    if (!fout.is_open())
      error("failed to open output file.");

    isInputFromFile = true;
  }

  if (!isInputFromFile || (fin.is_open() && fout.is_open()))
    test(fout);

  fin.close();
  fout.close();

  return 0;
}

int test(ofstream &fout)
{
  int height;
  int width;
  string item_id;
  int col, shift;

  // init
  cin >> height >> width;
  Board board(height, width);

  while (cin >> item_id)
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
    if (item_id == "show_more")
    {
      board.show_more();
      continue;
    }
    if (item_id == "show_all")
    {
      board.show_all();
      continue;
    }

    try
    {
      // WARN: row & col start from 1
      cin >> col >> shift;
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

#if defined(VERBOSE)
  board.write_in_file(fout);
#endif

  board.write_to_console();

  return 0;
}

void move_piece(Board &board, Piece &piece, const int shift)
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

  board.show();
}