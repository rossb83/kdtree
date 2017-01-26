#include <iostream>

#include "PointTest.hpp"
#include "PointCloudTest.hpp"
#include "PCDFileTest.hpp"
#include "SplitAxisRoundRobinStrategyTest.hpp"
#include "SplitAxisRangeStrategyTest.hpp"
#include "SplitPointSortStrategyTest.hpp"
#include "SplitPointSelectStrategyTest.hpp"
#include "KDTreeTest.hpp"

int main(int argc, char* argv[]) {

 rossb83::PointTest pointtest;
 rossb83::PointCloudTest pointcloudtest;
 rossb83::PCDFileTest pcdfiletest;
 rossb83::SplitAxisRoundRobinStrategyTest splitAxisRRTest;
 rossb83::SplitPointSortStrategyTest splitPointSortTest;
 rossb83::SplitPointSelectStrategyTest splitPointSelectStrategyTest;
 rossb83::SplitAxisRangeStrategyTest splitAxisRangeStrategyTest;
 rossb83::KDTreeTest kdTreeTest;
 return 0;
}
