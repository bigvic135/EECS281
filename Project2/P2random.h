#ifndef P2RANDOM_H
#define P2RANDOM_H

#include <vector>
#include <string>
#include <sstream>

class P2random {
public:
    static void PR_init(std::stringstream& ss,
                        unsigned int seed,
                        unsigned int num_traders,
                        unsigned int num_stocks,
                        unsigned int num_orders,
                        unsigned int arrival_rate);

    //Don't need to read further than this, unless you want to learn about
    //Mersenne Twister implementation
private:
    /**
    * mt.h: Mersenne Twister header file
    *
    * Jason R. Blevins <jrblevin@sdf.lonestar.org>
    * Durham, March  7, 2007
    */

    /**
    * Mersenne Twister.
    *
    * M. Matsumoto and T. Nishimura, "Mersenne Twister: A
    * 623-dimensionally equidistributed uniform pseudorandom number
    * generator", ACM Trans. on Modeling and Computer Simulation Vol. 8,
    * No. 1, January pp.3-30 (1998).
    *
    * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html.
    */
    class MersenneTwister {
    public:
        MersenneTwister(void);
        ~MersenneTwister(void);

        // The copy constructor and operator=() should never be used.
        MersenneTwister(const MersenneTwister&) = delete;
        MersenneTwister &operator=(const MersenneTwister&) = delete;

        void init_genrand(unsigned int s);

        unsigned int genrand_unsigned_int(void);

    private:
        static const unsigned int N = 624;
        static const unsigned int M = 397;
        // constant vector a
        static const unsigned int MATRIX_A = 0x9908b0dfU;
        // most significant w-r bits
        static const unsigned int UPPER_MASK = 0x80000000U;
        // least significant r bits
        static const unsigned int LOWER_MASK = 0x7fffffffU;

        unsigned int *mt_;                   // the state vector
        unsigned int mti_;                   // mti == N+1 means mt not initialized

        unsigned int s_;                     // Seed integer
        bool seeded_by_array_;               // Seeded by an array
        bool seeded_by_int_;                 // Seeded by an integer
    };
    static MersenneTwister mt;
};

#endif 
