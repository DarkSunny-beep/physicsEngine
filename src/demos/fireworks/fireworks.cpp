#include <cyclone/cyclone.h>
#include "../ogl_headers.h" 
#include "../app.h"
#include "../timing.h"

#include <stdio.h>

static cyclone::Random crandom;


class Firework : public cyclone::Particle
{
    public:
        unsigned type; //언젠간 사라져야 하므로 type 지정이 필요
        cyclone::real age;

        // 회전효과를 넣어보자
        cyclone::real rotation; // 몇 도 돌아가있는지(rad)
        cyclone::real rotationSpeed; // 회전 속도

        bool update(cyclone::real duration)
        {
            integrate(duration);
            rotation += rotationSpeed*duration;
            rotation = real_fmod(rotation, 2.0f * R_PI);

            age-=duration;
            return (age<0) || (position.y<0);
        }
};

struct FireworkRule
{
    unsigned type;
    cyclone::real minAge;

    cyclone::real maxAge;

    cyclone::Vector3 minVelocity;

    cyclone::Vector3 maxVelocity;

    cyclone::real damping;

    // firework의 fuse가 끝났을 경우 새로 생성되는 firework
    struct Payload
    {
        unsigned type;

        // payload의 개수
        unsigned count;

        // payload의 특성을 한 번에 setting
        void set(unsigned type, unsigned count)
        {
            this->type = type;
            this->count = count;
            // same of (*this).type, Payload::type
        }
    };

    /** The number of payloads for this firework type. */
    unsigned payloadCount;

    // set of payloads(array)
    Payload *payloads;

    FireworkRule():payloadCount(0), payloads(NULL) {} // default constructor

    void init(unsigned payloadCount) // 초기화 함수 
    {
        FireworkRule::payloadCount = payloadCount;
        payloads = new Payload[payloadCount]; // 동적 메모리 할당
    }

    ~FireworkRule()
    {
        if (payloads != NULL) delete[] payloads;
    }
    // 소멸자(destructor). 생성자에서 new 로 생성하였기 때문에 소멸자에서 delete를 해줌
    // 생성자에서도 new [] 였으므로 소멸자에서도 한번에 여러개를 삭제 해줘야 하므로 delete []를 사용

    /**
     * Set all the rule parameters in one go.
     */
    void setParameters(unsigned type, cyclone::real minAge, cyclone::real maxAge,
        const cyclone::Vector3 &minVelocity, const cyclone::Vector3 &maxVelocity,
        cyclone::real damping)
    {
        FireworkRule::type = type;
        FireworkRule::minAge = minAge;
        FireworkRule::maxAge = maxAge;
        FireworkRule::minVelocity = minVelocity;
        FireworkRule::maxVelocity = maxVelocity;
        FireworkRule::damping = damping;
    }

    // 해당 FireworkRule을 따르는 새로운 firework를 생성, 매개변수로 전달 받은 인스턴스에 그 데이터를 덮어쓰기
    // 부모 firework는 새 폭죽의 위치, 초기 속도의 기준을 잡는 데 사용됨
    // fireworkRule은 단순히 rule이고, 이 함수는 그 룰에서 실제 firework하나를 만들어 냄(데이터 덮어쓰기)
    void create(Firework *firework, const Firework *parent = NULL) const
    {
        firework->type = type;
        firework->age = crandom.randomReal(minAge, maxAge);
        //  minAge~maxAge 사이 랜덤 실수로 age 설정
        
        //  회전 효과 추가
        firework->rotation = 0.0f;

        cyclone::Random randomGen;
        firework->rotationSpeed = randomGen.randomReal(-3.14159f,3.14159f);

        cyclone::Vector3 vel;
        // 기본 생성자로 인해 x, y, z가 모두 0으로 초기화 됨
        if (parent) {
            // parent에 맞춰서 위치, 초기 속도 설정
            firework->setPosition(parent->getPosition());
            vel += parent->getVelocity();
        }
        else
        {
            cyclone::Vector3 start;
            int x = (int)crandom.randomInt(3) - 1;
            start.x = 5.0f * cyclone::real(x);
            firework->setPosition(start);
        }

        vel += crandom.randomVector(minVelocity, maxVelocity);
        firework->setVelocity(vel);

        //  모든 폭죽의 질량은 1로 사용한다. 폭죽이 오직 중력의 영향만 받으므로 각기 다른 질량은 필요없다.
        firework->setMass(1);

        firework->setDamping(damping);

        firework->setAcceleration(cyclone::Vector3::GRAVITY);

        firework->clearAccumulator();
        // 힘 벡터 0으로 초기화
    }
};

// FireworksDemo application을 실행하기 위한 class
class FireworksDemo : public Application
{
    /**
     * Holds the maximum number of fireworks that can be in use.
     */
    const static unsigned maxFireworks = 1024;
    // FireworksDemo마다 다른 게 아닌 하나의 고정된 변수.

    Firework fireworks[maxFireworks];

    /** Holds the index of the next firework slot to use. */
    unsigned nextFirework;

    /** And the number of rules. */
    const static unsigned ruleCount = 9;

    /** Holds the set of rules. */
    FireworkRule rules[ruleCount];

    /** Dispatches a firework from the origin. */
    void create(unsigned type, const Firework *parent=NULL);

    /** Dispatches the given number of fireworks from the given parent. */
    void create(unsigned type, unsigned number, const Firework *parent);

    /** Creates the rules. */
    void initFireworkRules();

public:
    /** Creates a new demo object. */
    FireworksDemo();
    ~FireworksDemo();
    // 생성자와 소멸자

    /** Sets up the graphic rendering. */
    virtual void initGraphics();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Update the particle positions. */
    virtual void update();

    /** Display the particle positions. */
    virtual void display();

    /** Handle a keypress. */
    virtual void key(unsigned char key);
};

// 구체적으로 방법 정의
FireworksDemo::FireworksDemo():nextFirework(0)
{
    // Make all shots unused
    for (Firework *firework = fireworks; firework < fireworks+maxFireworks;
         firework++)
    {
        firework->type = 0;
    }

    // Create the firework types
    initFireworkRules();
}

FireworksDemo::~FireworksDemo()
{
}

// 클래스의 private안에 정의된 함수들의 알맹이들을 정의
void FireworksDemo::initFireworkRules()
{
    // Go through the firework types and create their rules.
    rules[0].init(2); //payloadCount = 2 로 초기화
    rules[0].setParameters(
        1, // type
        0.5f, 1.4f, // age range
        cyclone::Vector3(-5, 25, -5), // min velocity
        cyclone::Vector3(5, 28, 5), // max velocity
        0.1 // damping
        );
    rules[0].payloads[0].set(3, 5); // type, count
    rules[0].payloads[1].set(5, 5);

    rules[1].init(1);
    rules[1].setParameters(
        2, // type
        0.5f, 1.0f, // age range
        cyclone::Vector3(-5, 10, -5), // min velocity
        cyclone::Vector3(5, 20, 5), // max velocity
        0.8 // damping
        );
    rules[1].payloads[0].set(4, 2);

    rules[2].init(0);
    rules[2].setParameters(
        3, // type
        0.5f, 1.5f, // age range
        cyclone::Vector3(-5, -5, -5), // min velocity
        cyclone::Vector3(5, 5, 5), // max velocity
        0.1 // damping
        );

    rules[3].init(0);
    rules[3].setParameters(
        4, // type
        0.25f, 0.5f, // age range
        cyclone::Vector3(-20, 5, -5), // min velocity
        cyclone::Vector3(20, 5, 5), // max velocity
        0.2 // damping
        );

    rules[4].init(1);
    rules[4].setParameters(
        5, // type
        0.5f, 1.0f, // age range
        cyclone::Vector3(-20, 2, -5), // min velocity
        cyclone::Vector3(20, 18, 5), // max velocity
        0.01 // damping
        );
    rules[4].payloads[0].set(3, 5);

    rules[5].init(0);
    rules[5].setParameters(
        6, // type
        3, 5, // age range
        cyclone::Vector3(-5, 5, -5), // min velocity
        cyclone::Vector3(5, 10, 5), // max velocity
        0.95 // damping
        );

    rules[6].init(1);
    rules[6].setParameters(
        7, // type
        4, 5, // age range
        cyclone::Vector3(-5, 50, -5), // min velocity
        cyclone::Vector3(5, 60, 5), // max velocity
        0.01 // damping
        );
    rules[6].payloads[0].set(8, 10);

    rules[7].init(0);
    rules[7].setParameters(
        8, // type
        0.25f, 0.5f, // age range
        cyclone::Vector3(-1, -1, -1), // min velocity
        cyclone::Vector3(1, 1, 1), // max velocity
        0.01 // damping
        );

    rules[8].init(0);
    rules[8].setParameters(
        9, // type
        3, 5, // age range
        cyclone::Vector3(-15, 10, -5), // min velocity
        cyclone::Vector3(15, 15, 5), // max velocity
        0.95 // damping
        );
    // ... and so on for other firework types ...
}

void FireworksDemo::initGraphics()
{
    // Call the superclass
    Application::initGraphics();

    // But override the clear color
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
}

const char* FireworksDemo::getTitle()
{
    return "Cyclone > Fireworks Demo";
}

void FireworksDemo::create(unsigned type, const Firework *parent)
{
    // Get the rule needed to create this firework
    FireworkRule *rule = rules + (type - 1);

    // Create the firework
    rule->create(fireworks+nextFirework, parent);

    // Increment the index for the next firework
    nextFirework = (nextFirework + 1) % maxFireworks;

    
}

void FireworksDemo::create(unsigned type, unsigned number, const Firework *parent)
{
    for (unsigned i = 0; i < number; i++)
    {
        create(type, parent);
    }
}

void FireworksDemo::update()
{
    // Find the duration of the last frame in seconds
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

    for (Firework *firework = fireworks;
         firework < fireworks+maxFireworks;
         firework++)
    {
        // Check if we need to process this firework.
        if (firework->type > 0)
        {
            // Does it need removing?
            if (firework->update(duration))
            {
                // Find the appropriate rule
                FireworkRule *rule = rules + (firework->type-1);

                // Delete the current firework (this doesn't affect its
                // position and velocity for passing to the create function,
                // just whether or not it is processed for rendering or
                // physics.
                firework->type = 0;

                // Add the payload
                for (unsigned i = 0; i < rule->payloadCount; i++)
                {
                    FireworkRule::Payload * payload = rule->payloads + i;
                    create(payload->type, payload->count, firework);
                }
            }
        }
    }

    Application::update();
}

void FireworksDemo::display()
{
    const static cyclone::real size = 0.1f;

    // Clear the viewport and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 4.0, 15.0,  0.0, 3.0, 0.0,  0.0, 1.0, 0.0);
    // 카메라의 위치. +z 는 사용자쪽으로 다가오는 방향 / 카메라가 쳐다보는 곳 / 내 정수리 방향

    // 1. 티나는 바닥 그리기
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.15f); // 어두운 남색 계열의 바닥 색상
    glBegin(GL_QUADS);
    glVertex3f(-20.0f, 0.0f, -20.0f); // y=0 높이에 넓게 사각형
    glVertex3f( 20.0f, 0.0f, -20.0f);
    glVertex3f( 20.0f, 0.0f,  20.0f);
    glVertex3f(-20.0f, 0.0f,  20.0f);
    glEnd();
    glPopMatrix();

    // 2. 투명도 기능
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Render each firework in turn
    for (Firework *firework = fireworks;
        firework < fireworks+maxFireworks;
        firework++)
    {
        // Check if we need to process this firework.
        if (firework->type > 0)
        {
            float r =1.0f, g=1.0f, b=1.0f;
            switch (firework->type)
            {
                case 1: r=1.0f; g=0.0f; b=0.0f; break;
                case 2: r=1.0f; g=0.5f; b=0.0f; break;
                case 3: r=1.0f; g=1.0f; b=0.0f; break;
                case 4: r=0.0f; g=1.0f; b=0.0f; break;
                case 5: r=0.0f; g=1.0f; b=1.0f; break;
                case 6: r=0.4f; g=0.4f; b=1.0f; break;
                case 7: r=1.0f; g=0.0f; b=1.0f; break;
                case 8: r=1.0f; g=1.0f; b=1.0f; break;
                case 9: r=1.0f; g=0.5f; b=0.5f; break;
            };

            const cyclone::Vector3 &pos = firework->getPosition();

            // 1. 폭죽 그리기
            glPushMatrix(); // 현재 화면 좌표계 저장(중심점을 스택에 push)
            
            // 폭죽의 위치로 붓(기준점)을 이동
            glTranslatef(pos.x, pos.y, pos.z);
            
            // Z축을 기준으로 폭죽의 rotation 각도만큼 회전 (라디안 -> 디그리 변환)
            glRotatef(firework->rotation * 180.0f / 3.141592f, 0.0f, 0.0f, 1.0f);

            glColor3f(r,g,b);  
            // 네모(Quad) 그리기 시작
            // (이미 이동했으므로 pos.x 등을 더할 필요 없이 0,0,0 기준으로 그림)
            glBegin(GL_QUADS);
            glVertex3f(-size, -size, 0.0f);
            glVertex3f( size, -size, 0.0f);
            glVertex3f( size,  size, 0.0f);
            glVertex3f(-size,  size, 0.0f);
            glEnd();
            glPopMatrix(); // 화면 좌표계 원상복구(다시 중심점으로 이동(0,0,0))

            // 2. 반사된 폭죽(reflection) 그리기
            glPushMatrix(); 
            glTranslatef(pos.x, -pos.y, pos.z);
            // 반사된 위치이므로 Y축만 마이너스(-pos.y)로 이동
            
            // 반사된 이미지는 반대로 도는 게 자연스러우므로 -를 곱함(좌우 반전 느낌)
            glRotatef(firework->rotation * -180.0f / 3.141592f, 0.0f, 0.0f, 1.0f);
            
            glColor4f(r,g,b,0.2f);
            glDisable(GL_DEPTH_TEST);
            //y<0으로 반사되어 바닥에 가려지며 그리질 못하는 상황을 없앰

            glBegin(GL_QUADS);
            glVertex3f(-size, -size, 0.0f);
            glVertex3f( size, -size, 0.0f);
            glVertex3f( size,  size, 0.0f);
            glVertex3f(-size,  size, 0.0f);
            glEnd();

            glDisable(GL_DEPTH_TEST);
            
            glPopMatrix(); 
        }
    }
    glDisable(GL_BLEND);
}

void FireworksDemo::key(unsigned char key)
{
    switch (key)
    {
    case '1': create(1, 1, NULL); break;
    case '2': create(2, 1, NULL); break;
    case '3': create(3, 1, NULL); break;
    case '4': create(4, 1, NULL); break;
    case '5': create(5, 1, NULL); break;
    case '6': create(6, 1, NULL); break;
    case '7': create(7, 1, NULL); break;
    case '8': create(8, 1, NULL); break;
    case '9': create(9, 1, NULL); break;
    }
}





/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new FireworksDemo();
}