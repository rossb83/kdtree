#ifndef ROSSB83_POINT_TEST_HPP
#define ROSSB83_POINT_TEST_HPP

#include <assert.h>

#include "Point.hpp"

namespace rossb83 {

 class PointTest {

  public:

   PointTest() {

    std::cout << "Running Point tests..." << std::endl;
   
    createTest();
    createInitTest();
    moveTest();
    copyTest();
    assignmentCopyTest();
    assignmentMoveTest();
    updateTest();
    minusOperatorTest();
    equalityOperatorTest();
    ostreamOperatorTest();
    normTest();
    hashTest();
   }

  private:

   void createTest() {
    
    std::cout << "Point create test..." << std::endl;    
 
    assert(p1.dims() == 5);

    for (int val : p1) {
     assert(val == 0);
    }
   }   

   void createInitTest() {

    std::cout << "Point Init Test..." << std::endl;
   
    assert(p2[0] == 1);
    assert(p2[1] == 2);
    assert(p2[2] == 3);
   }

   void moveTest() {

    std::cout << "Point Move Test..." << std::endl;
   
    Point<int> a = {1,2,3};
    Point<int> b(std::move(a));

    assert(a.dims() == 0);
    assert(b.dims() == 3);

    assert(b[0] == 1);
    assert(b[1] == 2);
    assert(b[2] == 3);
   }

   void copyTest() {

    std::cout << "Point Copy Test..." << std::endl;
   
    Point<int> p(p2);
    assert(p.dims() == p2.dims());
    
    for (int i = 0; i < p.dims(); i++) {
     
     assert(p[i] == p2[i]);
    }
   }

   void assignmentCopyTest() {

    std::cout << "Point Assignment Copy Test..." << std::endl;
   
    Point<int> p = {7,8,9};
    p = p2;
    assert(p.dims() == p2.dims());
    
    for (int i = 0; i < p.dims(); i++) {

     assert(p[i] == p2[i]);
    }
   }

   void assignmentMoveTest() {

    std::cout << "Point Assignment Move Test..." << std::endl;
   
    Point<int> a = {1,2,3};
    Point<int> b = {4,5,6,7};
    
    a = std::move(b);
    
    assert(a.dims() == 4);
    assert(b.dims() == 0);

    assert(a[0] == 4);
    assert(a[1] == 5);
    assert(a[2] == 6);
    assert(a[3] == 7);
   }

   void updateTest() {

    std::cout << "Point Update Test..." << std::endl;
   
    Point<int> a = p2;
    a[0] = 4;
    a[1] = 5;
    a[2] = 6;

    assert(a[0] == 4);
    assert(a[1] == 5);
    assert(a[2] == 6);
   }

   void minusOperatorTest() {

    std::cout << "Point Minus Test..." << std::endl;
   
    Point<int> a(3);

    Point<int> p = a - p2;
    
    assert(p[0] == -1);
    assert(p[1] == -2);
    assert(p[2] == -3);
   }

   void equalityOperatorTest() {
  
    std::cout << "Point equality operator test..." << std::endl;
 
    assert(!(p1 == p2));
    assert(p1 == p1);
    assert(p2 == Point<int>({1,2,3}));
   }

   void ostreamOperatorTest() {
 
    std::cout << "Point ostream operator test..." << std::endl;
 
    std::ostringstream stream;
    stream << p2;
    std::string s = stream.str();
    assert(s == "(1,2,3)"); 
   }

   void normTest() {
  
    std::cout << "Point norm test..." << std::endl;
    assert(std::norm(p2) == 14);
   }

   void hashTest() {
    
    std::cout << "Point hash test..." << std::endl;
    std::hash<Point<double>> h;
    // interesting... point hashes to different values
    // in clang vs g++
    //assert(h({0.1337,2.384,100}) == 4593985070451970907);
   }

   const Point<int> p1 = Point<int>(5);
   const Point<int> p2 = Point<int>({1,2,3});

 }; // class point test

} // namespace rossb83 

#endif // ROSSB83_POINT_HPP
