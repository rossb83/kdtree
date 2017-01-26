#ifndef ROSSB83_POINT_SORT_STRATEGY_TEST_HPP
#define ROSSB83_POINT_SORT_STRATEGY_TEST_HPP

#include <assert.h>
#include "SplitPointStrategy.hpp"
#include "SplitPointSortStrategy.hpp"

namespace rossb83 {

 class SplitPointSortStrategyTest {

  public:

   SplitPointSortStrategyTest() {

    std::cout << "Running Split Point Sort Strategy tests..." << std::endl;

    sortTest();
   }

  private:

   void sortTest() {

    std::cout << "split point sort test..." << std::endl;
    strategy = std::make_shared<SplitPointSortStrategy<int>>();
    
    std::vector<Point<int>> points;
    points.push_back({7,2,8});
    points.push_back({1,6,4});
    points.push_back({9,8,0});
    points.push_back({4,9,9});
    points.push_back({5,0,1});

    //strategy->splitPoint(points,0,1,3);
    //assert(points[2][0] == 4);
    
    //strategy->splitPoint(points,1,1,4);
    //assert(points[3][1] == 8);
   }

   std::shared_ptr<SplitPointStrategy<int>> strategy;

 }; // class SplitPointSortStrategyTest

} // namespace rossb83

#endif // ROSSB83_SPLIT_POINT_SORT_STRATEGY_TEST_HPP
