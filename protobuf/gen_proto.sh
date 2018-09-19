#!/bin/bash

rm -rf cpp
mkdir cpp

proto_exec=protoc
proto_path=./proto/
proto_files=`find $proto_path -name "*proto"`
proto_cpp_out=./cpp

$proto_exec --proto_path=$proto_path --cpp_out=$proto_cpp_out $proto_files
