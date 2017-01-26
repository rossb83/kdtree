#ifndef ROSSB83_SPLIT_AXIS_ROUND_ROBIN_STRATEGY_TEST_HPP
#define ROSSB83_SPLIT_AXIS_ROUND_ROBIN_STRATEGY_TEST_HPP

#include <assert.h>

#include "SplitAxisRoundRobinStrategy.hpp"

namespace rossb83 {

 class SplitAxisRoundRobinStrategyTest {

  public:

   SplitAxisRoundRobinStrategyTest() {

    std::cout << "Running SplitAxis Round Robin Strategy tests..." << std::endl;

    roundRobinTest();
   }

  private:

   void roundRobinTest() {

    std::cout << "split axis round robin test..." << std::endl;
    strategy = std::make_shared<SplitAxisRoundRobinStrategy<int>>();
    
    std::size_t axis1 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
    std::size_t axis2 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
    std::size_t axis3 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
    std::size_t axis4 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
    std::size_t axis5 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
    std::size_t axis6 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
    std::size_t axis7 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
    std::size_t axis8 = strategy->splitAxis({{0,1,2},{1,2,2}},0,1);
   
    assert(axis1 == 0);
    assert(axis2 == 1);
    assert(axis3 == 1);
    assert(axis4 == 2);
    assert(axis5 == 2);
    assert(axis6 == 2);
    assert(axis7 == 2);
    assert(axis8 == 0);
   }

   std::shared_ptr<SplitAxisStrategy<int>> strategy;

 }; // class SplitAxisRoundRobinStrategyTest

} // namespace rossb83

#endif // ROSSB83_SPLIT_AXIS_ROUND_ROBIN_STRATEGY_TEST_HPP
