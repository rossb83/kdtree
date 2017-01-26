#ifndef ROSSB83_POINTCLOUD_TEST_HPP
#define ROSSB83_POINTCLOUD_TEST_HPP

#include <assert.h>

#include "PointCloud.hpp"

namespace rossb83 {

 class PointCloudTest {

  public:

   PointCloudTest() {

    std::cout << "Running Point Cloud tests..." << std::endl;
   
    createTest();
    initTest();
    insertTest();
    moveTest();
    moveAssignmentTest();
    bruteForceNearestNeighborTest();
   }

  private:

   void createTest() {
    
    std::cout << "PoinCloud create test..." << std::endl;    
    
    assert(p1.capacity() == 100);
    assert(p1.dims() == 5);
    assert(p1.points() == 0);
   }

   void initTest() {
   
    std::cout << "PointCloud init test..." << std::endl;
   
    assert(p2.capacity() == 2);
    assert(p2.dims() == 2);
    assert(p2.points() == 2);
   
    assert(!p2.addPoint({7,8}));
   }

   void moveTest() {

    std::cout << "point cloud move test..." << std::endl;

    PointCloud<int> pc1({{1,2},{3,4},{5,6}});
    PointCloud<int> pc2(std::move(pc1));
    
    assert(pc1.dims() == 0);
    assert(pc2.dims() == 2);
    assert(pc1.capacity() == 0);
    assert(pc2.capacity() == 3);
    assert(pc1.points() == 0);
    assert(pc2.points() == 3);
    assert(pc2.containsPoint({1,2}));
    assert(pc2.containsPoint({3,4}));
    assert(pc2.containsPoint({5,6}));
   }

   void moveAssignmentTest() {

    std::cout << "point cloud move assignment test..." << std::endl;
  
    PointCloud<int> pc1({{1,2},{3,4},{5,6}});
    PointCloud<int> pc2 = std::move(pc1);
    
    assert(pc1.dims() == 0);
    assert(pc2.dims() == 2);
    assert(pc1.capacity() == 0);
    assert(pc2.capacity() == 3);
    assert(pc1.points() == 0);
    assert(pc2.points() == 3);
    assert(pc2.containsPoint({1,2}));
    assert(pc2.containsPoint({3,4}));
    assert(pc2.containsPoint({5,6}));
   }

   void bruteForceNearestNeighborTest() {

    std::cout << "Point Cloud nearest neighbor test..." << std::endl;

    Point<int> p = std::get<0>(p2.queryNearestNeighbor({7,8}));
    assert(p == Point<int>({3,4}));
   }

   void insertTest() {
   
    std::cout << "PointCloud Insert Test" << std::endl;    

    p1.addPoint({1,2,3,4,5});
    assert(p1.containsPoint({1,2,3,4,5}));
    assert(!p1.containsPoint({2,3,4,5,6}));
   }

   PointCloud<int> p1 = PointCloud<int>(100,5);
   PointCloud<int> p2 = PointCloud<int>({{1,2},{3,4}});

 }; // class pointcloud test

} // namespace rossb83 

#endif // ROSSB83_POINT_HPP
