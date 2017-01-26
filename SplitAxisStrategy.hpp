#ifndef ROSSB83_SPLIT_AXIS_STRATEGY_HPP
#define ROSSB83_SPLIT_AXIS_STRATEGY_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <tuple>
#include <regex>
#include <functional>
#include <algorithm>
#include <numeric>
#include <limits>
#include <math.h>
#include <complex>

namespace rossb83 {

 // this abstract class is an interface to determine which axis the kdtree will split on
 template<typename T>
 class SplitAxisStrategy {

  public:
   // this pure virtual method is an interface to the axis selection strategy
   virtual std::size_t splitAxis(const std::vector<Point<T>>& points, const std::size_t& begin, const std::size_t& end) = 0;

 }; // class SplitAxisStrategy
} // namespace rossb83

#endif
