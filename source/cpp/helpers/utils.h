/*
 * utils.h
 *
 *  Created on: Dec 14, 2013
 *      Author: jancajthaml
 */

#ifndef UTILS_H_
#define UTILS_H_

// utils.hpp
namespace utils {
    void srand(unsigned int seed);
    void srand();
    unsigned int rand();
}

// utils.cpp
#include <time.h>

namespace {
    static unsigned int s_rand_high = 1;
    static unsigned int s_rand_low = 1 ^ 0x49616E42;
}

void utils::srand(unsigned int seed)
{
    s_rand_high = seed;
    s_rand_low = seed ^ 0x49616E42;
}

void utils::srand()
{
    utils::srand(static_cast<unsigned int>(time(0)));
}

unsigned int utils::rand()
{
    static const int shift = sizeof(int) / 2;
    s_rand_high = (s_rand_high >> shift) + (s_rand_high << shift);
    s_rand_high += s_rand_low;
    s_rand_low += s_rand_high;
    return s_rand_high;
}


#endif /* UTILS_H_ */
