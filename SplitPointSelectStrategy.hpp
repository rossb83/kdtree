#ifndef ROSSB83_SPLIT_POINT_SELECT_STRATEGY
#define ROSSB83_SPLIT_POINT_SELECT_STRATEGY

#include <unordered_map>
#include <tuple>
#include <regex>
#include <functional>
#include <algorithm>
#include <numeric>
#include <limits>
#include <math.h>
#include <complex>
#include <cmath>

#include "SplitPointStrategy.hpp"

namespace rossb83 {

 // this strategy determines which point the kdtree will make the next node, it works by
 // sorting the input vector
 template<typename T>
 class SplitPointSelectStrategy : public SplitPointStrategy<T> {

  public:
   // this method will re-arrange the input by sorting it placing the median between begin and end
   Point<T> splitPoint(std::vector<Point<T>>& points, const std::size_t& dim, const std::size_t& begin, const std::size_t& end) {

    auto mid = points.begin() + std::ceil((begin + end)/2.0);

    // sort using the default operator<
    std::nth_element(points.begin() + begin, mid, points.begin() + end + 1,
    [dim](Point<T> p1, Point<T> p2) {return p1[dim] < p2[dim];});
   

    return std::move(points[std::ceil((begin + end)/2.0)]);
   }

 }; // class SplitPointSelectStrategy

} // namespace rossb83

#endif // ROSSB83_SPLIT_POINT_SELECT_STRATEGY
