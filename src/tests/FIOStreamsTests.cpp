#include <iostream>
#include <fstream>

using namespace std;

namespace tests {
    class FIOStreamsTests {
    public:
        void run() {
            ifstream inFile("/home/albert/Other/Tmp/test");
            char out[100];
            inFile >> out;
            inFile.close();
            cout << out;
        };
    };
}

