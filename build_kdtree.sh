#!/bin/sh

# this will build a kdtree from an input point cloud file
# -inputfile=sample_data.csv input pointcloud file
# -outputfile=sample_kdtree.dot ouptut serialized kdtree
# -splitpoint=select choose split point strategy, choices are either "select" or "sort"
# -splitaxis=cycle choose split axis strategy, choices are either "cycle" or "range"

#./build_kdtree -inputfile=sample_data.csv -outputfile=sample_kdtree.dot -splitpoint=select -splitaxis=range

#./build_kdtree -inputfile=sample_data.csv -outputfile=sample_kdtree.dot -splitpoint=select -splitaxis=cycle

#./build_kdtree -inputfile=sample_data.csv -outputfile=sample_kdtree.dot -splitpoint=sort -splitaxis=range

./build_kdtree -inputfile=sample_data.csv -outputfile=sample_kdtree.dot -splitpoint=sort -splitaxis=cycle
