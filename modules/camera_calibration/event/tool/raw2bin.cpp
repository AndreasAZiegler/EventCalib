//
// Created by huangkun on 2020/9/18.
//

#include <iostream>

#include <opengv2/event/EventStream.hpp>

using namespace std;
using namespace opengv2;

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << endl
             << "Usage: ./raw2bin rawFilePath"
             << endl;
        return 1;
    }

    EventStream::raw2bin(string(argv[1]));

    return 0;
}
