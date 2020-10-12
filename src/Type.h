#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <bitset>

namespace tetris
{
  const int maxn = 15 + 5;
  // times 2 for checking horizontal bound
  const int maxm = 40 * 2 + 5;
  using DataType = std::bitset<maxm>;
  using Point = std::pair<int, int>;
} // namespace tetris

#endif