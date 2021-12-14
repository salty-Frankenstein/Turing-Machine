#include"shell.h"
using namespace std;

int main(int argc, char* argv[]) {
    Shell sh(argc, argv);

    sh.run();
    
    return 0;
}
