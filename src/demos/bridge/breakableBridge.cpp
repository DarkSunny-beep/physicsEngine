#include <cyclone/cyclone.h>
#include "../ogl_headers.h"
#include "../app.h"
#include "../timing.h"

#include <stdio.h>
#include <cassert>

#define ROD_COUNT 6
#define CABLE_COUNT 10
#define SUPPORT_COUNT 12

#define BASE_MASS 1
#define EXTRA_MASS 10


class breakableBridgeDemo : public MassAggregateApplication
{
    std::vector<ParticleCableConstraint*> supports;
    std::vector<ParticleCable*> cables;
    std::vector<ParticleRod*> rods;

    cyclone::Vector3 massPos;
    cyclone::Vector3 massDisplayPos;

    
    void updateAdditionalMass();

public:
    /** Creates a new demo object. */
    breakableBridgeDemo();
    virtual ~breakableBridgeDemo();

    virtual const char* getTitle();

    virtual void display();

    virtual void update();

    virtual void key(unsigned char key);
};

// 생성자를 선언하자.
breakableBridgeDemo::breakableBridgeDemo()
:
MassAggregateApplication(12), cables(0), supports(0), rods(0),
massPos(0,0,0.5f) //12는 particleCount.
{
    for (unsigned i = 0; i < 12; i++)
    {
        unsigned x = (i%12)/2;
        particleArray[i].setPosition(
            cyclone::real(i/2)*2.0f-5.0f,
            4,
            cyclone::real(i%2)*2.0f-1.0f
            );
        particleArray[i].setVelocity(0,0,0);
        particleArray[i].setDamping(0.9f);
        particleArray[i].setAcceleration(cyclone::Vector3::GRAVITY);
        particleArray[i].clearAccumulator();
    }

    // 입자와 입자를 세로로(x축 방향으로) 연결해 줄 cables 생성
    int i = 0; 
    for (int count = 0; count < 10; count++) // 12개 입자를 엮는 10개의 케이블이라고 가정
    {
        // 1. 메모리에 새로운 케이블 객체를 하나 생성
        ParticleCable* newCable = new ParticleCable(); 
    
        // 2. 객체의 데이터 초기화
        newCable->particle[0] = &particleArray[i];
        newCable->particle[1] = &particleArray[i+2];
        newCable->maxLength = 1.9f;
        newCable->restitution = 0.3f;
    
        // 3. 세팅이 끝난 객체를 벡터와 world의 ContactGenerators에 넣는다.
        cables.push_back(newCable); 
        world.getContactGenerators().push_back(newCable);
        
        i++;
    }

    // supports 생성
    i = 0; 
    for (int count = 0; count < 12; count++) // 12개의 지지대
    {
        ParticleCableConstraint* newSupport = new ParticleCableConstraint();
    
        newSupport->particle = &particleArray[i];
        newSupport->anchor = cyclone::Vector3(cyclone::real(i/2)*2.2f-5.5f,
            6,
            cyclone::real(i%2)*1.6f-0.8f);
        if (i < 6) newSupport->maxLength = cyclone::real(i/2)*0.5f + 3.0f;
        else newSupport->maxLength = 5.5f - cyclone::real(i/2)*0.5f;
        newSupport->restitution = 0.5f;
    
        supports.push_back(newSupport);
        world.getContactGenerators().push_back(newSupport);
        
        i++;
    }

    // rods 생성
    i = 0;
    for (int count = 0; count < 6; count++) // 6개의 막대
    {
        ParticleRod* newRod = new ParticleRod();
    
        newRod->particle[0] = &particleArray[i*2];
        newRod->particle[1] = &particleArray[i*2+1];
        newRod->length = 2.0f;
        
        rods.push_back(newRod);
        world.getContactGenerators().push_back(newRod);
        
        i++;
    }
    updateAdditionalMass();
}

breakableBridgeDemo::~breakableBridgeDemo()
{
    for (auto g = cables.begin(); g != cables.end(); g++) {
        delete (*g); 
    }
    cables.clear();

    for (auto g = supports.begin(); g != supports.end(); g++) {
        delete (*g); 
    }
    supports.clear();

    for (auto g = rods.begin(); g != rods.end(); g++) {
        delete (*g); 
    }
    rods.clear();
}

// 매 프레임 실행되는 함수. 
void breakableBridgeDemo::updateAdditionalMass()
{
    for (unsigned i = 0; i < 12; i++)
    {
        particleArray[i].setMass(BASE_MASS);
    }

    // 현재 구체가 이동한 위치를 정수와 소수점으로 나눈다.
    int x = int(massPos.x); // 캐스팅
    //  massPos.x를 1로 나눈 나머지를 저장(소수점)
    cyclone::real xp = real_fmod(massPos.x, cyclone::real(1.0f));

    // 최댓값 보정 알고리즘
    if (x < 0)
    {
        x = 0;
        xp = 0;
    }
    if (x >= 5)
    {
        x = 5;
        xp = 0;
    }
    // 다리 위의 x좌표는 최대 5

    int z = int(massPos.z);
    cyclone::real zp = real_fmod(massPos.z, cyclone::real(1.0f));
    if (z < 0)
    {
        z = 0;
        zp = 0;
    }
    if (z >= 1)
    {
        z = 1;
        zp = 0;
    }
    // 다리 위의 z좌표는 최대 1

    // 실제 구체를 그릴 곳의 벡터를 초기화
    massDisplayPos.clear();

    // 위치 비율로 질량을 분배하자.

    // 기준점이 되는 입자 구하기
    particleArray[x*2+z].setMass(BASE_MASS + EXTRA_MASS*(1-xp)*(1-zp));
    massDisplayPos.addScaledVector(
        particleArray[x*2+z].getPosition(), (1-xp)*(1-zp)
        );

    
    if (xp > 0) // x방향으로 조금이라도 이동했다면
    {
        
        particleArray[x*2+z+2].setMass(BASE_MASS + EXTRA_MASS*xp*(1-zp));
        massDisplayPos.addScaledVector(
            particleArray[x*2+z+2].getPosition(), xp*(1-zp)
            );

        if (zp > 0) // x로도 이동했는데 z로도 이동했다면
        {
            particleArray[x*2+z+3].setMass(BASE_MASS + EXTRA_MASS*xp*zp);
            massDisplayPos.addScaledVector(
                particleArray[x*2+z+3].getPosition(), xp*zp
                );
        }
    }
    if (zp > 0)
    {
        particleArray[x*2+z+1].setMass(BASE_MASS + EXTRA_MASS*(1-xp)*zp);
        massDisplayPos.addScaledVector(
            particleArray[x*2+z+1].getPosition(), (1-xp)*zp
            );
    }  

    // 위치에 따른 질량분배 후, 모든 particles를 돌며 질량이 10.0f를 넘는 입자를 찾아 연결을 끊는다.
    for(int i=0 ;i<12; i++)
    {
        if(particleArray[i].getMass() >= 10.0f)
        {
            Particle* heavy = &particleArray[i];
            for(auto it = supports.begin();it!=supports.end();it++)
            {
                if(heavy == (*it)->particle)
                {
                    Particle* dummy1 = new Particle();
                    dummy1->setPosition(heavy->getPosition());
                    dummy1->setMass(0.3f);
                    dummy1->setVelocity(0,0,0);
                    dummy1->setDamping(0.9f);
                    dummy1->setAcceleration(cyclone::Vector3::GRAVITY);
                    (*it)->particle = dummy1;
                    world.getParticles().push_back(dummy1);

                }
            }
            for(auto it = cables.begin();it!=cables.end();it++)
            {
                if(heavy == (*it)->particle[0])
                {
                    Particle* dummy2 = new Particle();
                    dummy2->setPosition(heavy->getPosition());
                    dummy2->setMass(0.3f);
                    dummy2->setVelocity(0,0,0);
                    dummy2->setDamping(0.9f);
                    dummy2->setAcceleration(cyclone::Vector3::GRAVITY);
                    (*it)->particle[0] = dummy2;
                    world.getParticles().push_back(dummy2);
                }
                if (heavy == (*it)->particle[1])
                {
                    Particle* dummy2 = new Particle();
                    dummy2->setPosition(heavy->getPosition());
                    dummy2->setMass(0.3f);
                    dummy2->setVelocity(0,0,0);
                    dummy2->setDamping(0.9f);
                    dummy2->setAcceleration(cyclone::Vector3::GRAVITY);
                    (*it)->particle[1] = dummy2;
                    world.getParticles().push_back(dummy2);
                }
            }
            for(auto it = rods.begin();it!=rods.end();it++)
            {
                if(heavy == (*it)->particle[0])
                {
                    Particle* dummy3 = new Particle();
                    dummy3->setPosition(heavy->getPosition());
                    dummy3->setMass(0.3f);
                    dummy3->setVelocity(0,0,0);
                    dummy3->setDamping(0.9f);
                    dummy3->setAcceleration(cyclone::Vector3::GRAVITY);
                    (*it)->particle[0] = dummy3;
                    world.getParticles().push_back(dummy3);
                }
                if (heavy == (*it)->particle[1])
                {
                    Particle* dummy3 = new Particle();
                    dummy3->setPosition(heavy->getPosition());
                    dummy3->setMass(0.3f);
                    dummy3->setVelocity(0,0,0);
                    dummy3->setDamping(0.9f);
                    dummy3->setAcceleration(cyclone::Vector3::GRAVITY); 
                    (*it)->particle[1] = dummy3; 
                    world.getParticles().push_back(dummy3);
                }
            }
        }
    }

}

void breakableBridgeDemo::display()
{
    MassAggregateApplication::display();

    glBegin(GL_LINES);
    glColor3f(0,0,1);
    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        if(rods[i]->particle[0]!=NULL && rods[i]->particle[1] != NULL)
        {
        cyclone::Particle **particles = rods[i]->particle;
        const cyclone::Vector3 &p0 = particles[0]->getPosition();
        const cyclone::Vector3 &p1 = particles[1]->getPosition();
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
        }
    }

    glColor3f(0,1,0);
    for (unsigned i = 0; i < CABLE_COUNT; i++)
    {
        if(cables[i]->particle[0]!=NULL && cables[i]->particle[1] != NULL)
        {
        cyclone::Particle **particles = cables[i]->particle;
        const cyclone::Vector3 &p0 = particles[0]->getPosition();
        const cyclone::Vector3 &p1 = particles[1]->getPosition();
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
        }
    }

    glColor3f(0.7f, 0.7f, 0.7f);
    for (unsigned i = 0; i < SUPPORT_COUNT; i++)
    {
        if(supports[i]->particle!=NULL)
        {
        const cyclone::Vector3 &p0 = supports[i]->particle->getPosition();
        const cyclone::Vector3 &p1 = supports[i]->anchor;
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
        }

    }
    glEnd();

    glColor3f(1,0,0);
    glPushMatrix();
    glTranslatef(massDisplayPos.x, massDisplayPos.y+0.25f, massDisplayPos.z);
    glutSolidSphere(0.25f, 20, 10);
    glPopMatrix();
}

void breakableBridgeDemo::update()
{
    MassAggregateApplication::update();

    updateAdditionalMass();
}

const char* breakableBridgeDemo::getTitle()
{
    return "Cyclone > breakableBridge Demo";
}

void breakableBridgeDemo::key(unsigned char key)
{
    switch(key)
    {
    case 's': case 'S':
        massPos.z += 0.1f;
        if (massPos.z > 1.0f) massPos.z = 1.0f;
        break;
    case 'w': case 'W':
        massPos.z -= 0.1f;
        if (massPos.z < 0.0f) massPos.z = 0.0f;
        break;
    case 'a': case 'A':
        massPos.x -= 0.1f;
        if (massPos.x < 0.0f) massPos.x = 0.0f;
        break;
    case 'd': case 'D':
        massPos.x += 0.1f;
        if (massPos.x > 5.0f) massPos.x = 5.0f;
        break;

    default:
        MassAggregateApplication::key(key);
    }

    updateAdditionalMass();
}


Application* getApplication()
{
    return new breakableBridgeDemo();
}