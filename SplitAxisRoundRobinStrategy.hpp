#ifndef ROSSB83_SPLIT_AXIS_ROUND_ROBIN_STRATEGY_HPP
#define ROSSB83_SPLIT_AXIS_ROUND_ROBIN_STRATEGY_HPP

#include <vector>
#include <iostream>

#include "SplitAxisStrategy.hpp"

namespace rossb83 {

 // this split axis strategy will split the axis based on the tree level, every level will have the
 // the same axis split, and the axis split will increment by one each level and then start over
 template<typename T>
 class SplitAxisRoundRobinStrategy : public SplitAxisStrategy<T> {

  private:
   // the number of nodes so far
   std::size_t nodes_;

   // the number of levels so far
   std::size_t level_;

   // the previous axis split used
   std::size_t axis_;

  public:
   SplitAxisRoundRobinStrategy() : nodes_(0), axis_(0), level_(1) {}

   // since a kdtree is always balanced, this algorithm can tell what level it is on based
   // on the number of nodes processed so far and the previous level
   std::size_t splitAxis(const std::vector<Point<T>>& points, const std::size_t& begin, const std::size_t& end) {
    if (++nodes_ >= pow(2,level_)) {

     level_++;
     axis_ = ++axis_ % points[begin].dims();
    }
    return axis_;
   }

 };// class Split

} // namespace rossb83

#endif
