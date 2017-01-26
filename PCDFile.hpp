#ifndef ROSSB83_PCD_FILE_HPP
#define ROSSB83_PCD_FILE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace rossb83 {

 template<typename T>
 class PCDFile {

  public:

   PCDFile(std::string filename) {

    filename_ = filename;
    countDims();
    countPoints();
   }

   size_t dims() const {return dims_;}
   size_t points() const {return points_;}

   auto begin() {
   
    return LineInputIterator<Point<T>>(filename_, dims_);
   }

   auto end() const {
   
    return LineInputIterator<Point<T>>();
   }

  private:

   void countPoints() {

    std::ifstream file(filename_);
 
    // make newline characters visible
    file.unsetf(std::ios_base::skipws);

    // count lines
    points_ = std::count(
     std::istream_iterator<char>(file),
     std::istream_iterator<char>(),
     '\n');

   }

   void countDims() {

    std::ifstream file(filename_);
 
    std::string row;
    std::getline(file,row);
    dims_ = std::count(row.begin(),row.end(),',') + 1;
   }

   std::size_t dims_;
   std::size_t points_;
   std::string filename_;

   // custom iterator code below largely adapated from:
   // http://stackoverflow.com/questions/1567082/how-do-i-iterate-over-cin-line-by-line-in-c
   template <class PointT = Point<T>>
   class LineInputIterator {

   public:
    typedef typename std::string::value_type char_type;
    typedef typename std::string::traits_type traits_type;
    typedef std::basic_istream<char_type, traits_type> istream_type;

    LineInputIterator(): is_(0) {}

    LineInputIterator(std::string const& filename, std::size_t const&  dims) : 
        point_(Point<T>(dims)) {

        file_.open(filename);
        is_ = &file_;
        ++*this;
    }

    LineInputIterator(LineInputIterator&& in) {

        ;
    }

    const Point<T>& operator*() const { return point_; }

    const Point<T>* operator->() const { return &point_; }

    LineInputIterator<Point<T>>& operator++() {
     
     std::string line;
        
     if (is_ && !getline(*is_, line)) {
      is_ = NULL;
     }

     std::istringstream iss(line);

     iss >> point_;

     return *this;
    }
    
    LineInputIterator<Point<T>> operator++(int) {

     LineInputIterator<Point<T>> prev(*this);
     ++*this;
     return prev;
    }

    bool operator!=(const LineInputIterator<Point<T>>& other) const {
        
     return is_ != other.is_;
    }

    bool operator!=(const LineInputIterator<Point<T>>&& other) const {
        
     return is_ != other.is_;
    }
    
    bool operator==(const LineInputIterator<Point<T>>& other) const {
        
     return !(*this != other);
    }

    bool operator==(const LineInputIterator<Point<T>>&& other) const {
        
     return !(*this != other);
    }

   private:
    std::ifstream file_;
    istream_type* is_;
    Point<T> point_;
  };
 }; // class PCDFile
} // namespace rossb83

#endif
