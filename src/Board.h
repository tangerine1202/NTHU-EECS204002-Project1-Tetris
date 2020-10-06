#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include "Shape.h"
#include "utils.h"

const int maxn = 15 + 5;
const int maxm = 40 + 5;

namespace tetrix
{
  using std::cout;
  using std::string;
  using std::to_string;
  using std::vector;
  using utils::debug;
  using utils::info;
  using utils::warning;

  class Board
  {
  private:
    vector<int> board[maxn];
    int height;
    int width;
    int needUpdateHeight;

  public:
    Board(int height, int width)
        : height(height),
          width(width),
          needUpdateHeight(height + 5)
    {
      for (int i = 0; i < maxn; i++)
        board[i] = vector<int>(maxm, 0);
    };
    int fallItem(Shape &itme);
    int moveItem(Shape &item, int mov);
    int setItem(Shape &item);

    int findEliminate(int prev_eliminate_row);
    int eliminateLine(int row);
    int fallInLine(int row);

    bool isValidRef(Shape &item, Point next_ref);
    bool isItemInBoard(Shape &item);
    bool canEliminateLine(int row);
    bool checkOutOfBound();

    // void show();
    void show(int indicate_row = -1);
    void show_more(int indicate_row);
  };

  /**
   * Item fall down until stuck.
   */
  int Board::fallItem(Shape &item)
  {
    Point next_ref(item.ref.first, item.ref.second);

    while (next_ref.first - 1 >= 0)
    {
      // down one line
      next_ref.first--;

      const bool isValidRef = this->isValidRef(item, next_ref);
      if (!isValidRef)
      {
        // Since this line is invalid, rollback last fall down.
        next_ref.first++;
        break;
      }
    }

    // update item position
    item.ref = next_ref;
    info("item fall stop at line: " + to_string(next_ref.first));

    return 0;
  }

  int Board::moveItem(Shape &item, int mov)
  {
    debug("'mov' is " + to_string(mov));

    Point next_ref(item.ref.first, item.ref.second);
    const int moveDirection = mov > 0 ? 1 : -1;

    while (mov != 0 &&
           next_ref.second + moveDirection >= 0 &&
           next_ref.second + moveDirection <= this->width)
    {
      // move one block
      next_ref.second += moveDirection;
      mov -= moveDirection;

      const bool isValidRef = this->isValidRef(item, next_ref);
      if (!isValidRef)
      {
        // Since this line is invalid, rollback last move.
        next_ref.second -= moveDirection;
        mov += moveDirection;
        break;
      }
    }

    // Should be able to move 'mov' times
    if (mov != 0)
    {
      string str = "Failed to move item, " + to_string(mov) + " times remained";
      throw str;
    }

    // update item position
    item.ref = next_ref;

    string direction_str = (moveDirection == 1 ? "right" : "left");
    info("item move " + direction_str + " at col: " + to_string(next_ref.second));
    return 0;
  }

  int Board::setItem(Shape &item)
  {
    for (int i = 0; i < 4; i++)
    {
      const int row = item.ref.first + item.block[i].first;
      const int col = item.ref.second + item.block[i].second;
      // TODO: +1 to check if there are double assign
      this->board[row][col] += 1;
    }

    debug("set item at " + item.getRefStr());
    this->show();

    return 0;
  }

  // Check board if can eliminate
  int Board::findEliminate(int prev_eliminate_row)
  {
    debug("start findEliminate from line " + to_string(prev_eliminate_row));
    this->show(prev_eliminate_row);

    for (int row = prev_eliminate_row; row < this->needUpdateHeight; row++)
    {
      const bool canEliminateLine = this->canEliminateLine(row);
      if (canEliminateLine)
      {
        this->eliminateLine(row);
      }
    }
    return 0;
  }

  // eliminate line and fall line
  int Board::eliminateLine(int row)
  {
    debug("eliminate line: " + to_string(row));

    for (int i = row; i < this->needUpdateHeight; i++)
    {
      this->board[i] = this->board[i + 1];
      this->show_more(i);
    }

    debug("eliminated");

    this->fallInLine(row - 1);
    return 0;
  }

  // Above line fit in current line(row)
  int Board::fallInLine(int row)
  {
    // Can't fit in to -n line
    if (row >= 0)
    {
      bool canFall = true;
      for (int i = 0; i < this->width; i++)
      {
        if (this->board[row][i] && this->board[row + 1][i])
        {
          canFall = false;
          break;
        }
      }

      if (canFall)
      {
        info("Can fall line into: " + to_string(row));
        debug("fall line: " + to_string(row));
        for (int i = 0; i < this->width; i++)
        {
          this->board[row][i] = this->board[row][i] | this->board[row + 1][i];
        }
      }
      else
        info("Can't fall line " + to_string(row + 1) + " into line " + to_string(row));
      this->findEliminate(row);
    }

    // FIXME: performance concern, this function call need review.
    this->findEliminate(0);
    return 0;
  }

  /**
   * WARN:
   * Only check **horizontal** bound.
   * Vertical bound check after eliminating finished.
   */
  bool Board::isValidRef(Shape &item, Point next_ref)
  {
    for (int i = 0; i < 4; i++)
    {
      const int row = next_ref.first + item.block[i].first;
      const int col = next_ref.second + item.block[i].second;

      // Only check **horizontal** bound.
      const bool isInHorizontalBound = (col >= 0 && col < this->width);

      if (this->board[row][col] || !isInHorizontalBound)
        return false;
    }

    string str = "item at (" + to_string(next_ref.first) + ", " + to_string(next_ref.second) + ") is valid.";
    info(str);

    return true;
  }

  bool Board::isItemInBoard(Shape &item)
  {
    for (int i = 0; i < 4; i++)
    {
      const int row = item.ref.first + item.block[i].first;
      const int col = item.ref.second + item.block[i].second;
      if (row < 0 || row >= this->width || col < 0 || col >= this->height)
        return false;
    }
    return true;
  }

  bool Board::canEliminateLine(int row)
  {
    for (int i = 0; i < this->width; i++)
    {
      if (this->board[row][i] == 0)
        return false;
    }
    info("Can eliminate row: " + to_string(row));
    return true;
  }

  bool Board::checkOutOfBound()
  {
    for (int i = this->height; i < this->needUpdateHeight; i++)
      for (int j = 0; j < this->width; j++)
        if (this->board[i][j] != 0)
          return true;
    return false;
  }

  void Board::show(int indicate_row)
  {
    cout << '\n';
    for (int i = this->height - 1; i >= 0; i--)
    {
      for (int j = 0; j < this->width; j++)
      {
        cout << this->board[i][j] << ' ';
      }
      if (i == indicate_row)
        cout << "<-";
      cout << '\n';
    }
    cout << '\n';
  }

  // show needUpdateHeight board
  void Board::show_more(int indicate_row = -1)
  {
    cout << '\n';
    for (int i = this->needUpdateHeight - 1; i >= height; i--)
    {
      for (int j = 0; j < this->width; j++)
      {
        cout << this->board[i][j] << ' ';
      }
      if (i == indicate_row)
        cout << "<-";
      cout << '\n';
    }

    for (int i = 0; i < this->width * 2; i++)
      cout << '~';
    cout << '\n';

    for (int i = this->height - 1; i >= 0; i--)
    {
      for (int j = 0; j < this->width; j++)
      {
        cout << this->board[i][j] << ' ';
      }
      if (i == indicate_row)
        cout << "<-";
      cout << '\n';
    }
    cout << '\n';
  }

} // namespace tetrix

#endif
