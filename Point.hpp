#ifndef ROSSB83_POINT_HPP
#define ROSSB83_POINT_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <tuple>
#include <regex>
#include <functional>
#include <algorithm>
#include <numeric>
#include <limits>
#include <math.h>
#include <complex>
#include <stdexcept>

namespace rossb83 {

 // point will hold k values to represent a position in k-dimensional space
 //
 // example: (1,2) is a point of dimension 2
 //
 // restrictions: currently the dimension of a point is constant
 //  and is not adjustable beyond the creation of the point instance
 template<typename T>
 class Point {

  public:

   // creates an empty point
   Point() : dims_(0) {}

   // creates a point at origin of specified dimensionality
   Point(const size_t& dims) {
    
    // dims value is authoratative for size
    dims_ = dims;
    data_.resize(dims_,0);
   }

   // creates a point of specified values
   Point(const std::initializer_list<T>& vals) : 
    data_(vals), dims_(vals.size()) {}

   // creates a point from r-value specified point
   Point(Point<T>&& other) {
    
    // other's data_ is now empty vector
    this->data_ = std::move(other.data_);

    // other's label is now empty string
    this->label_ = std::move(other.label_);
    
    // grab other's previous size and set it to zero
    this->dims_ = other.dims_;
    other.dims_ = other.data_.size();
   }

   // creates a point by copying values from another point
   Point(const Point<T>& other) {
   
    this->data_ = other.data_;
    this->dims_ = other.dims_;
    this->label_ = other.label_;
   }

   // getter - retrieve element of point at specified dimension
   T operator[](const std::size_t& i) const {
    
    // possible user error here, request value at non-existing dimension
    if (i >= dims_) {
     
     throw std::runtime_error(std::to_string(i) + " exceeds dimensionality");
    }

    // use const iterator
    return *(this->begin() +i);
   }

   // setter - update element of point at specified dimension
   T& operator[](const std::size_t& i) {
   
    // possible user error here, throw runtime error
    if (i >= dims_) {
    
     throw std::runtime_error(std::to_string(i) + " exceeds dimensionality");
    }

    // use non-const iterator
    return *(this->begin() + i);
   }

   // assigns this point's values to another point's values
   Point<T>& operator=(Point<T> point) {

    this->data_ = std::move(point.data_);
    this->label_ = std::move(point.label_);
    this->dims_ = point.dims_;
    point.dims_ = 0;
    return *this;
   }

   bool operator!=(const Point<T>& p) const {
   
    return !(*this == p);
   }

   bool operator==(const Point<T>& p) const {
  
    // points must be of same dimension to be considered equal 
    if (this->dims() != p.dims()) return false;

    for (int i = 0; i < p.dims(); i++) {
     if (this->operator[](i) != p[i]) return false;
    }

    return true;
   }   

   // subtracts points element by element, ie (3,4) - (1,2) = (2,2)
   friend Point<T> operator-(const Point<T>& lhs, const Point<T>& rhs) {

    // TODO: ? return empty point if dimensions of input do not match
    if (lhs.dims() != rhs.dims()) return Point();

    Point p(lhs.dims());
    std::transform(lhs.begin(),lhs.end(),rhs.begin(),p.begin(),std::minus<T>());

    return p;
   }

   // creates a point streamed in the format (p1,p2,p3,...,pN) or p1,p2,p3...pN
   friend std::istream& operator>>(std::istream& ss, Point<T>& p) {
 
    std::string data;
    std::size_t dim = 0;

    while(getline(ss,data,',')) {
    
     if (data.at(0) == '(') data = data.substr(1);
     if (data.at(data.size() - 1) == ')') data = data.substr(0,data.size() - 1);
    
     p[dim++] = static_cast<T>(std::stod(data));
    }
 
    return ss; 
   }

   // streams a point in format (p1,p2,p3,...,pN)
   friend std::ostream& operator<<(std::ostream& ss, const Point<T>& p) {
   
    ss << "(";

    for (size_t i = 0; i < p.dims(); i++) {
    
     ss << p[i];
     ss << ((i < (p.dims()-1)) ? "," : "");
    }
    
    ss << ")";
    return ss;
   }

   // const begin iterator
   typename std::vector<T>::const_iterator begin() const {return data_.begin();}

   // begin iterator
   typename std::vector<T>::iterator begin() {return data_.begin();}

   // const end iterator
   typename std::vector<T>::const_iterator end() const {return data_.end();}

   // non-const end iterator
   typename std::vector<T>::iterator end() {return data_.end();}
   
   // getter - dimensionality of point
   size_t dims() const {return dims_;}

   // setter - optional label
   void label(std::string label) {this->label_ = label;}

   // getter - optional label
   std::string label() const {return label_;}

  private:

   // optional label
   std::string label_;
  
   // each index in this dataype refers to a dimension in this point
   std::vector<T> data_;

   // the dimensionality of this point
   size_t dims_;
 
 }; // class Point

} // namespace rossb83

// add special definitions to std relating to Point
namespace std {

 // define the norm of a point as its inner product with itself
 template<typename T>
 double norm(const rossb83::Point<T>& p) {
  
    return inner_product(p.begin(), p.end(), p.begin(), 0.0);
 }

 template <typename T>
 struct hash<rossb83::Point<T>> {
   size_t operator()(const rossb83::Point<T>& p) const {

    // empty point always hashes to zero
    if (p.dims() == 0) return 0;

    // TODO: come up with better hash function, currently only taking
    // first element into account
    hash<T> h;
    return h(p[0]);
   }
 }; // class hash<Point>

} // namespace std


#endif // ROSSB83_POINT_HPP
