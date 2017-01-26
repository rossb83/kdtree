#ifndef ROSSB83_KDTREE_TEST_HPP
#define ROSSB83_KDTREE_TEST_HPP

#include "kdtree.hpp"
#include "DotFileWriter.hpp"
#include "DotFileReader.hpp"
#include "SplitAxisRangeStrategy.hpp"

namespace rossb83 {

 class KDTreeTest {

  public:
  
   KDTreeTest() {
  
       createTest();
       ReadWriteIntegrationTest();
       equalityTest();
       inequalityTest();
       queryNearestNeighborTest();
       nearestNeighborIntegrationTest();
   }

  private:

   void ReadWriteIntegrationTest() {
   
       std::cout << "kdtree read/write integration test..." << std::endl;

       DotFileWriter<double> dotfilewriter("tree.dot");
       dotfilewriter.writeFile(kdtree);

       DotFileReader<double> dotfilereader("tree.dot");

       KDTree<double> kdtree_integ(dotfilereader);
       
       assert(kdtree == kdtree_integ);
   }

   void equalityTest() {

       std::cout << "kdtree equality test..." << std::endl;
   
       KDTree<double> kdtree1 = {{1,2},{3,4},{5,6}};
       KDTree<double> kdtree2 = {{1,2},{3,4},{5,6}};

       assert(kdtree1 == kdtree2);
   }

   void inequalityTest() {

       std::cout << "kdtree inequality test..." << std::endl;
   
       KDTree<double> kdtree1 = {{1,2},{7,4},{5,6}};
       KDTree<double> kdtree2 = {{1,2},{3,4},{5,6}};

       assert(kdtree1 != kdtree2);

       KDTree<double> kdtree3 = {{1,2},{7,4},{5,6},{9,10}};

       assert(kdtree1 != kdtree3);
   }

    void nearestNeighborIntegrationTest() {

        std::cout << "LONG TEST ALERT... kdtree nearest neighbor integration test..." << std::endl;

        PCDFile<double> pcd1("sample_data.csv");
        PCDFile<double> pcd2("query_data.csv");

        PointCloud<double> queryPointCloud(pcd2);

        PointCloud<double> samplePointCloud(pcd1);
        KDTree<double> sampleKDTree(pcd1);

        DotFileWriter<double> dotfilewriter("tree.dot");
        dotfilewriter.writeFile(sampleKDTree);

        DotFileReader<double> dotfilereader("tree.dot");
        KDTree<double> sampleKDTreeRead(dotfilereader);

        for (Point<double> queryPoint : queryPointCloud) {

            std::tuple<Point<double>, double, int> t1 = samplePointCloud.queryNearestNeighbor(queryPoint);
            std::tuple<Point<double>, double, int> t2 = sampleKDTreeRead.queryNearestNeighbor(queryPoint);

            for (int i = 0; i < queryPoint.dims(); i++) {
            
                assert(std::abs(std::get<0>(t1)[i] - std::get<0>(t2)[i]));
            }
           
            assert(std::get<0>(t1).label() == std::get<0>(t2).label());
            assert(std::abs(std::get<1>(t1) - std::get<1>(t2)) < epsilon);
            assert(std::get<2>(t1) > std::get<2>(t2));
        }
    }

    void queryNearestNeighborTest() {

        std::cout << "kdtree query nearest neighbor test" << std::endl;
    
        std::tuple<Point<double>,double,int> nearest = kdtree.queryNearestNeighbor({3,4});
        assert(std::get<0>(nearest) == Point<double>({3,4}));
        assert(std::get<1>(nearest) == 0);

        nearest = kdtree.queryNearestNeighbor({1,2});
        assert(std::get<0>(nearest) == Point<double>({1,2}));
        assert(std::get<1>(nearest) == 0);

        nearest = kdtree.queryNearestNeighbor({5,6});
        assert(std::get<0>(nearest) == Point<double>({5,6}));
        assert(std::get<1>(nearest) == 0);

        nearest = kdtree.queryNearestNeighbor({5.1,6});
        assert(std::get<0>(nearest) == Point<double>({5,6}));
        assert(std::abs(std::get<1>(nearest) - 0.1) < epsilon);
    }

    void createTest() {

        std::cout << "kdtree create test" << std::endl;

        int i = 0;
        std::vector<Point<double>> points = {{3,4},{1,2},{5,6},{},{},{},{}};
        //std::vector<int> dims = {0, 1, 1, 0, 0, 0, 0}; dims for round robin axis split strategy
        std::vector<int> dims = {0,0,0,0,0,0,0};
    
        for (std::pair<Point<double>, int> p : kdtree) {
        
            assert(p.first == points[i]);
            assert(p.second == dims[i++]);
        }
    }   

    std::shared_ptr<SplitPointStrategy<double>> splitPointStrategy = 
     std::make_shared<SplitPointSortStrategy<double>>();

    std::shared_ptr<SplitAxisStrategy<double>> splitAxisStrategy =
     std::make_shared<SplitAxisRangeStrategy<double>>();

    PointCloud<double> pointCloud = {{1,2},{3,4},{5,6}};

    KDTree<double> kdtree = KDTree<double>(
     std::move(pointCloud),
     splitPointStrategy,
     splitAxisStrategy);

    double epsilon = 0.000001;

 }; // class KDTreeTest

} // namespace rossb83

#endif
