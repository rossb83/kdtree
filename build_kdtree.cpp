#include <locale>

#include "Point.hpp"
#include "kdtree.hpp"
#include "DotFileWriter.hpp"
#include "PCDFile.hpp"
#include "SplitAxisStrategyFactory.hpp"
#include "SplitPointStrategyFactory.hpp"

#include <string>

using namespace rossb83;

/*
 * quick script that builds a kdtree and stores it to a specified file
 */
int main(int argc, char* argv[]) {

    static const std::string INPUT_FILE = "inputfile";
    static const std::string OUTPUT_FILE = "outputfile";
    static const std::string SPLIT_POINT = "splitpoint";
    static const std::string SPLIT_AXIS = "splitaxis";

    std::unordered_map<std::string,std::string> inputs;
    inputs.insert({{INPUT_FILE,"sample_data.csv"},{OUTPUT_FILE,"sample_kdtree.dot"},{SPLIT_POINT,"sort"},{SPLIT_AXIS,"cycle"}});

    for (size_t i = 1; i < argc; i++) {

        std::string input = std::string(argv[i]);
        int delimiter = input.find("=");

        if ((argv[i][0] == '-') && (delimiter != std::string::npos)) {

            inputs[input.substr(1,delimiter-1)] = input.substr(delimiter+1);
        }
    }

    std::cout << "Reading point data input file: " << inputs[INPUT_FILE] << std::endl;

    // read input file from disk
    PCDFile<double> inputfile(inputs[INPUT_FILE]);

    std::cout << "Building kdtree with: " << std::endl;
    std::cout << "\tSplit Axis Strategy: " << inputs[SPLIT_AXIS] << std::endl;
    std::cout << "\tSplit Point Strategy: " << inputs[SPLIT_POINT] << std::endl;

    // generate strategies to create kdtree
    std::shared_ptr<SplitAxisStrategy<double>> splitAxisStrategy = SplitAxisStrategyFactory<double>::createSplitAxisStrategy(inputs[SPLIT_AXIS]);
    std::shared_ptr<SplitPointStrategy<double>> splitPointStrategy = SplitPointStrategyFactory<double>::createSplitPointStrategy(inputs[SPLIT_POINT]);

    // generate kdtree from input file
    KDTree<double> kdtree(inputfile, splitPointStrategy, splitAxisStrategy);

    std::cout << "Serializing kdtree to output file: " << inputs[OUTPUT_FILE] << std::endl;
    
    // serialize kdtree and store on disk
    DotFileWriter<double> dotfile(inputs[OUTPUT_FILE]);
    dotfile.writeFile(kdtree);

    return 0;
}
