#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include "Piece.h"
#include "Utils.h"
#include "Type.h"

// enable A_SOLUTION
// #define A_SOLUTION

extern const int maxn = 15 + 5;
extern const int maxm = 40 + 5;

namespace tetris
{
  using std::cout;
  using std::ofstream;
  using std::string;
  using std::to_string;
  using std::vector;
  using utils::debug;
  using utils::error;
  using utils::info;
  using utils::warning;

  class Board
  {
  private:
    vector<DataType> board;
    int height;
    int width;
    int need_update_height;

  public:
    Board(){};
    Board(const int height, const int width)
        : height(height),
          width(width),
          need_update_height(height + 5)
    {
      board.resize(maxn, 0);
    };
    // move_piece main fn
    int down_piece(Piece &piece);
    int shift_piece(Piece &piece, const int shift);
    int set_piece(const Piece &piece);
    // move_piece support fn
    bool isValidRef(const Piece &item, const Point &next_ref) const;
    bool isNotOverlapping(const Piece &item, const Point &next_ref) const;
    bool isInHorizontalBound(const Piece &piece, const Point &next_ref) const;

    // update_board main fn
    int update();
    // update_board  supoort fn
    bool can_fit_into(const int source_line_idx, const int target_line_idx) const;
    int fit_into(const int source_line_idx, const int target_line_idx);

    // cshow board in console
    void show(const int indicate_row = -1) const;
    void show_more(const int indicate_row = -1) const;
    void show_all(const int indicate_row = -1) const;

    // write output into file
    void write_in_file(ofstream &fout);
    // write output to console (without VERBOSE block)
    void write_to_console();
  };

  /**
   * Piece fall down until stuck.
   * checking:
   * - init ref check
   * - down ref check
   */
  int Board::down_piece(Piece &piece)
  {
    info("start to down piece");

    Point next_ref(piece.ref.first, piece.ref.second);

    // check init point
    const bool isInitRefValid = this->isValidRef(piece, piece.ref);
    if (!isInitRefValid)
    {
      string err_msg = "init ref " + piece.get_ref_str() + " is invalid";
      throw err_msg;
    }

    while (next_ref.first - 1 >= 0)
    {
      // down one line
      next_ref.first--;

      debug("next_ref: (" + to_string(next_ref.first) + ", " + to_string(next_ref.second) + ")");

      const bool isValidRef = this->isValidRef(piece, next_ref);
      if (!isValidRef)
      {
        // Since this line is invalid, rollback last fall down.
        next_ref.first++;
        break;
      }
    }

    piece.ref = next_ref;
    info("piece down stop at " + piece.get_ref_str());

    return 0;
  }

  /**
   * Piece shift horizontally until stuck.
   * checking:
   * - shift ref check
  */
  int Board::shift_piece(Piece &piece, const int shift)
  {
    info("start to shift piece");

    Point next_ref(piece.ref.first, piece.ref.second);
    int shift_cnt = shift;
    const bool shift_right = shift_cnt > 0 ? true : false;

    while (shift_cnt != 0)
    {
      // shift one block
      next_ref.second += (shift_right ? 1 : -1);
      shift_cnt += (shift_right ? -1 : 1);

      const bool isValidRef = this->isValidRef(piece, next_ref);
      if (!isValidRef)
      {
        // Since this line is invalid, rollback last move.
        next_ref.second -= (shift_right ? 1 : -1);
        shift_cnt -= (shift_right ? -1 : 1);
        break;
      }
    }

    // Should be able to shift 'shift' times
    if (shift_cnt != 0)
    {
      string str = "Failed to shift item. '" + to_string(shift_cnt) + "' times remain.";
      throw str;
    }

    piece.ref = next_ref;
    info("piece shift '" + (string)(shift_right ? "right" : "left") + "' and stop at " + piece.get_ref_str());

    return 0;
  }

  int Board::set_piece(const Piece &piece)
  {
    for (int i = 0; i < 4; i++)
    {
      const int source_row = i;
      const int target_row = piece.ref.first + i;
      const int shift = piece.ref.second;
      const DataType source_data = piece.block[source_row] >> shift;

      this->board[target_row] ^= source_data;
    }

    info("piece set at " + piece.get_ref_str());
    this->show_more();

    return 0;
  }

  /**
   * WARN:
   * NO upper bound checked
   * Only check overlapping & horizontal bound
   */
  bool Board::isValidRef(const Piece &piece, const Point &next_ref) const
  {
    bool result = true;

    const bool isInHorizontalBound = this->isInHorizontalBound(piece, next_ref);
    const bool isNotOverlapping = this->isNotOverlapping(piece, next_ref);
    if (!isInHorizontalBound || !isNotOverlapping)
      result = false;

    string str = "piece at (" + to_string(next_ref.first) + ", " + to_string(next_ref.second) + ") " +
                 "is " + (result ? "valid" : "invalid");
    debug(str);

    return result;
  }

  bool Board::isNotOverlapping(const Piece &piece, const Point &next_ref) const
  {
    bool result = true;
    for (int i = 0; i < 4; i++)
    {
      const int source_row = i;
      const int target_row = next_ref.first + i;
      const int target_col = next_ref.second;
      const DataType source_data = piece.block[source_row] >> target_col;
      const DataType target_data = this->board[target_row];

      // check overlaping
      if ((source_data & target_data) != 0)
      {
        result = false;
        break;
      }
    }

    string str = "piece at (" + to_string(next_ref.first) + ", " + to_string(next_ref.second) + ") " +
                 (result ? "no overlapping occured" : "cause overlapping");
    debug(str);

    return result;
  }

  bool Board::isInHorizontalBound(const Piece &piece, const Point &next_ref) const
  {
    bool result = true;

    for (int i = 0; i < 4; i++)
    {
      const int source_row = i;
      const int target_col = next_ref.second;

      // Check left bound
      // remove out of left bound block
      const DataType left_bound_removed_tmp = piece.block[source_row] >> target_col;
      const DataType left_bound_removed_data = left_bound_removed_tmp >> this->width;
      // not remove out of left bound block
      const DataType left_tmp = piece.block[source_row] >> this->width;
      const DataType left_data = left_tmp >> target_col;

      const bool isOutOfLeftBound = (left_data != left_bound_removed_data) || (target_col < 0);
      if (isOutOfLeftBound)
      {
        debug("Out of left bound.");
        result = false;
      }

      // Check right bound
      // not remove out of right bound block
      const DataType right_data = piece.block[source_row] >> target_col;
      // remove out of right bound block
      const DataType right_bound_removed_tmp = right_data >> (maxm - this->width);
      const DataType right_bound_removed_data = right_bound_removed_tmp << (maxm - this->width);

      const bool isOutOfRightBound = (right_data != right_bound_removed_data) || (target_col >= this->width);
      if (isOutOfRightBound)
      {
        debug("Out of right bound.");
        result = false;
      }

      if (!result)
      {
        /*
        debug("source_row: " + to_string(source_row));
        debug("shift: " + to_string(shift));
        debug("left data:               " + left_data.to_string().substr(0, this->width * 2));
        debug("left bound removed data: " + left_bound_removed_data.to_string().substr(0, this->width * 2));
        debug("right data:               " + right_data.to_string().substr(0, this->width * 2));
        debug("right bound removed data: " + right_bound_removed_data.to_string().substr(0, this->width * 2));
        */
        break;
      }
    }

    if (result)
    {
      string str = "piece at (" + to_string(next_ref.first) + ", " + to_string(next_ref.second) + ") " +
                   "is in the horizontal bound";
      debug(str);
    }

    return result;
  }

  int Board::update()
  {
    int line_idx = 0;
    // Store the line index of the line which have blocks. (Stack)
    vector<int> s;

    // 1. calculate state after falling and eliminating
    DataType line_data;
    for (; line_idx < this->need_update_height; line_idx++)
    {
      line_data = this->board[line_idx];

      // skip full or empty line
      if (line_data.count() >= width || line_data.count() == 0)
      {
        debug("line '" + to_string(line_idx) + "' is empty or full, skip.");
        continue;
      }

      // No line below current line
      if (s.empty())
      {
        debug("s is empty, push line '" + to_string(line_idx) + "' into s");
        s.push_back(line_idx);
        continue;
      }

#if defined(A_SOLUTION)
      const bool can_fit_into = this->can_fit_into(line_idx, s.back());
      if (can_fit_into)
      {
        // fit current line into s.top() line
        this->fit_into(line_idx, s.back());

        // fit_into then line is full
        const bool isSTopFull = (this->board[s.back()].count() == width);
        if (isSTopFull)
        {
          debug("s.top is full, pop.");
          s.pop_back();
        }

        continue;
      }
#endif

      debug("push line '" + to_string(line_idx) + "'");
      s.push_back(line_idx);
    }

    // 2. check upper bound
    const int updated_height = s.size();
    if (updated_height > height)
    {
      string err_msg = "Out of upper bound. Block stack to '" + to_string(updated_height) + "' height";
      error(err_msg);
      throw err_msg;
    }

    // 3. update the board
    for (int i = 0; i < updated_height; i++)
      this->board[i] = this->board[s[i]];

    // 4. clean above lines
    for (int i = updated_height; i < need_update_height; i++)
      this->board[i] = 0;

    info("board updated");

    return 0;
  }

  bool Board::can_fit_into(const int source_line_idx, const int target_line_idx) const
  {
    const DataType source_data = this->board[source_line_idx];
    const DataType target_data = this->board[target_line_idx];
    bool res;

    if ((source_data & target_data) == 0)
      res = true;
    else
      res = false;

    debug("line '" + to_string(source_line_idx) + "'  " +
          (res ? "can" : "can't") +
          " fit into line '" + to_string(target_line_idx) + "'");

    return res;
  }

  int Board::fit_into(const int source_line_idx, const int target_line_idx)
  {
    // TODO: check overlaping!
    this->board[target_line_idx] ^= this->board[source_line_idx];

    debug("fit line '" + to_string(source_line_idx) + "'  " +
          "into line '" + to_string(target_line_idx) + "'");

    return 0;
  }

  // show board functions
  void Board::show(const int indicate_row) const
  {
    string output = "show board: \n";
    output += '\n';
    for (int i = this->height - 1; i >= 0; i--)
    {
      output += this->board[i].to_string().substr(0, this->width);
      if (i == indicate_row)
        output += "<-";
      output += '\n';
    }
    info(output);
  }

  // show need_update_height * width board
  void Board::show_more(const int indicate_row) const
  {
    string output = "show more board: \n";
    output += '\n';
    for (int i = this->need_update_height - 1; i >= this->height; i--)
    {
      output += this->board[i].to_string().substr(0, this->width);
      if (i == indicate_row)
        output += "<-";
      output += '\n';
    }

    for (int i = 0; i < this->width; i++)
      output += '~';
    output += '\n';

    for (int i = this->height - 1; i >= 0; i--)
    {
      output += this->board[i].to_string().substr(0, this->width);
      if (i == indicate_row)
        output += "<-";
      output += '\n';
    }
    output += '\n';
    info(output);
  }

  // show need_update_height * (width * 2) board
  void Board::show_all(const int indicate_row) const
  {
    string output = "show more board: \n";
    output += '\n';
    for (int i = this->need_update_height - 1; i >= this->height; i--)
    {
      output += this->board[i].to_string().substr(0, this->width);
      output += "|";
      output += this->board[i].to_string().substr(this->width, this->width);
      if (i == indicate_row)
        output += "<-";
      output += '\n';
    }

    for (int i = 0; i < this->width * 2 + 1; i++)
      output += '~';
    output += '\n';

    for (int i = this->height - 1; i >= 0; i--)
    {
      output += this->board[i].to_string().substr(0, this->width);
      output += "|";
      output += this->board[i].to_string().substr(this->width, this->width);
      if (i == indicate_row)
        output += "<-";
      output += '\n';
    }
    output += '\n';
    info(output);
  }

  // write output into file
  void Board::write_in_file(ofstream &fout)
  {
    for (int i = this->height - 1; i >= 0; i--)
    {
      for (int j = maxm - 1; j >= maxm - this->width; j--)
        fout << this->board[i][j] << " \n"[j == (maxm - this->width)];
    }
  }
  // write output to console without VERBOSE block
  void Board::write_to_console()
  {
    for (int i = this->height - 1; i >= 0; i--)
    {
      for (int j = maxm - 1; j >= maxm - this->width; j--)
        cout << this->board[i][j] << " \n"[j == (maxm - this->width)];
    }
  }

} // namespace tetris

#endif