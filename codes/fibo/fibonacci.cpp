//
// fibonacci-fibonacci.cpp
//
#include <iostream>

using namespace std;

const int N_MAX = 20;
long x[N_MAX];

int main(int argc, char * argv[])
{
    for (long n=0; n<N_MAX; n++) {
        if (n<2) {
            x[n] = n;
        } else
            x[n] = x[n-1] + x[n-2];
        cout << x[n] << endl;
    }

    return 0;
}

