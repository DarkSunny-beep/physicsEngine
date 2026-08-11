#pragma once
#include <cyclone/core.h>

namespace cyclone
{
    class Particle
    {
        public:
            
            void setInverseMass(const real inverseMass);
            
            void setMass(const real mass);
            real getMass() const;
            real getInverseMass() const;
            bool hasFiniteMass() const;
            void setDamping(const real damping);
            real getDamping() const;

            void setPosition(const Vector3& position);
            void setPosition(const real x, const real y, const real z);

            void getPosition(Vector3 *position) const;
            Vector3 getPosition() const;

            void setVelocity(const Vector3& velocity);
            void setVelocity(const real x, const real y, const real z);

            void getVelocity(Vector3 *velocity) const;
            Vector3 getVelocity() const;

        
            void setAcceleration(const Vector3 &acceleration);
            void setAcceleration(const real x, const real y, const real z);

            void getAcceleration(Vector3 *acceleration) const;        
            Vector3 getAcceleration() const;

            void clearAccumulator(); //accumulate: 모으다
            void addForce(const Vector3 &force); //달랑베르의 원리를 적용하는 함수

            void integrate(real duration); //duration: 프레임 사이 시간 간격

        protected:
            real inverseMass;
            Vector3 position;
            Vector3 velocity;
            Vector3 acceleration;
            //중력처럼 변함없는 가속도를 위해서 있는 멤버이다.
            real damping;
            Vector3 forceAccum;
        


    };
}
