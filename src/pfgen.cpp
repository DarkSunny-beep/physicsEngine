#include <cyclone/pfgen.h>

using namespace cyclone;

void ParticleForceRegistry::updateForces(real duration)
{
    Registry::iterator i = registrations.begin();
    for(; i!=registrations.end(); i++)
    {
        i->fg->updateForce(i->particle, duration);
    }
    // 장부를 쫙 돌면서 모든 힘 발생기를 실행하며 업데이트
}

ParticleGravity::ParticleGravity(const Vector3& gravity): gravity(gravity) {}

void ParticleGravity::updateForce(Particle *particle, real duration)
{
    if(!particle->hasFiniteMass()) return;
    // 만일 유한한 질량이 아니라면(=무한하다면, 벽같은 것) 아무 것도 하지 않고 리턴

    particle->addForce(gravity * particle->getMass());
}

ParticleDrag::ParticleDrag(const real k1, const real k2): k1(k1),k2(k2) {}

void ParticleDrag::updateForce(Particle *particle, real duration)
{
    Vector3 force;
    particle->getVelocity(&force); //속도
    real dragCoeff = force.magnitude(); //속력
    dragCoeff = k1*dragCoeff + k2*dragCoeff*dragCoeff;

    force.normalize();
    force *= -dragCoeff;
    particle->addForce(force);

}

ParticleSpring::ParticleSpring(Particle* other, real springConstant, real restLength)
    : other(other), springConstant(springConstant), restLength(restLength) {}

void ParticleSpring::updateForce(Particle *particle, real duration)
{
    Vector3 force;
    Vector3 otherPos;
    Vector3 parPos;

    other->getPosition(&otherPos);
    particle->getPosition(&parPos);
    Vector3 d = parPos-otherPos;
    real l = d.magnitude();
    d.normalize();
    force.addScaledVector(d,-springConstant*(l-restLength));

    particle->addForce(force);
    // 내가 작성한 거라 책과 다름.

}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength)
    : anchor(anchor), springConstant(springConstant), restLength(restLength) {}

void ParticleAnchoredSpring::updateForce(Particle *particle, real duration)
{
    Vector3 force;
    Vector3 parPos;
    particle->getPosition(&parPos);
    Vector3 d = parPos- *anchor;
    real l = d.magnitude();
    d.normalize();
    force.addScaledVector(d,-springConstant*(l-restLength));

    particle->addForce(force);
    // 내가 작성한 거라 책과 다름.
}

ParticleBungee::ParticleBungee(Particle *other, real springConstant, real restLength)
    : other(other), springConstant(springConstant), restLength(restLength) {}

void ParticleBungee::updateForce(Particle *particle, real duration)
{
    Vector3 parPos;
    Vector3 otherPos;
    particle->getPosition(&parPos);
    other->getPosition(&otherPos);
    Vector3 d = parPos-otherPos;
    real l = d.magnitude();

    if(l-restLength<0) return; // 척력이 발생하는 조건에서 return

    Vector3 force;    
    d.normalize();
    force.addScaledVector(d,-springConstant*(l-restLength));

    particle->addForce(force);
    // 내가 작성한 거라 책과 다름.
}

ParticleBuoyancy::ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDenstiy)
    : maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {}
void ParticleBuoyancy::updateForce(Particle *particle, real duration)
{
    real depth = particle->getPosition().y;

     // 파티클이 물 밖에 위치할 경우 return
    if(depth>=waterHeight+maxDepth) return;

    Vector3 force; // 자동으로 (0,0,0)으로 초기화

    // 파티클이 완전히 물에 잠겨있을 경우
    if(depth<=waterHeight-maxDepth)
    {
        force.addScaledVector(Vector3::GRAVITY,-volume*liquidDensity);
        particle->addForce(force);
        return;
    }
    
    // 파티클이 적당히 잠겨있을 경우
    real d = (depth-waterHeight-maxDepth)/(2*maxDepth);
    force.addScaledVector(Vector3::GRAVITY,-volume*liquidDensity*d);
    particle->addForce(force);

}

ParticleFakeSpring::ParticleFakeSpring(Vector3* anchor, real springConstant, real damping)
    : anchor(anchor), springConstant(springConstant), damping(damping){}
void ParticleFakeSpring::updateForce(Particle *particle, real duration)
{
    if(!particle->hasFiniteMass()) return;

    //앵커로부터 파티클의 상대적인 위치 계산
    Vector3 position;
    particle->getPosition(&position);
    position -= *anchor;
    real gamma = 0.5f * real_sqrt(4*springConstant - damping*damping);
    if(gamma==0.0f) return;

    Vector3 c= position*(damping/(2.0f*gamma)) + particle->getVelocity()*(1.0f/gamma);

    Vector3 targetPos = position*(real_cos(gamma*duration)) + c*real_sin(gamma*duration);
    targetPos *= real_exp(-0.5f*damping*duration);

    //가속도 계산
    Vector3 accel = (targetPos - position)*(1.0f/(duration*duration)) - particle->getVelocity()*(1.0f/duration);
    particle->addForce(accel*particle->getMass());

}
