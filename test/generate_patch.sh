
 g++ -fpic -shared -o libpatch.so patch.cpp -fno-access-control -std=c++11 -pthread -g -rdynamic -ldl -m64
