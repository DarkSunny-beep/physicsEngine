#pragma once

#include "pcontacts.h"

namespace cyclone
{
    class ParticleLink : public ParticleContactGenerator
    {
        friend class BridgeDemo;
        public:
            Particle* particle[2];

        protected:
            real currentLength() const;

        public:
        // 접촉 생성기 메서드이다. 빈 contact 배열을 받아서, 채워넣는 역할을 한다. 
        // limit은 이 배열에 저장 가능한 최대 개수를 의미한다.
        // 반환하는 unsigned는, 늘어나지 않았다면 0, 한계 길이를 초과해 늘어났다면 1이다.
            virtual unsigned addContact(ParticleContact* contact, unsigned limit) const =0;
    };
    
    class ParticleCable:public ParticleLink
    {
        friend class BridgeDemo;
        public:
            real maxLength;
            real restitution;
        public:
            virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;

    };

    class ParticleConstraint : public ParticleContactGenerator
    {
        friend class BridgeDemo;
        public:
            Particle *particle;
            Vector3 anchor;
        protected:
            real currentLength() const;
        public:
            virtual unsigned addContact(ParticleContact *contact, unsigned limit) const = 0;
    };

    class ParticleCableConstraint : public ParticleConstraint
    {
        friend class BridgeDemo;
        public:
            real maxLength;
            real restitution;

            virtual unsigned addContact(ParticleContact *contact, unsigned limit)const override;
    };

    class ParticleRodConstraint : public ParticleConstraint
    {
        friend class BrigdeDemo;
        public:
        
            real length;

        public:
            virtual unsigned addContact(ParticleContact *contact,
            unsigned limit) const;
    };


    class ParticleRod : public ParticleLink
    {
        public:
            real length;
        public:
            virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;

    };
}