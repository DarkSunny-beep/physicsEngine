#pragma once
#include "particle.h"
#include "core.h"
#include <vector>

namespace cyclone
{
    class ParticleForceGenerator
    {
        public:
            virtual void updateForce(Particle *particle, real duration)=0;
    };

    class ParticleGravity : public ParticleForceGenerator
    {
        Vector3 gravity;
        public:
            ParticleGravity(const Vector3& gravity);
            virtual void updateForce(Particle *particle, real duration);
    };

    class ParticleDrag : public ParticleForceGenerator
    {   
        real k1;
        real k2;
        public:
            ParticleDrag(const real k1, const real k2);
            virtual void updateForce(Particle *particle, real duration);
    };

    class ParticleSpring : public ParticleForceGenerator
    {
        Particle *other; // 또 다른 맨 끝에 달려있는 파티클

        real springConstant; // 용수철 상수

        real restLength; // 정지 질량
        public:
            ParticleSpring(Particle *other, real springConstant, real restLength);
            virtual void updateForce(Particle *particle, real duration);
    };

    class ParticleAnchoredSpring : public ParticleForceGenerator
    {
        Vector3 *anchor;

        real springConstant;
        real restLength;

        public:
            ParticleAnchoredSpring(Vector3 *anchor, real springConstant, real restLength);
            virtual void updateForce(Particle *particle, real duration);
    };

    class ParticleBungee : public ParticleForceGenerator
    {
        Particle *other;

        real springConstant;
        real restLength;

        public:
            ParticleBungee(Particle *other, real springConstant, real restLength);
            virtual void updateForce(Particle *particle, real duration);
    };

    class ParticleBuoyancy : public ParticleForceGenerator
    {
        real maxDepth; 
        real volume; // 물체의 부피
        real waterHeight; // y=0 으로부터 액체 표면까지의 높이
        real liquidDensity; // 액체의 밀도. 순수 물의 경우 1000kg/m^3

        public:
            ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDenstiy=1000.0f);
            virtual void updateForce(Particle *particle, real duration);
    };

    class ParticleFakeSpring : public ParticleForceGenerator
    {
        Vector3 *anchor;
        real springConstant;
        real damping;

        public:
            ParticleFakeSpring(Vector3 *anchor, real springConstant, real damping);
            virtual void updateForce(Particle *particle, real duration);

    };

    class ParticleForceRegistry
    {
        protected:
            struct ParticleForceRegistration
            {
                Particle *particle;
                ParticleForceGenerator *fg;
            };
            typedef std::vector<ParticleForceRegistration> Registry;
            Registry registrations;

        public:
            void add(Particle* particle, ParticleForceGenerator* fg); //페어 생성
            void remove(Particle* particle, ParticleForceGenerator* fg); //페어를 해제
            void clear(); //모든 페어 해제

            void updateForces(real duration); //업데이트를 위해 모든 힘 발생기를 실행
            
    };
}
