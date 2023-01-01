//
// recurfibo - recurfibo.cpp
//
#include <iostream>

using namespace std;

const int N_MAX = 30;

long Fibonacci( long n ) {
    if( n<2 )
        return n;
    else
        return Fibonacci(n-1) + Fibonacci(n-2);
}

int main(int argc, char * argv[])
{
    for (long i=0; i<N_MAX; i++) {
        long n = Fibonacci( i );
        cout << "n=" << n << endl;
    }

    return 0;
}

