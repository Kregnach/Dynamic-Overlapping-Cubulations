#ifndef RANDOM_H
#define RANDOM_H

#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <memory>


static inline unsigned long mix(unsigned long a, unsigned long b, unsigned long c) {
    a = a - b;  a = a - c;  a = a ^ (c >> 13);
    b = b - c;  b = b - a;  b = b ^ (a << 8);
    c = c - a;  c = c - b;  c = c ^ (b >> 13);
    a = a - b;  a = a - c;  a = a ^ (c >> 12);
    b = b - c;  b = b - a;  b = b ^ (a << 16);
    c = c - a;  c = c - b;  c = c ^ (b >> 5);
    a = a - b;  a = a - c;  a = a ^ (c >> 3);
    b = b - c;  b = b - a;  b = b ^ (a << 10);
    c = c - a;  c = c - b;  c = c ^ (b >> 15);
    return c;
}

static inline unsigned long getseed() { return mix(clock(), time(NULL), getpid()); }



class SplitMix {
private:
    uint64_t x;
public:
    SplitMix(uint64_t seed) : x(seed) {}
    uint64_t operator()() {
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
};

class Xoshiro256PlusPlus {
private:
    uint64_t s[4];
    uint64_t rotl(const uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }
public:
    Xoshiro256PlusPlus(uint64_t seed = getseed()) {
        SplitMix rng(seed);
        s[0] = rng();
        s[1] = rng();
        s[2] = rng();
        s[3] = rng();
    }
    
    
    void reseed(uint64_t seed) {
		SplitMix rng(seed);
		s[0] = rng();
		s[1] = rng();
		s[2] = rng();
		s[3] = rng();
	}


    uint64_t operator()() {
        const uint64_t result_starstar = rotl(s[0] * 5, 7) * 9;
        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result_starstar;
    }
};

/*// Function to set or reset the global RNG's seed
void setGlobalRNGSeed(uint64_t seed) {
    static bool canSetSeed = true;
    if (canSetSeed) {
        // Directly initialize the static instance with a new seed
        static Xoshiro256PlusPlus rng(seed);
        canSetSeed = false; // Prevent further re-seeding to ensure consistent RNG behavior
    } else {
        // Optionally, log a warning or handle the case when an attempt is made to re-seed the RNG
    }
}*/

/*Xoshiro256PlusPlus& RNG() {
    static Xoshiro256PlusPlus rng(getseed()); // Default initialization
    return rng;
}*/



// Global RNG accessor (no heap allocation / indirection).
static inline Xoshiro256PlusPlus& RNG() {
    static Xoshiro256PlusPlus rng_instance(getseed());
    return rng_instance;
}



static inline void setGlobalRNGSeed(uint64_t seed) { RNG().reseed(seed); }
/*// Global RNG function
Xoshiro256PlusPlus& RNG() {
    static Xoshiro256PlusPlus rng_instance;
    return rng_instance;
}*/

// Function templates using the global RNG instance
static inline uint32_t uniform_int(uint32_t range) {
    uint32_t x = RNG()();
    uint64_t m = uint64_t(x) * uint64_t(range);
    uint32_t l = uint32_t(m);
    if (l < range) {
        uint32_t t = -range;
        if (t >= range) {
            t -= range;
            if (t >= range) t %= range;
        }
        while (l < t) {
            x = RNG()();
            m = uint64_t(x) * uint64_t(range);
            l = uint32_t(m);
        }
    }
    return m >> 32;
}

static inline double uint64_to_double(uint64_t x) { return (x >> 11) * (1. / (UINT64_C(1) << 53)); }

static inline double uniform_real() { return uint64_to_double(RNG()()); }

static inline double uniform_real(double min, double max) { return min + (max - min) * uniform_real(); }

template<typename RNG> double random_normal(double mean, double sigma) {
    const double two_pi = 2.0 * 3.14159265358979323846;

    double u1, u2;
    do {
        u1 = uniform_real();
        u2 = uniform_real();
    } while (u1 <= std::numeric_limits<double>::min());

    double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(two_pi * u2);
    return z0 * sigma + mean;
}

template<typename RNG> double random_bernoulli(double probTrue) { return uniform_real() <= probTrue; }

int random_choice(std::vector<double>& weights) {
    double total = 0.0;
    for (auto w : weights) total += w;
    
    double p = uniform_real(0.0, total);
    for (size_t i = 0; i < weights.size(); ++i) {
        p -= weights[i];
        if (p <= 0.0) return static_cast<int>(i);
    }
    return 0; // Default return in case of rounding errors
}

#endif // RANDOM_H

