#ifndef ROSSB83_SPLIT_POINT_STRATEGY_HPP
#define ROSSB83_SPLIT_POINT_STRATEGY_HPP

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

 // this abstract class is an interface to determine which point the kdtree will make the next node, it works by
 // finding the median in an input vector of points and placing that median point in the center of the vector
 template<typename T>
 class SplitPointStrategy {

  public:
   // this pure virtual method is an interface to the point selection strategy, it will re-arrange the input
   // point vector with the median placed at the center between begin and end
   virtual Point<T> splitPoint(std::vector<Point<T>>& points, const std::size_t& dim, const std::size_t& begin, const std::size_t& end) = 0;

 }; // class SplitPointStrategy
} // namespace rossb83

#endif // ROSSB83_SPLIT_POINT_STRATEGY
