#ifndef ROSSB83_DOT_FILE_READER_HPP
#define ROSSB83_DOT_FILE_READER_HPP

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
#include <unordered_set>
#include <tuple>
#include <regex>
#include <functional>
#include <algorithm>
#include <numeric>
#include <limits>
#include <math.h>
#include <complex>
#include <cmath>
// #include <regex> -> this doesn't seem to work, TODO: investigate https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53631

#include "kdtree.hpp"

// ben's namespace
namespace rossb83 {

/*
 * class that deserializes a dot file persisted on disk to an in-memory kdtree
 */
template <typename T>
class DotFileReader {

    typedef std::pair<Point<T>,std::size_t> PointDimPair;

    public:

    /*
     * ctor open file with filename
     * input filename - file to be read from
     */
    DotFileReader(const std::string& filename) {

        file_.open(filename);
    }

    /*
     * dtor closes file
     */
    ~DotFileReader() {file_.close();}

    /*
     * begin iterator
     */
    auto begin() {return DotFileIterator<PointDimPair>(file_);}

    /*
     * end iterator
     */
    auto end() {return DotFileIterator<PointDimPair>();}

    /*
     * const begin iterator
     */
    auto begin() const {return DotFileIterator<PointDimPair>(file_);}

    /*
     * const end iterator
     */
    auto end() const {return DotFileIterator<PointDimPair>();}

    /*
     * iteraters through dotfile extracting the points
     */
    template <typename PointDimPair = std::pair<Point<T>,int>>
    class DotFileIterator {

        public:

        typedef typename std::string::value_type char_type;
        typedef typename std::string::traits_type traits_type;
        typedef std::basic_istream<char_type, traits_type> istream_type;

        /**
         * returns empty iterator signaling end iterator
         */
        DotFileIterator() : is_(nullptr) {}

        /*
         * returns initialized values in iterator signaling begin iterator
         * input root - node to begin preorder traversal
         */
        DotFileIterator(istream_type& is) : is_(&is) {++*this;}

        /*
         * overloaded deref operator that returns current point/dim given iterator
         */
        const PointDimPair& operator*() const {return pointDimPair_; }

        /*
         * iterate to next point in DOT file
         */
        DotFileIterator<PointDimPair>& operator++() {

            std::string line = "";

            while (is_ && !containsPoint(line)) { // continue to next line until point is found

                if (is_ && !getline(*is_, line)) { // check if end of file is reached
                 
                    is_ = nullptr; // no point found
                }
            }

            if (is_ != nullptr) { // ensure end of file is not reached yet

                pointDimPair_ = extractPoint(line);
            }

            return *this;
        }

        /*
         * called upon every iteration to ensure iteration is not complete
         */
        bool operator!=(const DotFileIterator<PointDimPair>& other) const {return (is_ != other.is_);}

        private:
  
        /*
         * istream iterator to read file line by line
         */
        istream_type* is_;
        
        /*
         * point/dimension read from input file
         */
        PointDimPair pointDimPair_;
 
        /*
         * helper function to determine if line from input file contains a point
         * TODO: refactor to use c++11 regexes
         * input line - line of text from input file
         * output true if line contains a point and false otherwise
         */
        static bool containsPoint(const std::string& line) {return (line.find("];") != std::string::npos);}

        /*
         * given a line of input text, return point and dimension
         * TODO: refactor to use c++11 regexes
         * input line - line of text from input file guaranteed to contain point
         * output deserialized point and dimension
         */
        static PointDimPair extractPoint(const std::string& line) {

            // this entire function should be refactored to use regex c++11 extension

            std::size_t pointstart = 0; // starting position of point data
        
            // regex "\"" indicates this line contains a null node
            if ((pointstart = line.find("\"")) == std::string::npos) {
            
                // empty point interpreted as null node
                return std::make_pair<Point<T>, std::size_t>(Point<T>(),0);
            
            } else {
               
                // very ugly code to extract point and dimension for line of text in DOT file
                
                std::string label = line.substr(line.find("*") + 1,pointstart - line.find("*") - 11);
                std::istringstream point(line.substr(pointstart + 2, line.find("@") - pointstart - 3));
                std::istringstream dim(line.substr(line.find("@") + 1, line.length() - line.find("@") - 4));
              
                // output point/dimension pair
                Point<T> p(std::count(line.begin(),line.end(),',') + 1); // input dimensions by counting commas in line
                std::size_t d;
               
                point >> p; // deserialize point
                p.label(std::move(label)); // add label to point

                dim >> d; // deserialize dimension
 
                return std::make_pair<Point<T>, std::size_t>(static_cast<Point<T>>(p),static_cast<size_t>(d));
            }
        }

    }; // class iterator

    private:

    /*  
     * file stream to be read
     */
    std::ifstream file_;
    
}; // class DotFileReader

} // namespace rossb83

#endif // ROSSB83_DOT_FILE_READER_HPP
