#include <cyclone/pworld.h>

using namespace cyclone;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
: resolver(iterations), maxContacts(maxContacts)
{
    contacts = new ParticleContact[maxContacts];
    calculateIterations = (iterations == 0);
}

void ParticleWorld::startFrame()
{
    for (Particles::iterator p = particles.begin();
        p != particles.end();
        p++)
    {
        // ParticleWorld 내부의 모든 파티클에 누적되어있는 힘을 초기화한다.
        (*p)->clearAccumulator();
    }
}

unsigned ParticleWorld::generateContacts()
{
    unsigned limit = maxContacts;
    ParticleContact *nextContact = contacts;

    std::memset(contacts, 0, maxContacts * sizeof(ParticleContact));

    for (ContactGenerators::iterator g = contactGenerators.begin();
        g != contactGenerators.end();
        g++)
    {

        if(limit==0) break;
        unsigned used =(*g)->addContact(nextContact, limit);
        if(used>limit) used = limit;

        limit -= used;
        nextContact += used;
    }

    // Return the number of contacts used.
    return maxContacts - limit;
}

void ParticleWorld::integrate(real duration)
{

    for (Particles::iterator p = particles.begin();
        p != particles.end();
        p++)
    {
        // Remove all forces from the accumulator
        (*p)->integrate(duration);
    }
}

void ParticleWorld::runPhysics(real duration)
{
    // 1. 힘 업데이트
    registry.updateForces(duration);


    // 2. 힘 토대로 적분하여 속도 위치 업뎃
    integrate(duration);


    // 3. 접촉 생성(감지)
    unsigned usedContacts = generateContacts();

    // 4. 해결
    if(usedContacts>0)
    {
        resolver.setIterations(40);
        resolver.resolveContacts(contacts, usedContacts, duration);
        // contact 배열과, contact의 개수와 duration을 인자로 넘김.
    
    }
}

ParticleWorld::Particles& ParticleWorld::getParticles()
{
    return particles;
}

ParticleWorld::ContactGenerators& ParticleWorld::getContactGenerators()
{
    return contactGenerators;
}

ParticleForceRegistry& ParticleWorld::getForceRegistry()
{
    return registry;
}


void GroundContacts::init(cyclone::ParticleWorld::Particles *particles)
{
    GroundContacts::particles = particles;
}

unsigned GroundContacts::addContact(cyclone::ParticleContact *contact,
                                    unsigned limit) const
{
    unsigned count = 0;
    for (cyclone::ParticleWorld::Particles::iterator p = particles->begin();
        p != particles->end();
        p++)
    {
        cyclone::real y = (*p)->getPosition().y;
        if (y < 0.0f)
        {
            contact->contactNormal = cyclone::Vector3::UP;
            contact->particle[0] = *p;
            contact->particle[1] = NULL;
            contact->penetration = -y;
            contact->restitution = 0.2f;
            contact++;
            count++;
        }

        if (count >= limit) return count;
    }
    // ParticleWorld의 모든 파티클을 순회하며 파티클의 위치가 y<0에 있을 경우 접촉으로 간주하고
    // contact에 저장한다. 접촉의 개수를 초과할 경우 반복을 끝내고, 등록된 접촉 개수를 반환한다.
    return count;
}