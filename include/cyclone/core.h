#pragma once
#include <cyclone/precision.h>
namespace cyclone
{
    class Vector3
    
    {
        public:
            real x;
            real y;
            real z;
        private:
            real pad;
        public:
            Vector3(): x(0), y(0),z(0) {}
            
            Vector3(const real x, const real y, const real z):
            x(x), y(y), z(z) {}
            const static Vector3 GRAVITY;
            const static Vector3 UP;
            
            void invert()
            {
                x= -x;
                y= -y;
                z= -z;
            }
            real magnitude() const //const: 내부 값을 절대 변환하지 않는다는 의미
            {
                return real_sqrt(x*x + y*y + z*z);
            }
            real squareMagnitude() const
            {
                return x*x + y*y + z*z; 
            }
            void operator*=(const real value)
            {
                x*=value;
                y*=value;
                z*=value;
            } 
            void normalize()
            {
                real l = magnitude();
                if(l>0)
                {
                    (*this) *= ((real) 1)/l;
                }
            }

            Vector3 operator*(const real value) const
            {
                return Vector3(x*value, y*value, z*value);
            }

            //벡터와 벡터의 연산이 가능하도록 연산자 오버로딩을 해보자
            void operator+=(const Vector3& v)
            {
                //(자료형)& : 참조 연산자. v의 주소를 받음
                x += v.x;
                y += v.y;
                z += v.z;
            }
            Vector3 operator+(const Vector3& v) const
            {
                return Vector3(x+v.x, y+v.y, z+v.z);
            }
            void operator-=(const Vector3& v)
            {
                x -= v.x;
                y -= v.y;
                z -= v.z;
            }
            Vector3 operator-(const Vector3& v) const
            {
                return Vector3(x-v.x, y-v.y, z-v.z);
            }
            void addScaledVector(const Vector3& vector, real value)
            {
                // (*this) += vector*value;
                // 위 방법은 쓸데없는 하나의 객체를 만들었다 부수는 과정을 동반하므로 간단해보이지만 cpu가 더 수고스러움
                x+= vector.x * value;
                y+= vector.y * value;
                z+= vector.z * value;

            }

            Vector3 componentProduct(const Vector3& v) const
            {
                return Vector3(x*v.x, y*v.y, z*v.z);
            }
            void componentProductUpdate(const Vector3& vector)
            {
                x *= vector.x;
                y *= vector.y;
                z *= vector.z;
            }

            real scalarProduct(const Vector3& v) const
            {
                return x*v.x + y*v.y + z*v.z;
            }
            real operator*(const Vector3& v) const
            {
                return x*v.x + y*v.y + z*v.z;
            }//위 scalarProduct 함수와 100% 일치하는 기능. 단지 내적을 A*B라고 표현하는게
            //조금 어색하므로 친절히 위 함수를 같이 선언해주는 것.
            Vector3 vectorProduct(const Vector3& v) const
            {
                return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
            }
            void operator%=(const Vector3& v)
            {
                (*this) = vectorProduct(v);
                //하나하나 계산해서 대입하면 임시 객체를 생성하는 불필요한 일을 할 필요가 없지만 휴먼 에러가 생길 수 있음
            }

            Vector3 operator%(const Vector3& v)const
            {
                return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
            }

            void clear()
            {
                x = y = z = 0;
            }
    };
    class Quaternion
    {
    public:
        union {
            struct {
                /**
                 * Holds the real component of the quaternion.
                 */
                real r;

                /**
                 * Holds the first complex component of the
                 * quaternion.
                 */
                real i;

                /**
                 * Holds the second complex component of the
                 * quaternion.
                 */
                real j;

                /**
                 * Holds the third complex component of the
                 * quaternion.
                 */
                real k;
            };

            /**
             * Holds the quaternion data in array form.
             */
            real data[4];
        };

        // ... other Quaternion code as before ...

        /**
         * The default constructor creates a quaternion representing
         * a zero rotation.
         */
        Quaternion() : r(1), i(0), j(0), k(0) {}

        /**
         * The explicit constructor creates a quaternion with the given
         * components.
         *
         * @param r The real component of the rigid body's orientation
         * quaternion.
         *
         * @param i The first complex component of the rigid body's
         * orientation quaternion.
         *
         * @param j The second complex component of the rigid body's
         * orientation quaternion.
         *
         * @param k The third complex component of the rigid body's
         * orientation quaternion.
         *
         * @note The given orientation does not need to be normalised,
         * and can be zero. This function will not alter the given
         * values, or normalise the quaternion. To normalise the
         * quaternion (and make a zero quaternion a legal rotation),
         * use the normalise function.
         *
         * @see normalise
         */
        Quaternion(const real r, const real i, const real j, const real k)
            : r(r), i(i), j(j), k(k)
        {
        }

        /**
         * Normalises the quaternion to unit length, making it a valid
         * orientation quaternion.
         */
        void normalise()
        {
            real d = r*r+i*i+j*j+k*k;

            // Check for zero length quaternion, and use the no-rotation
            // quaternion in that case.
            if (d < real_epsilon) {
                r = 1;
                return;
            }

            d = ((real)1.0)/real_sqrt(d);
            r *= d;
            i *= d;
            j *= d;
            k *= d;
        }

        /**
         * Multiplies the quaternion by the given quaternion.
         *
         * @param multiplier The quaternion by which to multiply.
         */
        void operator *=(const Quaternion &multiplier)
        {
            Quaternion q = *this;
            r = q.r*multiplier.r - q.i*multiplier.i -
                q.j*multiplier.j - q.k*multiplier.k;
            i = q.r*multiplier.i + q.i*multiplier.r +
                q.j*multiplier.k - q.k*multiplier.j;
            j = q.r*multiplier.j + q.j*multiplier.r +
                q.k*multiplier.i - q.i*multiplier.k;
            k = q.r*multiplier.k + q.k*multiplier.r +
                q.i*multiplier.j - q.j*multiplier.i;
        }

        /**
         * Adds the given vector to this, scaled by the given amount.
         * This is used to update the orientation quaternion by a rotation
         * and time.
         *
         * @param vector The vector to add.
         *
         * @param scale The amount of the vector to add.
         */
        void addScaledVector(const Vector3& vector, real scale)
        {
            Quaternion q(0,
                vector.x * scale,
                vector.y * scale,
                vector.z * scale);
            q *= *this;
            r += q.r * ((real)0.5);
            i += q.i * ((real)0.5);
            j += q.j * ((real)0.5);
            k += q.k * ((real)0.5);
        }

        void rotateByVector(const Vector3& vector)
        {
            Quaternion q(0, vector.x, vector.y, vector.z);
            (*this) *= q;
        }
    };

    /**
     * Holds a transform matrix, consisting of a rotation matrix and
     * a position. The matrix has 12 elements, it is assumed that the
     * remaining four are (0,0,0,1); producing a homogenous matrix.
     */
}