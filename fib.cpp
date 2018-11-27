/**
 * @file fib.cpp
 * Contains Fibonacci-number generating functions both utilizing and not
 * utilizing memoization.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "fib.h"
#include <map>

using std::map;

static map<unsigned long, unsigned long> memo;

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
unsigned long fib(unsigned long n)
{
    if (n < 2) {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0. This version utilizes memoization.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
unsigned long memoized_fib(unsigned long n)
{
    if (n < 2) {
        return 1;
    }

    auto iter = memo.find(n);
    if (iter != memo.end()) {
        return iter->second;
    }

    unsigned long res = memoized_fib(n - 1) + memoized_fib(n - 2);
    memo[n] = res;
    return res;
}
