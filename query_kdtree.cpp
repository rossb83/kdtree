#include "Point.hpp"
#include "kdtree.hpp"
#include "DotFileWriter.hpp"
#include "PCDFile.hpp"

using namespace rossb83;

/*
 * quick script that queries a serialized kdtree and outputs the nearest neighbors to a file
 */
int main(int argc, char* argv[]) {

    static const std::string KDTREE_FILE = "kdtreefile";
    static const std::string QUERY_FILE = "queryfile";
    static const std::string OUTPUT_FILE = "outputfile";

    std::unordered_map<std::string,std::string> inputs;
    inputs.insert({{KDTREE_FILE,"sample_kdtree.dot"},{QUERY_FILE,"query_data.csv"},{OUTPUT_FILE,"sample_query.csv"}});

    for (size_t i = 1; i < argc; i++) {

        std::string input = std::string(argv[i]);
        int delimiter = input.find("=");

        if ((argv[i][0] == '-') && (delimiter != std::string::npos)) {

            inputs[input.substr(1,delimiter-1)] = input.substr(delimiter+1);
        }
    }

    std::cout << "Reading query data input file: " << inputs[QUERY_FILE] << std::endl;

    // read input file from disk
    PCDFile<double> queryfile(inputs[QUERY_FILE]);

    std::cout << "Deserializing kdtree file: " << inputs[KDTREE_FILE] << std::endl;
    
    // generate kdtree from input file
    DotFileReader<double> dotfile(inputs[KDTREE_FILE]);
    KDTree<double> kdtree(dotfile);

    // create output file
    std::cout << "creating output file: " << inputs[OUTPUT_FILE] << std::endl;
    std::ofstream out(inputs[OUTPUT_FILE]);

    for (Point<double> p : queryfile) {

        // returns tuple where 1st element is the nearest neighbor, 2nd is the euclidean distance, and 3rd is the number of nodes in the tree visited
        std::tuple<Point<double>, double, int> nearestneighbor = kdtree.queryNearestNeighbor(p);
        out << std::get<0>(nearestneighbor).label() << "," << std::get<1>(nearestneighbor) << std::endl;
    }

    return 0;
}
