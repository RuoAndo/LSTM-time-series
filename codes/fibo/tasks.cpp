//
//Å@tasks - tasks.cpp
//
#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/task.h"

using namespace std;
using namespace tbb;

const int N_MAX = 30;

long Fibonacci( long n ) {
    if( n<2 )
        return n;
    else
        return Fibonacci(n-1) + Fibonacci(n-2);
}

class FibTask: public task {
public:
    const long n;
    long* const fibon;
    FibTask( long n_, long* sum_ ) : n(n_), fibon(sum_) { }
    task* execute() {
        if( n < 2 ) {
            *fibon = n;
        } else {
            long x, y;
            FibTask& a = *new( allocate_child() ) FibTask(n-1, &x);
            FibTask& b = *new( allocate_child() ) FibTask(n-2, &y);
            set_ref_count(3);
            spawn( b );
            spawn_and_wait_for_all( a );
            *fibon = x + y;
        }
        return NULL;
    }
};

long ParallelFib( long n ) {
    long fibon;
    FibTask& a = *new(task::allocate_root()) FibTask(n,&fibon);
    task::spawn_root_and_wait(a);
    return fibon;
}

int main(int argc, char * argv[])
{
    task_scheduler_init init;

    for (long i=0; i<N_MAX; i++) {
        long n = ParallelFib( i );
        cout << "n=" << n << endl;
    }

    init.terminate();

    return 0;
}
