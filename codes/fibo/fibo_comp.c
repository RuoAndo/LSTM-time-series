//
//  Fibonacci Algorithm
//
//  Created by Alberto Pasca on 25/02/14.
//  Copyright (c) 2014 albertopasca.it. All rights reserved.
//

#include <iostream>
#include <chrono>


// +---------------------------------------------------------------------------+
#pragma mark - Fibonacci Numbers by Recursion
// +---------------------------------------------------------------------------+


long fib_r (int n)
{
  if (n == 0) return(0);
  if (n == 1) return(1);

  return(fib_r(n-1) + fib_r(n-2));
}


// +---------------------------------------------------------------------------+
#pragma mark - Fibonacci Numbers by Caching
// +---------------------------------------------------------------------------+


#define MAXN    1500    /* largest interesting n */
#define UNKNOWN -1      /* contents denote an empty cell */
long f[MAXN+1];         /* array for caching computed fib values */

long fib_c (int n)
{
  if ( f[n] == UNKNOWN )
    f[n] = fib_c(n-1) + fib_c(n-2);

  return (f[n]);
}

long fib_c_driver(int n)
{
  int i;

  f[0] = 0;
  f[1] = 1;

  for (i=2; i<=n; i++)
    f[i] = UNKNOWN;

  return(fib_c(n));
}


// +---------------------------------------------------------------------------+
#pragma mark - Fibonacci Numbers by Dynamic Programming
// +---------------------------------------------------------------------------+


long fib_dp(int n)
{
  int  i=2;         /* counter */
  long f[MAXN+1];   /* array to cache computed fib values */

  f[0] = 0;
  f[1] = 1;

  for ( ; i<=n; i++ )
    f[i] = f[i-1] + f[i-2];

  return f[n];
}


// +---------------------------------------------------------------------------+
#pragma mark - Fibonacci Numbers by Dynamic Programming, optimized
// +---------------------------------------------------------------------------+


long fib_ultimate(int n)
{
  int i;                        // counter
  long back2 = 0, back1 = 1;    // last two values of f[n]
  long next;                    // placeholder for sum
  
  if ( n == 0 ) return 0;

  for ( i=2; i<n; i++ )
  {
    next  = back1 + back2;
    back2 = back1;
    back1 = next;
  }
  
  return back1 + back2;
}


// +---------------------------------------------------------------------------+
#pragma mark - Main
// +---------------------------------------------------------------------------+

int main(int argc, const char * argv[])
{
  auto start = std::chrono::high_resolution_clock::now();
  fib_r(20);
  auto finish = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "\tns\n";

  start = std::chrono::high_resolution_clock::now();
  fib_c_driver(400);
  finish = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "\tns\n";

  start = std::chrono::high_resolution_clock::now();
  fib_dp(700);
  finish = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "\tns\n";

  start = std::chrono::high_resolution_clock::now();
  fib_ultimate(7000);
  finish = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "\tns\n";


  return 0;
}
