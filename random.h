/* Copyright (c) 2018 Arvid Gerstmann. */
/* This code is licensed under MIT license. */
#ifndef AG_RANDOM_H
#define AG_RANDOM_H
#include <random>
#include <ctime>
class pcg
{
public:
    using result_type = uint32_t;
    static constexpr result_type (min)() { return 0; }
    static constexpr result_type (max)() { return UINT32_MAX; }
    friend bool operator==(pcg const &, pcg const &);
    friend bool operator!=(pcg const &, pcg const &);

    pcg()
        : m_state(0x853c49e6748fea9bULL)
        , m_inc(0xda3e39cb94b95bdbULL)
    {}
    explicit pcg(std::mt19937& mt, std::uniform_int_distribution<int>& dist)
    {
        seed(mt, dist);
    }

    void seed(std::mt19937& mt, std::uniform_int_distribution<int>& dist)
    {
        uint64_t s0 = uint64_t(dist(mt)) << 31 | uint64_t(dist(mt));
        uint64_t s1 = uint64_t(dist(mt)) << 31 | uint64_t(dist(mt));

        m_state = 0;
        m_inc = (s1 << 1) | 1;
        (void)operator()();
        m_state += s0;
        (void)operator()();
    }

    result_type operator()()
    {
        uint64_t oldstate = m_state;
        m_state = oldstate * 6364136223846793005ULL + m_inc;
        uint32_t xorshifted = uint32_t(((oldstate >> 18u) ^ oldstate) >> 27u);
        int rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    uint64_t m_state;
    uint64_t m_inc;
};

bool operator==(pcg const &lhs, pcg const &rhs);

bool operator!=(pcg const &lhs, pcg const &rhs);


#endif /* AG_RANDOM_H */
