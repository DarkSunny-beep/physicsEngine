#include <cyclone/plinks.h>

real ParticleLink::currentLength() const
{
    Vector3 relativePos
    = particle[0]->getPosition() - particle[1]->getPosition();
    return relativePos.magnitude();
}

unsigned ParticleCable::addContact(ParticleContact* contact, unsigned limit) const
{
    real length = currentLength();

    // 아직 충분히 늘어나있지 않은 경우(따로 처리가 필요하지 않음)
    if(length < maxLength)
    {
        return 0;
        // 등록할 접촉이 없음.
    }
    
    // if문에서 걸리지 않았다면 접촉 처리가 필요한 경우이므로, contact에 정보를 저장해준다.
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal.normalize();
    contact->contactNormal = normal;

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    return 1;
}


unsigned ParticleRod::addContact(ParticleContact* contact, unsigned limit) const
{
    real currentLen = currentLength();
    if(currentLen == length)
        return 0;

    // if문에 걸리지 않았을 경우, 보정이 필요함.
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal.normalize();
    
    // contactNormal은 압축되어있는지, 늘어나있는지에 달려있다.
    if(currentLen > length)
    {
        contact->contactNormal = normal;
        contact->penetration = currentLen-length;
    } else 
    {
        contact->contactNormal = normal*(-1);
        contact->penetration = length-currentLen;
    }

    contact->restitution = 0; // 바운스 하면 안되므로

    return 1;


}

real ParticleConstraint::currentLength() const
{
    Vector3 relativePos = particle->getPosition() - anchor;
    return relativePos.magnitude();
}

unsigned ParticleCableConstraint::addContact(ParticleContact *contact,
                                   unsigned limit) const
{
    // Find the length of the cable
    real length = currentLength();

    // Check if we're over-extended
    if (length < maxLength)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->particle[0] = particle;
    contact->particle[1] = nullptr;

    // Calculate the normal
    Vector3 normal = anchor - particle->getPosition();
    normal.normalize();
    contact->contactNormal = normal;

    contact->penetration = length-maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned ParticleRodConstraint::addContact(ParticleContact *contact,
                                 unsigned limit) const
{
    // Find the length of the rod
    real currentLen = currentLength();

    // Check if we're over-extended
    if (currentLen == length)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->particle[0] = particle;
    contact->particle[1] = 0;

    // Calculate the normal
    Vector3 normal = anchor - particle->getPosition();
    normal.normalize();

    // The contact normal depends on whether we're extending or compressing
    if (currentLen > length) {
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
    } else {
        contact->contactNormal = normal * -1;
        contact->penetration = length - currentLen;
    }

    // Always use zero restitution (no bounciness)
    contact->restitution = 0;

    return 1;
}