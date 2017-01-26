#ifndef ROSSB83_POINTCLOUD_HPP
#define ROSSB83_POINTCLOUD_HPP

#include <unordered_set>
#include <limits>
#include <tuple>
#include <cmath>

#include "PCDFile.hpp"
#include "Point.hpp"

namespace rossb83 {

 template<typename T>
 class PointCloud {

  public:
   PointCloud(const std::size_t& capacity, const std::size_t& dims) {
   
    data_.reserve(10*capacity);
    capacity_ = capacity;
    dims_ = dims;
    points_ = 0;
   }

    /*
     * create a pointcloud with input from a pcdfile
     */ 
    PointCloud(PCDFile<T>& pcdfile) : PointCloud(pcdfile.points(), pcdfile.dims()) {
   
        int label = 0;
 
        for (Point<T> p : pcdfile) {
        
            p.label(std::to_string(label++));
            addPoint(std::move(p));
        }
    }

   PointCloud(const std::initializer_list<Point<T>>& vals) : 
    data_(vals), capacity_(vals.size()), points_(vals.size()) {
  
    dims_ = vals.begin()->dims();
    
    for(int i = 0; i < vals.size()-1; i++) {
 
     if ((vals.begin() + i)->dims() != (vals.begin() + i + 1)->dims()) {
     
      throw std::runtime_error("Point dimensions must match");
     }
    }
   }

   // don't allow copying a pointcloud to a new instance (well there is a sneaky way to do it...)
   // pointclouds could potentially be huge, one is enough... until the design review
   PointCloud(PointCloud& other) = delete;

   // moves a pointcloud to a new instance
   PointCloud(PointCloud&& other) {

    this->data_ = std::move(other.data_);
    this->capacity_ = other.capacity_;
    this->dims_ = other.dims_;
    this->points_ = other.points_;

    other.capacity_ = 0;
    other.dims_ = 0;
    other.points_ = 0;
   }

   // assigns this pointcloud's values to another pointcloud's values
   PointCloud<T>& operator=(PointCloud<T> rhs) {
    
    this->data_ = std::move(rhs.data_);
    this->capacity_ = rhs.capacity;
    this->dims_ = rhs.dims_;
    this->points_ = rhs.points_;

    rhs.capacity_ = 0;
    rhs.dims_ = 0;
    rhs.points_ = 0;

    return *this;
   }

   // inserts a point into the pointcloud
   bool addPoint(const Point<T>& p) {

    if (p.dims() != dims_) {
     throw std::runtime_error("Point dimensionality does not match");
    }

    if (points_ < capacity_) {

     points_++;
     data_.insert(p);
     return true;
    }

    return false;
   }

    std::tuple<Point<T>, double, std::size_t> queryNearestNeighbor(const Point<T>& queryPoint) {

        // initialize nearest neighbor/distance as empty point at distance infinity
        Point<T> nearestNeighbor = Point<T>();
        double nearestDistance = std::numeric_limits<double>::max();
        std::size_t numnodesvisited = 0;

        // lambda to update nearest neighbor
        auto updateNearestNeighbor = [&queryPoint, &nearestNeighbor, &nearestDistance](const Point<T> p) {
 
            double queryDistance = std::norm(queryPoint - p); 
    
            if(queryDistance < nearestDistance) {
    
                nearestDistance = queryDistance;
                nearestNeighbor = p;
            }   
        };  

        for (Point<T> p : data_) {
        
            updateNearestNeighbor(p);
            numnodesvisited++;
        }  

        return std::make_tuple(nearestNeighbor, std::sqrt(nearestDistance), numnodesvisited);
    }

   bool containsPoint(const Point<T>& p) {
   
    return (data_.find(p) != data_.end());
   }

   // const begin iterator
   typename std::unordered_set<Point<T>>::const_iterator begin() const {return data_.begin();}
 
   // begin iterator
   typename std::unordered_set<Point<T>>::iterator begin() {return data_.begin();}
 
   // const end iterator
   typename std::unordered_set<Point<T>>::const_iterator end() const {return data_.end();}
 
   // non-const end iterator
   typename std::unordered_set<Point<T>>::iterator end() {return data_.end();}

   std::size_t dims() const {return dims_;}

   std::size_t points() const {return points_;}

   std::size_t capacity() const {return capacity_;}

  private:
   std::size_t dims_;
   std::size_t points_;
   std::size_t capacity_;
   std::unordered_set<Point<T>> data_;

 }; // class pointcloud

} // namespace rossb83

#endif // ROSSB83_POINTCLOUD_HPP
