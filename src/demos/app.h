
// 이 책의 demos를 실행하기 위해 미리 짜두는 미니 프레임워크의 핵심 뼈대
#pragma once
#include <cstdlib>

#include <cyclone/cyclone.h>

class Application //모든 데모프로그램의 부모가 되는 최소 조상 클래스
{
protected:

    int height;
    int width;
    //현재 띄워진 창의 높이와 너비 기억 용도

public:
    
    virtual const char* getTitle();

    //화면을 그릴 준비
    virtual void initGraphics();

    // 3D화면을 바라보는 시야각 설정
    virtual void setView();
    
    virtual void deinit();

    //매 프레임마다 화면에 물체를 그리는 함수
    virtual void display();

    //매 프레임마다 물리 엔진 상태 갱신. integrate함수 같은 게 여기서 발생
    virtual void update();

    virtual void key(unsigned char key);

    virtual void resize(int width, int height);

    virtual void mouse(int button, int state, int x, int y);

    virtual void mouseDrag(int x, int y);

    
    void renderText(float x, float y, const char *text, void* font=NULL);
};


class MassAggregateApplication : public Application
{
protected:
    cyclone::ParticleWorld world;
    cyclone::Particle *particleArray;
    cyclone::GroundContacts groundContactGenerator;

public:
    MassAggregateApplication(unsigned int particleCount);
    virtual ~MassAggregateApplication();

    virtual void update();
    virtual void initGraphics();
    virtual void display();
    // 업데이트, 그래픽 렌더링, 디스플레이.
};

/**
 * This application adds additional functionality used in many of the
 * demos. This includes the ability to track contacts (for rigid bodies)
 * and move the camera around.
 */
 class RigidBodyApplication : public Application
 {
 protected:
    /** Holds the maximum number of contacts. */
    const static unsigned maxContacts = 256;

    // /** Holds the array of contacts. */
    // cyclone::Contact contacts[maxContacts];

    // /** Holds the collision data structure for collision detection. */
    // cyclone::CollisionData cData;

    // /** Holds the contact resolver. */
    // cyclone::ContactResolver resolver;

    /** Holds the camera angle. */
    float theta;

    /** Holds the camera elevation. */
    float phi;

    /** Holds the position of the mouse at the last frame of a drag. */
    int last_x, last_y;

    /** True if the contacts should be rendered. */
    bool renderDebugInfo;

    /** True if the simulation is paused. */
    bool pauseSimulation;

    /** Pauses the simulation after the next frame automatically */
    bool autoPauseSimulation;

    /** Processes the contact generation code. */
    virtual void generateContacts() = 0;

    /** Processes the objects in the simulation forward in time. */
    virtual void updateObjects(cyclone::real duration) = 0;

    /**
     * Finishes drawing the frame, adding debugging information
     * as needed.
     */
    void drawDebug();

    /** Resets the simulation. */
    virtual void reset() = 0;

public:
    /**
     * Creates a new application object.
     */
    RigidBodyApplication();

    /** Display the application. */
    virtual void display();

    /** Update the objects. */
    virtual void update();

    /** Handle a mouse click. */
    virtual void mouse(int button, int state, int x, int y);

    /** Handle a mouse drag */
    virtual void mouseDrag(int x, int y);

    /** Handles a key press. */
    virtual void key(unsigned char key);
 };