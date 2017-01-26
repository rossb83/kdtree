#ifndef ROSSB83_POINT_SELECT_STRATEGY_TEST_HPP
#define ROSSB83_POINT_SELECT_STRATEGY_TEST_HPP

#include <assert.h>
#include "SplitPointStrategy.hpp"
#include "SplitPointSelectStrategy.hpp"
#include "SplitPointSelectStrategyTest.hpp"

namespace rossb83 {

 class SplitPointSelectStrategyTest {

  public:

   SplitPointSelectStrategyTest() {

    std::cout << "Running Split Point Sort Strategy tests..." << std::endl;

    selectTest();
   }

  private:

   void selectTest() {

    std::cout << "split point select test..." << std::endl;
    strategy = std::make_shared<SplitPointSelectStrategy<int>>();
    
    std::vector<Point<int>> points;
    points.push_back({7,2,8});
    points.push_back({1,6,4});
    points.push_back({9,8,0});
    points.push_back({4,9,9});
    points.push_back({5,0,1});

    Point<int> p = strategy->splitPoint(points,0,0,4);


    //assert(points[2][0] == 4);
    
    //strategy->splitPoint(points,1,1,4);
    //assert(points[3][1] == 8);
   }

   std::shared_ptr<SplitPointStrategy<int>> strategy;

 }; // class SplitPointSelectStrategyTest

} // namespace rossb83

#endif // ROSSB83_SPLIT_POINT_SELECT_STRATEGY_TEST_HPP
