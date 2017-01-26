#ifndef ROSSB83_SPLIT_AXIS_RANGE_STRATEGY_TEST_HPP
#define ROSSB83_SPLIT_AXIS_RANGE_STRATEGY_TEST_HPP

#include <assert.h>

#include "SplitAxisRangeStrategy.hpp"

namespace rossb83 {

 class SplitAxisRangeStrategyTest {

  public:

   SplitAxisRangeStrategyTest() {

    std::cout << "Running SplitAxis Range Strategy tests..." << std::endl;

    rangeTest();
   }

  private:

   void rangeTest() {

    std::cout << "split axis range test..." << std::endl;
    strategy = std::make_shared<SplitAxisRangeStrategy<int>>();
    
    std::size_t axis1 = strategy->splitAxis({{0,1,2},{3,4,5},{11,7,8}},0,2);
    std::size_t axis2 = strategy->splitAxis({{0,1,2},{3,7,5},{11,7,8}},0,1);
    std::size_t axis3 = strategy->splitAxis({{0,1,2},{3,7,9},{11,7,8}},0,1);
    std::size_t axis4 = strategy->splitAxis({{0,1,2},{3,7,9},{11,7,8}},0,0);
 
    assert(axis1 == 0);
    assert(axis2 == 1);
    assert(axis3 == 2);
    assert(axis4 == 0);
   }

   std::shared_ptr<SplitAxisStrategy<int>> strategy;

 }; // class SplitRangeRobinStrategyTest

} // namespace rossb83

#endif // ROSSB83_SPLIT_AXIS_ROUND_ROBIN_STRATEGY_TEST_HPP
