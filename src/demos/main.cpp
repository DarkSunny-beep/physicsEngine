
// Include appropriate OpenGL headers.
#include "ogl_headers.h"

// Include the general application structure.
#include "app.h"

// Include the timing functions
#include "timing.h"

// 특정 데모를 위한 것. 각각의데모.cpp 에 이 함수의 알맹이를 선언 해줘야 한다.
extern Application* getApplication();
// extern: 공유용.(알맹이 딴 데 있다고 알려주는 느낌) 

// main.cpp에서 전역 Application 변수를 설정한다
Application* app;

//glut의 함수들을 이용해 창을 만든다(ogl_headers를 통해 불러와짐)
void createWindow(const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640,320);
    glutInitWindowPosition(0,0);
    glutCreateWindow(title);
}

/**
 * Called each frame to update the 3D scene. Delegates to
 * the application.
 */
void update()
{
    // Update the timing.
    TimingData::get().update();

    // Delegate to the application.
    app->update();
}

/**
 * Called each frame to display the 3D scene. Delegates to
 * the application.
 */
void display()
{
    app->display();

    // Update the displayed content.
    glFlush();
    glutSwapBuffers();
}

/**
 * Called when a mouse button is pressed. Delegates to the
 * application.
 */
void mouse(int button, int state, int x, int y)
{
    app->mouse(button, state, x, y);
}

/**
 * Called when the display window changes size.
 */
void reshape(int width, int height)
{
    app->resize(width, height);
}

/**
 * Called when a key is pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
    // Note we omit passing on the x and y: they are rarely needed.
    app->key(key);
}

/**
 * Called when the mouse is dragged.
 */
void motion(int x, int y)
{
    app->mouseDrag(x, y);
}

/**
 * The main entry point. We pass arguments onto GLUT.
 */
int main(int argc, char** argv)
{
    // Set up GLUT and the timers
    glutInit(&argc, argv);
    TimingData::init();

    // Create the application and its window
    app = getApplication();
    createWindow(app->getTitle());

    // Set up the appropriate handler functions
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // Run the application
    app->initGraphics();
    glutMainLoop();

    // Clean up the application
    app->deinit();
    delete app;
    TimingData::deinit();
}