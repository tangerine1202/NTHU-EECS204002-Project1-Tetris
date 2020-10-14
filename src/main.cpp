#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Type.h"
#include "Board.h"
#include "Piece.h"
#include "Utils.h"

// enable FINAL_SUBMIT
#define FINAL_SUBMIT
// enable A_SOLUTION
// at "src/Board.h"
// enable log messages
// at "src/Utils.h"
// enable debug mode
// at "src/Utils.h"

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
#if defined(FINAL_SUBMIT)
    output_file_name = "108062308_proj1.final";
#elif defined(A_SOLUTION)
    output_file_name = "output_A.final";
#else
    output_file_name = "output.final";
#endif

    info("input manually");
    info("output file: " + output_file_name);

    info("'cin.rdbuf' to 'cin'");

    try
    {
      fout.open(output_file_name, ofstream::out);
      if (!fout.is_open())
      {
        string err_msg = "failed to open output file.";
        throw err_msg;
      }
    }
    catch (string err_msg)
    {
      error(err_msg);
    }

    isInputFromFile = false;
  }
  else
  {
    input_file_name = argv[1];
    output_file_name = (argc >= 3 ? argv[2] : input_file_name);

    // if there is no output file and input file extension is ".data", then change to ".final". Otherwise, append ".final" directly.
    bool isInputFileEndWithData = true;
    for (int i = 0; i < 5; i++)
    {
      if (input_file_name[input_file_name.length() - 1 - i] != ".data"[4 - i])
        isInputFileEndWithData = false;
    }
    if (!(argc >= 3) && isInputFileEndWithData)
    {
      output_file_name = output_file_name.substr(0, output_file_name.length() - 5);
#if defined(FINAL_SUBMIT)
      output_file_name = "108062308_proj1.final";
#elif defined(A_SOLUTION)
      output_file_name += "_A.final";
#else
      output_file_name += ".final";
#endif
    }
    else
    {
#if defined(FINAL_SUBMIT)
      output_file_name = "108062308_proj1.final";
#elif defined(A_SOLUTION)
      output_file_name += "_A.final";
#else
      output_file_name += ".final";
#endif
    }

    info("input file: " + input_file_name);
    info("output file: " + output_file_name);

    try
    {
      fin.open(input_file_name, ifstream::in);
      if (!fin.is_open())
      {
        string err_msg = "failed to open input file.";
        throw err_msg;
      }
      else
      {
        cin.rdbuf(fin.rdbuf());
        info("'cin.rdbuf' from 'fin'");
      }

      fout.open(output_file_name, ofstream::out);
      if (!fout.is_open())
      {
        string err_msg = "failed to open output file.";
        throw err_msg;
      }
    }
    catch (string err_msg)
    {
      error(err_msg);
    }

    isInputFromFile = true;
  }

  if (!isInputFromFile || (fin.is_open() && fout.is_open()))
    test(fout);

  fin.close();
  info("input file " + input_file_name + " closed");
  fout.close();
  info("output file " + output_file_name + " closed");

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
    catch (string err_msg)
    {
      error(err_msg);
      // return 1;
    }
  }

  board.write_in_file(fout);

#if defined(VERBOSE)
  board.write_to_console();
#endif

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
