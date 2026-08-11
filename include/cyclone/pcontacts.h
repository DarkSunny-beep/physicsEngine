#pragma once
#include "core.h"
#include "particle.h"

using namespace cyclone;

namespace cyclone
{
class ParticleContact
{
    friend class ParticleContactResolver;
    public:
        //충돌하는 두 입자 배열
        Particle* particle[2];
        real restitution; // 반발계수 c
        Vector3 contactNormal; // world의 좌표에서의 접촉 법선

        real penetration; // 상호 침투의 깊이(위치 보정을 위함)
        Vector3 particleMovement[2];
    protected:
        void resolve(real duration); //속도와 상호 침투를 해결하는 함수
        real calculateSeparatingVelocity() const;
    private:
        void resolveVelocity(real duration);
        void resolveInterpenetration(real duration);

};

class ParticleContactResolver
{
    protected:
        unsigned iterations;
        unsigned iterationsUsed;
        
    public:
        ParticleContactResolver(unsigned iterations);

        void setIterations(unsigned iterations);

        void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
};

class ParticleContactGenerator
{
    
    public:
        virtual unsigned addContact(ParticleContact *contact, unsigned limit) const =0;
    // 위 함수는 contact 배열에 접촉을 추가한다. limit은 등록 가능한 최대 접촉 개수이며 함수는
    // 정상적으로 등록된 접촉의 개수를 반환한다.
};

}
