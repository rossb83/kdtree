#ifndef ROSSB83_KDTREE_HPP
#define ROSSB83_KDTREE_HPP

#include <unordered_set>
#include <set>
#include <string>
#include <list>
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
#include <cmath>
#include <typeinfo>

#include "PointCloud.hpp"
#include "SplitPointSortStrategy.hpp"
#include "SplitAxisRoundRobinStrategy.hpp"
#include "DotFileReader.hpp"

// ben's namespace
namespace rossb83 {

/*
 * datastructure that stores k-dimensional points and allows fast query of nearest neighbors
 */
template<typename T>
class KDTree {

    class KDNode;
    
    typedef std::shared_ptr<SplitPointStrategy<T>> SplitPointStrategyPtr;
    typedef std::shared_ptr<SplitAxisStrategy<T>> SplitAxisStrategyPtr;
    typedef std::shared_ptr<KDNode> KDNodePtr;
    typedef std::pair<Point<T>,int> PointDimPair;
 
    public:

    /*
     * builds kdtree given a list of points
     * input pointcloud - list of points to put in kdtree
     * input splitPointStrategy - decision algorithm to find median of input list of points and choose point to split on
     * input splitAxisStrategy - decision algorithm to find axis to split on
     */
    KDTree(PointCloud<T> pointCloud,const SplitPointStrategyPtr splitPointStrategy,const SplitAxisStrategyPtr splitAxisStrategy)
        : splitPointStrategy_(splitPointStrategy), splitAxisStrategy_(splitAxisStrategy) {

        // input points are stored in a pointcloud, however a vector would be more convenient for median finding and processing
        std::vector<Point<T>> points;
        points.reserve(pointCloud.points());
    
        // http://cpptruths.blogspot.com/2013/10/moving-elements-from-stl-containers-and.html
        for(const auto& point : pointCloud) {
           // PointCloud is backed by associative container, must move elements one by one
           points.push_back(std::move(const_cast<Point<T>&>(point))); // ugly hack
        }
    
        // build kdtree and assign root
        BuildKDTree(points);
    }

    /*  
     * builds a kdtree from a pcd file
     */
    KDTree(PCDFile<T>& pcdfile, const SplitPointStrategyPtr splitPointStrategy,const SplitAxisStrategyPtr splitAxisStrategy) : 
        KDTree(PointCloud<T>(pcdfile),splitPointStrategy,splitAxisStrategy) {}

    /*
     * builds a kdtree from a pcd file with default strategies
     */
    KDTree(PCDFile<T>& pcdfile) : KDTree(
        PointCloud<T>(pcdfile),
        std::make_shared<SplitPointSortStrategy<T>>(),
        std::make_shared<SplitAxisRoundRobinStrategy<T>>()) {}

    /*
     * builds a kdtree from an initializer list
     */
    KDTree(const std::initializer_list<Point<T>>& vals) : 
        KDTree(vals,std::make_shared<SplitPointSortStrategy<T>>(),std::make_shared<SplitAxisRoundRobinStrategy<T>>()) {} 

    /*
     * builds a kd tree given a graphviz dotfile
     * input dotfile - handle to filestream containing a serialized pointcloud
     */
    KDTree(DotFileReader<T>& dotfile) {

        // get root from dot file
        auto it = dotfile.begin();
        root = buildNode(*it);

        // enqueue root
        std::queue<KDNodePtr> q;
        if (root) q.push(root);

        // build kdtree in level-order
        while(!q.empty()) {
       
            // current node
            KDNodePtr temp = q.front();
            q.pop();

            // children nodes
            temp->left_ = buildNode(*++it);
            temp->right_ = buildNode(*++it);

            // enqueue children
            if (temp->left_) q.push(temp->left_);
            if (temp->right_) q.push(temp->right_);
        }        
    }

    // don't allow copying a kdtree to a new instance (well there is still a sneaky way to do it...)
    // kdtrees could potentially be huge, one is enough... until the design review
    KDTree(KDTree& other) = delete;

    /*
     * move a kdtree instance
     */
    KDTree(KDTree&& other) {
   
       // moving shared pointers will not effect internal reference counters 
       // other's shared pointers will now all be null
       this->root = std::move(other.root);
       // TODO: move strategies as well
    }

    /**
     * begin iterator to walk tree in level order
     */
    auto begin() {

        KDNodePtr temp = root;
        
        
        return LevelorderIterator<std::pair<Point<T>,int>>(temp);
    }

    auto begin() const {
        
        KDNodePtr temp = root;
        return LevelorderIterator<std::pair<Point<T>,int>>(temp);
    }

    /**
     * end iterator to signify all nodes in kdtree have been visited
     */
    auto end() {return LevelorderIterator<std::pair<Point<T>,int>>();}

    auto end() const {return LevelorderIterator<std::pair<Point<T>,int>>();}

    /*
     * queries tree for nearest neighbor of input point
     * input queryPoint - point to search for nearest neighbor of
     * output Point - point in kdtree that is closest to input point
     *
     * this function works by iteratively performing a "modified" inorder dfs
     * the modification is that normally inorder searches leftChild->parent->rightChild
     * instead we search "most likely child" first (either left or right depending on heuristic)
     * then we search parent, and finally we either skip "least likely child" and in effect
     * "prune" the tree, or search "least likely child" (if heuristic is met)
     */
    std::tuple<Point<T>, double, std::size_t> queryNearestNeighbor(const Point<T>& queryPoint) const {

        // initialize nearest neighbor/distance as empty point at distance infinity
        Point<T> nearestNeighbor = Point<T>();
        double nearestDistance = std::numeric_limits<double>::max();
        std::size_t numnodesvisited = 0;

        // state variables for iterative "modified" inorder traversal
        auto current = root;
        std::stack<KDNodePtr> s;

        // lambda to explore the next node that lies on the same side of the axis as the query point
        auto traverseBestPath = [&queryPoint](KDNodePtr p) {   
            return ((queryPoint[p->dim_] < p->point_[p->dim_]) ? p->left_ : p->right_);
        };  

        // lambda to explore the next node that lies on the opposite side of the axis as the query point
        auto traverseWorstPath = [&queryPoint](KDNodePtr p) {
            return ((queryPoint[p->dim_] < p->point_[p->dim_]) ? p->right_ : p->left_);
        };  

        // lambda to decide to prune tree branch iff the hypersphere around the query point intersects the axis hyperplane!!
        auto pruneTree = [&queryPoint, &nearestNeighbor, &nearestDistance](const KDNodePtr p) {
            return (std::norm(queryPoint[p->dim_] - p->point_[p->dim_]) > nearestDistance) ? true : false;
        };  

        // lambda to update nearest neighbor
        auto updateNearestNeighbor = [&queryPoint, &nearestNeighbor, &nearestDistance](const Point<T> p) {
 
            double queryDistance = std::norm(queryPoint - p);
            
            if(queryDistance < nearestDistance) {
                
                nearestDistance = queryDistance;
                nearestNeighbor = p;
            }
        };  

        // push current node on stack and traverse best path
        if (current) {

            s.push(current);
            current = traverseBestPath(current);
        }

        while (!s.empty()) { // explore every non-pruned node in tree

            if (current) { // continue exploring "best" child
   
                // push current node on stack and traverse "best" path
                s.push(current);
                current = traverseBestPath(current);
    
            } else { // reached a leaf, unwind stack

                // visit node
                KDNodePtr temp = s.top();
                s.pop();
                numnodesvisited++;

                // check if we found a new nearest neighbor, hope to check only O(lgn) times
                updateNearestNeighbor(temp->point_);

                // optimization: try to save a lot of time by pruning tree and not exploring other child
                if(!pruneTree(temp) && (temp = traverseWorstPath(temp))) {
      
                    s.push(temp);
                    current = traverseBestPath(temp);
                } // end if
            } // end else
        } // end while

        return std::make_tuple(std::move(nearestNeighbor), sqrt(nearestDistance), numnodesvisited);

    } // end function queryNearestNeighbor

    /*
     * compares two kdtrees for inequality
     */
    bool operator!=(const KDTree& other) const {

        return !(this->operator==(other));
    }

    /*
     * compares two kdtrees for equality
     */
    bool operator==(const KDTree& other) const {
    
        // roots of trees
        KDNodePtr rootA = this->root;
        KDNodePtr rootB = other.root;

        // enqueue both trees
        std::queue<KDNodePtr> qA;
        std::queue<KDNodePtr> qB;

        if (rootA) qA.push(rootA);
        if (rootB) qB.push(rootB);

        // iterate through trees in level order
        while (!qA.empty() && !qB.empty()) {

            // examine nodes in level order
            KDNodePtr tempA = qA.front();
            KDNodePtr tempB = qB.front();
            qA.pop();
            qB.pop();

            // check for equality
            if ((tempA->point_ != tempB->point_) || (tempA->dim_ != tempB->dim_)) { 
                return false;
            }

            if (!tempA->left_ && tempB->left_) return false;
            if (!tempB->right_ && tempB->right_) return false;

            // enqueue nodes
            if (tempA->left_) qA.push(tempA->left_);
            if (tempA->right_) qA.push(tempA->right_);
            if (tempB->left_) qB.push(tempB->left_);
            if (tempB->right_) qB.push(tempB->right_);
        }

        // make sure both trees are same length
        return (qA.empty() != qB.empty()) ? false : true;
    }

    private:

    /*
     * helper function to construct kd tree given a list of points
     * input points - list of points to move into kdtree
     */
    void BuildKDTree(std::vector<Point<T>>& points) {
        
        // magic numbers used in this method for tuple access
        const static std::size_t NODE = 0;
        const static std::size_t START = 1;
        const static std::size_t STOP = 2;

        // maintain stack inputs to build tree in preorder manner
        std::queue<std::tuple<KDNodePtr, int, int>> q;

        // build root node
        int start = 0;
        int stop = points.size() - 1;
        KDNodePtr temp = root = buildNode(points,start,stop);

        if (root) q.push(std::make_tuple(root,start,stop));

        while(!q.empty()) { // iterate until every input point is processed
          
            // extract data from queue
            temp = std::get<NODE>(q.front());
            start = std::get<START>(q.front());
            stop = std::get<STOP>(q.front());
            q.pop();
            
            // build child nodes
            int mid = std::ceil((start + stop)/2.0);
            temp->left_ = buildNode(points, start, mid - 1);
            temp->right_ = buildNode(points, mid + 1, stop);
            // push data onto "stack"
            if (temp->left_) q.push(std::make_tuple(temp->left_, start, mid - 1));
            if (temp->right_) q.push(std::make_tuple(temp->right_, mid + 1, stop));
        }
    } 

    /*
     * helper nested class - node to store data in KDTree
     */
    class KDNode {

        friend class KDTree;

        public:

        /*
         * build node
         * input point - k dimensional point to store in node
         * input dim - dimension to consider splitting on for children of this node
         */
        KDNode(Point<T> point, std::size_t dim) :
            point_(point), dim_(dim), left_(nullptr), right_(nullptr) {}

        /*
         * streams a kdnode in format point@dimension
         * input ss - ostream to output serialized node
         */
        friend std::ostream& operator<<(std::ostream& ss, const KDNodePtr kdnode) {
   
            // serialize kdnode into stream
            ss << kdnode->point_;
            ss << "@";
            ss << kdnode->dim_;

            return ss;
        }

        private:

        /*
         * left child, less than parent node @dim
         */
        std::shared_ptr<KDNode> left_;

        /*
         * right child, greater than parent node @dim
         */
        std::shared_ptr<KDNode> right_;

        /*
         * k dimensional point stored at this node
         */
        Point<T> point_;

        /*
         * dimension to compare left/right child points to
         */
        std::size_t dim_;

    }; // class KDNode

    /*
     *  helper function to build node (used for in-memory pointcloud container)
     *  input points - point vector to be move into tree
     *  input start - inclusive index of point vector to start selection of split point/axis
     *  input stop - inclusive index of point vector to stop selection of split point/axis
     */
    KDNodePtr buildNode(std::vector<Point<T>>& points, const int& start, const int& stop) {
        
        // base case, we have passed a leaf
        if (start > stop) return nullptr;

        // decide axis and point to split on
        int splitAxis = splitAxisStrategy_->splitAxis(points, start, stop); 
        Point<T> splitPoint = splitPointStrategy_->splitPoint(points, splitAxis, start, stop);
        return std::make_shared<KDNode>(std::move(splitPoint), splitAxis);
    }

    /*
     * helper function to build node (used for dot file deserialization)
     * input p - point/dimension to be moved into node
     */
    KDNodePtr buildNode(const PointDimPair& p) {
        return (p.first == Point<T>()) ? nullptr : std::make_shared<KDNode>(std::move(p.first),p.second);
    }

    /*
     * strategy to determine median of input points for insertion
     */
    const SplitPointStrategyPtr splitPointStrategy_;

    /*
     * strategy to determine axis of input points for insertion
     */
    const SplitAxisStrategyPtr splitAxisStrategy_;

    /*
     * root of kdtree
     */
    KDNodePtr root;

    /*
     * nested iterator class to walk kdtree nodes in level order
     */
    template <typename PointDimPair = std::pair<Point<T>,int>>
    class LevelorderIterator {

        public:
    
        /**
         * returns empty iterator signaling end iterator
         */ 
        LevelorderIterator() {}

        /*
         * returns initialized values in iterator signaling begin iterator
         * input root - node to begin preorder traversal
         */
        LevelorderIterator(KDNodePtr root) {
 
            // initialize iteration
            q_.push(root);
            ++*this;
        }

        /*
         * overloaded deref operator that returns current point/dim given iterator
         */
        const PointDimPair& operator*() const {return pointDimPair_; }

        LevelorderIterator<PointDimPair>& operator++() {

            prevSize = q_.size();

            if (!q_.empty()) {
            
                KDNodePtr temp = q_.front();
                q_.pop();
 
                if (temp) q_.push(temp->left_);
                if (temp) q_.push(temp->right_);
                pointDimPair_ = temp ? std::make_pair(temp->point_, temp->dim_) : std::make_pair(Point<T>(), std::size_t(0));
            }            

            return *this;
        }

        bool operator!=(const LevelorderIterator<PointDimPair>& other) const {
            
            return (prevSize != other.q_.size());
        }

        private:
   
        std::queue<KDNodePtr> q_;
        PointDimPair pointDimPair_;
        int prevSize = 0;
        
    }; // class iterator

}; // class KDTree

} // namespace rossb83

#endif
