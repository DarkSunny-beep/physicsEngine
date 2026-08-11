#include <assert.h>
//단언(assert) 매크로 사용을 위함
//프로그램 실행 중 조건이 거짓일 경우 프로그램을 즉시 중단 후 오류 위치를 알려줌
#include <cyclone/particle.h>
#include <limits>

using namespace cyclone;
// cyclone이라는 네임스페이스 안에 정의된 모든 클래스, 함수들을 코드 내에서 cyclone:: 접두사를 붙이지
// 않고도 바로 사용할 수 있게 해주는 선언

void Particle::integrate(real duration)
{
    assert(duration>0.0);
    // 시간 간격(duration)이 0.0보다 작으면 오류 처리

    // 선형 위치 벡터 업데이트
    position.addScaledVector(velocity, duration);

    // a=1/m * F 를 이용하여 가속도 벡터 업데이트
    Vector3 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);
    // 만약 forceAccum이 바람이나 폭발력같은 순간적인 힘이라면 다음프레임에서는 초기화되어야 한다.
    // 이때 그냥 acceleration에 업데이트 해버리면 계속 그 순간적인 힘을 받는 것으로 되기에 새로운 객체를 
    // 생성해줘야 한다.

    
    // 새로운 가속도 벡터로 새로운 속도 벡터 업데이트(다음 위치 벡터 업데이트에 사용됨)
    velocity.addScaledVector(resultingAcc, duration);

    // velocity에 damping효과를 적용(duration에 따른 damping으로 적용)
    velocity*= real_pow(damping, duration);

    // 만일 파티클이 땅에 닿아있다면, 땅 전용 마찰력 부여<->속도 감쇠
    // 직접 해보니 y==0.0f로 하면 조건 부합이 아예 안되는 거 같아서 범위를 좀 넓힘.
    if (position.y<=0.03f)
    {
        velocity*=0.5f;
    }

    clearAccumulator();
}

void Particle::setMass(const real mass)
{
    assert(mass != 0);
    Particle::inverseMass = ((real)1.0)/mass;
    //역수보다는 질량 그자체에 더 익숙한 개발자들을 위해 일단 setMass에서 자동으로 inverseMass를 설정해줌
}

real Particle::getMass() const
{
    if (inverseMass == 0) {
        return std::numeric_limits<real>::max(); //REAL_MAX;
        //std(Standard): CPP 기본 라이브러리. <real>은 real타입의 max를 반환하라는 간접적 의미
    } else {
        return ((real)1.0)/inverseMass;
    }
}

void Particle::setInverseMass(const real inverseMass)
{
    Particle::inverseMass = inverseMass;
    //명시적이고 직관적인 inverseMass 지정 함수
}

real Particle::getInverseMass() const
{
    return inverseMass;
}

bool Particle::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
    // 물체가 움직일 수 있는지
}

void Particle::setDamping(const real damping)
{
    Particle::damping = damping;
}

real Particle::getDamping() const
{
    return damping;
}

void Particle::setPosition(const Vector3 &position)
{
    Particle::position = position;
}

void Particle::setPosition(const real x, const real y, const real z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void Particle::getPosition(Vector3 *position) const
{
    *position = Particle::position;
    // 인수로 넣는 position 벡터 객체에 particle안의 position을 복사한다.(한 번만 복사)
    // 아래처럼 굳이 position 벡터 하나를 더 복사해서 return(외부로 한 번 더 복사) 하는 일은 없다.
}

Vector3 Particle::getPosition() const
{
    return position;
}

void Particle::setVelocity(const Vector3 &velocity)
{
    Particle::velocity = velocity;
}

void Particle::setVelocity(const real x, const real y, const real z)
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void Particle::getVelocity(Vector3 *velocity) const
{
    *velocity = Particle::velocity;
}

Vector3 Particle::getVelocity() const
{
    return velocity;
}

void Particle::setAcceleration(const Vector3 &acceleration)
{
    Particle::acceleration = acceleration;
}

void Particle::setAcceleration(const real x, const real y, const real z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void Particle::getAcceleration(Vector3 *acceleration) const
{
    *acceleration = Particle::acceleration;
}

Vector3 Particle::getAcceleration() const
{
    return acceleration;
}

void Particle::clearAccumulator()
{
    forceAccum.clear();
    //forceAccum 벡터의 x,y,z을 0으로 초기화한다.
}

void Particle::addForce(const Vector3 &force)
{
    forceAccum += force;
}