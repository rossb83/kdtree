#ifndef ROSSB83_DOT_FILE_WRITER_HPP
#define ROSSB83_DOT_FILE_WRITER_HPP

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

#include "kdtree.hpp"

// ben's namespace
namespace rossb83 {

/*
 * class that serializes kdtree into files to be persisted on disk, added bonus is these files
 * can be open and viewed in GraphViz! (www.graphviz.org), for an online viewer check
 * www.webgraphviz.com if the dot file < 100 lines
 */
template <typename T>
class DotFileWriter {

    typedef std::pair<Point<T>,int> PointDimPair;

    public:

    /*
     * attach a filename
     */
    DotFileWriter(const std::string& filename) {

        filename_ = filename;
    }

    /*
     * iterates through a kdtree in level-order storing its contents in dot file format
     */
    void writeFile(const KDTree<T>& kdtree) const {

        std::ofstream file(filename_);
        int nodelabel = 0;

        file << "digraph BST {" << std::endl << std::endl;
        file << "\tnode [fontname=\"Arial\"]" << std::endl << std::endl;

        for (PointDimPair p : kdtree) { // level-order kdtree iteration

            if (p.first != Point<T>()) { // write node to dot file
               
                // text to represent node
                file << "\t" << nodelabel++ << " /*" << p.first.label() << "*/";
                file << " [label=\"" << p.first << "@" << p.second << "\"];" << std::endl;
            } else { // write null to dot file

                // text to represent nullptr
                file << "\t" << std::to_string(nodelabel++) << " [shape=point];" << std::endl;
            }
        }

        file << std::endl;

        // since a kdtree is a complete binary tree, the indices of a parent's children
        // have an easy formula that is also used for heaps
        for (int i = 0; i <= nodelabel/2 - 1; i++) {

            file << "\t" << i << " -> " << 2*i+1 << std::endl;
            file << "\t" << i << " -> " << 2*i+2 << std::endl;
        }

        file << "}";
        file.close();
    }

    /*
     * filename of file to store on disk
     */
    std::string filename_;

}; // class DotFileWriter

} // namespace rossb8

#endif
