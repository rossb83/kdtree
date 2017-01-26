#ifndef ROSSB83_PCDFILE_TEST_HPP
#define ROSSB83_PCDFILE_TEST_HPP

#include <assert.h>

#include "PCDFile.hpp"
#include "PointCloud.hpp"

namespace rossb83 {

 class PCDFileTest {

  public:

   PCDFileTest() {

    std::cout << "Running Point Cloud tests..." << std::endl;
   
    createTest();
    iteratorTest();
   }

  private:

   void createTest() {
   
    PCDFile<double> pcdfile("sample_data.csv"); 
    std::cout << "PCDFile create test..." << std::endl;    
   
    assert(pcdfile.dims() == 3);
    assert(pcdfile.points() == 1000);
   }

   void iteratorTest() {
 
    std::cout << "PCDFile iterator test..." << "\n";
 
    PCDFile<double> pcdfile("sample_data.csv");

    int pointCounter = 0;
    Point<double> prevPoint(3);

    for (Point<double> p : pcdfile) {
     assert(p.dims() == 3);
     assert(!(p == prevPoint));
     prevPoint = p;
     ++pointCounter;
    }

    assert(pointCounter == 1000);
   }

 }; // class pcdfiletest

} // namespace rossb83 

#endif // ROSSB83_PCD_FILE_TEST_HPP
