#include "P2random.h"

void P2random::PR_init(std::stringstream& ss,
                       unsigned int seed,
                       unsigned int num_traders,
                       unsigned int num_stocks,
                       unsigned int num_orders,
                       unsigned int arrival_rate) {
    P2random::MersenneTwister mt;
    mt.init_genrand(seed);

    int max_price = 100;
    int max_quantity = 50;

    long double timestamp = 0;
    for (unsigned int i = 0; i < num_orders; ++i) {
        unsigned int time_increase = (mt.genrand_unsigned_int() % arrival_rate + 1);
        timestamp += (1.0l / time_increase);
        unsigned int stock_num = mt.genrand_unsigned_int() % num_stocks;
        unsigned int trader_num = mt.genrand_unsigned_int() % num_traders;

        std::string buy_or_sell = (mt.genrand_unsigned_int() % 2) ? "BUY" : "SELL";
        unsigned int quantity = mt.genrand_unsigned_int() % max_quantity + 1;
        unsigned int price = mt.genrand_unsigned_int() % max_price + 1;

        ss << int(timestamp)
           << " " << buy_or_sell
           << " T" << trader_num
           << " S" << stock_num
           << " $" << price
           << " #" << quantity
           << "\n";
    } // for
} // PR_init()

/**
* C++ Mersenne Twister wrapper class written by
* Jason R. Blevins <jrblevin@sdf.lonestar.org> on July 24, 2006.
* Based on the original MT19937 C code by
* Takuji Nishimura and Makoto Matsumoto.
*/

/*
A C-program for MT19937, with initialization improved 2002/1/26.
Coded by Takuji Nishimura and Makoto Matsumoto.

Before using, initialize the state by using init_genrand(seed).

Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. The names of its contributors may not be used to endorse or promote
products derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Any feedback is very welcome.
http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/**
* Constructor
*/
P2random::MersenneTwister::MersenneTwister(void) :
    mt_(new unsigned int[N]), mti_(N + 1), s_(0),
    seeded_by_array_(false), seeded_by_int_(false) {
    init_genrand(0);
} // constructor

/**
* Destructor
*/
P2random::MersenneTwister::~MersenneTwister(void) {
    delete[] mt_;
    mt_ = nullptr;
} // destructor

/**
* Initializes the Mersenne Twister with a seed.
*
* \param s seed
*/
void P2random::MersenneTwister::init_genrand(unsigned int s) {
    mt_[0] = s & 0xffffffffU;
    for (mti_ = 1; mti_ < N; mti_++) {
        mt_[mti_] =
            (1812433253U * (mt_[mti_ - 1] ^ (mt_[mti_ - 1] >> 30)) + static_cast<unsigned int>(mti_));
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt_[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt_[mti_] &= 0xffffffffU;
        /* for >32 bit machines */
    } // for
    // Store the seed
    s_ = s;
    seeded_by_array_ = false;
    seeded_by_int_ = true;
} // init_genrand()

/**
* Generates a random number on [0,0xffffffff]-interval
*
* \return random number on [0, 0xffffffff]
*/
unsigned int P2random::MersenneTwister::genrand_unsigned_int(void) {
    unsigned int y;

    if (mti_ >= N) { /* generate N words at one time */
        // mag01[x] = x * MATRIX_A  for x=0,1
        const static unsigned int mag01[2] = { 0x0U, MATRIX_A };
        unsigned int kk;

        if (mti_ == N + 1)   /* if init_genrand() has not been called, */
            init_genrand(5489U); /* a default initial seed is used */

        for (kk = 0; kk < N - M; kk++) {
            y = (mt_[kk] & UPPER_MASK) | (mt_[kk + 1] & LOWER_MASK);
            mt_[kk] = mt_[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        } // for
        for (; kk < N - 1; kk++) {
            y = (mt_[kk] & UPPER_MASK) | (mt_[kk + 1] & LOWER_MASK);
            mt_[kk] = mt_[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        } // for
        y = (mt_[N - 1] & UPPER_MASK) | (mt_[0] & LOWER_MASK);
        mt_[N - 1] = mt_[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti_ = 0;
    } // if

    y = mt_[mti_++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    return y;
} // genrand_unsigned_int()
