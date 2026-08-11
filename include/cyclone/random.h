

// 해당 헤더파일은 난수 생성기의 정의들을 포함한다.

#ifndef CYCLONE_RANDOM_H
#define CYCLONE_RANDOM_H
// 헤더 가드의 역할을 한다. 다른 파일에서 random.h를 이미 불러온 경우 CYCLONE_RANDOM_H로 이미 정의
// 되어 있으므로 해당 파일의 복사를 건너뛰게 된다.(이름 재정의 방지) -> 도장같은 거라고 생각하면 됨

#include "core.h"

namespace cyclone {

    // 랜덤 클래스를 설정하면 각기 다른 랜덤 스트림을 만들어 낼 수 있으며 이는 폭죽, 몬스터 생성 등등
    // 다양한 분야에서 독립적인 랜덤 스트림을 사용할 수 있게 한다. 
    class Random
    {
    public:
    	/**
    	 * left bitwise rotation
    	 */

    	unsigned rotl(unsigned n, unsigned r);
    	/**
    	 * right bitwise rotation
    	 */
    	unsigned rotr(unsigned n, unsigned r);

        // 랜덤 스트림 생성자, seed는 0으로 초기화됨
        Random();

        // 주어진 시드로 랜덤 스트림 생성함.
        Random(unsigned seed);

        // 주어진 시드로 크기 17의 버퍼 생성
        void seed(unsigned seed);

        // 스트림에서 랜덤 비트열을 뽑아낸다.
        unsigned randomBits();

        /**
         * Returns a random floating point number between 0 and 1.
         */
        real randomReal();

        /**
         * Returns a random floating point number between 0 and scale.
         */
        real randomReal(real scale);

        /**
         * Returns a random floating point number between min and max.
         */
        real randomReal(real min, real max);

        /**
         * Returns a random integer less than the given value.
         */
        unsigned randomInt(unsigned max);

        /**
         * Returns a random binomially distributed number between -scale
         * and +scale.
         */
        real randomBinomial(real scale);

        /**
         * Returns a random vector where each component is binomially
         * distributed in the range (-scale to scale) [mean = 0.0f].
         */
        Vector3 randomVector(real scale);

        /**
         * Returns a random vector where each component is binomially
         * distributed in the range (-scale to scale) [mean = 0.0f],
         * where scale is the corresponding component of the given
         * vector.
         */
        Vector3 randomVector(const Vector3 &scale);

        /**
         * Returns a random vector in the cube defined by the given
         * minimum and maximum vectors. The probability is uniformly
         * distributed in this region.
         */
        Vector3 randomVector(const Vector3 &min, const Vector3 &max);

        /**
         * Returns a random vector where each component is binomially
         * distributed in the range (-scale to scale) [mean = 0.0f],
         * except the y coordinate which is zero.
         */
        Vector3 randomXZVector(real scale);

        /**
         * Returns a random orientation (i.e. normalized) quaternion.
         */
        Quaternion randomQuaternion();

    private:
        // Internal mechanics
        int p1, p2;
        unsigned buffer[17];
    };

}

#endif // CYCLONE_BODY_H