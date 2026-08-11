#include <cstdlib>
#include <ctime>
#include <cyclone/random.h>

using namespace cyclone;

Random::Random()
{
    seed(0);
}

Random::Random(unsigned seed)
{
    Random::seed(seed);
}

void Random::seed(unsigned s)
{
    if (s == 0) {
        s = (unsigned)clock();
    }
    // 시드값이 딱히 없는 경우. 그냥 clock함수가 반환하는 시간(클럭틱)을 시드로 사용한다.

    // 단순한 선형합동기(Linear congruential generator, LCG)를 이용해 17개의 난수로 버퍼를 채운다.
    for (unsigned i = 0; i < 17; i++)
    {
        // Simple linear congruential generator
        s = s * 2891336453 + 1;
        buffer[i] = s;
    }

    // 최종 난수를 생성할 때 사용할 (버퍼 내부를 가르키는)포인터 초기화.
    p1 = 0;  p2 = 10;
}


// 비트 연산자를 이용해서 rotation left, rotation right.
unsigned Random::rotl(unsigned n, unsigned r)
{
	  return (n << r) | (n >> (32 - r));
}

unsigned Random::rotr(unsigned n, unsigned r)
{
	  return (n >> r) | (n << (32 - r));
}

unsigned Random::randomBits()
{
    unsigned result;

    // Rotate the buffer and store it back to itself
    result = buffer[p1] = rotl(buffer[p2], 13) + rotl(buffer[p1], 9);

    // Rotate pointers
    if (--p1 < 0) p1 = 16;
    if (--p2 < 0) p2 = 16;

    // Return result : 최종 난수 생성
    return result;
}

#ifdef SINGLE_PRECISION
real Random::randomReal()
{
    // Get the random number
    unsigned bits = randomBits();

    // Set up a reinterpret structure for manipulation
    union {
        real value;
        unsigned word;
    } convert;

    // Now assign the bits to the word. This works by fixing the ieee
    // sign and exponent bits (so that the size of the result is 1-2)
    // and using the bits to create the fraction part of the float.
    convert.word = (bits >> 9) | 0x3f800000;

    // And return the value
    return convert.value - 1.0f;
}
#else
real Random::randomReal()
{
    // Get the random number
    unsigned bits = randomBits();

    // Set up a reinterpret structure for manipulation
    union {
        real value;
        unsigned words[2];
    } convert;

    // Now assign the bits to the words. This works by fixing the ieee
    // sign and exponent bits (so that the size of the result is 1-2)
    // and using the bits to create the fraction part of the float. Note
    // that bits are used more than once in this process.
    convert.words[0] =  bits << 20; // Fill in the top 16 bits
    convert.words[1] = (bits >> 12) | 0x3FF00000; // And the bottom 20

    // And return the value
    return convert.value - 1.0;
}
#endif

real Random::randomReal(real min, real max)
{
    return randomReal() * (max-min) + min;
}

real Random::randomReal(real scale)
{
    return randomReal() * scale;
}

unsigned Random::randomInt(unsigned max)
{
    return randomBits() % max;
}

real Random::randomBinomial(real scale)
{
    return (randomReal()-randomReal())*scale;
}

Quaternion Random::randomQuaternion()
{
    Quaternion q(
        randomReal(),
        randomReal(),
        randomReal(),
        randomReal()
        );
    q.normalise();
    return q;
}

Vector3 Random::randomVector(real scale)
{
    return Vector3(
        randomBinomial(scale),
        randomBinomial(scale),
        randomBinomial(scale)
        );
}

Vector3 Random::randomXZVector(real scale)
{
    return Vector3(
        randomBinomial(scale),
        0,
        randomBinomial(scale)
        );
}

Vector3 Random::randomVector(const Vector3 &scale)
{
    return Vector3(
        randomBinomial(scale.x),
        randomBinomial(scale.y),
        randomBinomial(scale.z)
        );
}

Vector3 Random::randomVector(const Vector3 &min, const Vector3 &max)
{
    return Vector3(
        randomReal(min.x, max.x),
        randomReal(min.y, max.y),
        randomReal(min.z, max.z)
        );
}