#!/bin/sh

# this will build a kdtree from an input point cloud file
# -kdtreefile=sample_kdtree.dot input serialized kdtree file
# -outputfile=sample_query.csv output file to store query data
# -queryfile=query_data.csv data to query kdtree with

./query_kdtree -kdtreefile=sample_kdtree.dot -queryfile=query_data.csv -outputfile=sample_query.csv
