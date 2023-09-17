#pragma once

static unsigned int* wrand_seed()
{
    static unsigned int m_seed = 0;
    return &m_seed;
}

static unsigned int* xrand_seed()
{
    static unsigned int m_seed = 0;
    return &m_seed;
}

#define wrseed (*wrand_seed())
#define xrseed (*xrand_seed())

void swrand(unsigned int seed)
{
    wrseed = seed;
}

void sxrand(unsigned int seed)
{
    xrseed = seed;
}

unsigned int wrand(unsigned int seed)
{
    wrseed = wrseed * seed;
    wrseed = (1103515245 * wrseed + 12345) & 0x7FFFFFFF;
    return wrseed;
}

unsigned int xrand(unsigned int seed) {
    xrseed ^= (xrseed << 13);
    xrseed ^= (xrseed >> 17);
    xrseed ^= (xrseed << 5);
    xrseed += seed;
    return xrseed;
}

unsigned int random_in_range(unsigned int begin, unsigned int end)
{
    if (end == 0) return 0;
    if (begin > end)
    {
        auto temp = begin;
        begin = end;
        end = temp;
    }
    return ((wrand(static_cast<unsigned int>(time(NULL))) % (end - begin)) + begin);
}
