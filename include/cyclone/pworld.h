#pragma once
#include "cyclone.h"

namespace cyclone
{
    class ParticleWorld
    {
        // 파티클들을 갖고 있는 연결리스트(자기 참조 구조체)를 만든다.
        
        public:
        typedef std::vector<Particle*> Particles;
        typedef std::vector<ParticleContactGenerator*> ContactGenerators;
        // cpp 표준 라이브러리의 연결리스트를 사용한다. (책과 다름)

        protected:
        Particles particles;
        bool calculateIterations;
        // (각 프레임마다 contact resolver에게 주는) '반복횟수'를 계산해야 하는지에 대한 유무

        ParticleForceRegistry registry;
        ParticleContactResolver resolver;

        ContactGenerators contactGenerators;
    
        // '접촉'의 장부
        ParticleContact *contacts;
        unsigned maxContacts;

        public:

        ParticleWorld(unsigned maxContacts, unsigned iterations =0);
        unsigned generateContacts(); // 생성된 접촉의 수를 리턴

        void startFrame();
        void integrate(real duration);
        void runPhysics(real duration);

        Particles& getParticles();
        /**
         * Returns the list of contact generators.
         */
        ContactGenerators& getContactGenerators();

        /**
         * Returns the force registry.
         */
        ParticleForceRegistry& getForceRegistry();        
        
        
    };

    class GroundContacts : public ParticleContactGenerator
    {
        ParticleWorld::Particles *particles;

    public:
        void init(cyclone::ParticleWorld::Particles *particles);

        virtual unsigned addContact(cyclone::ParticleContact *contact,
            unsigned limit) const;
    };
}