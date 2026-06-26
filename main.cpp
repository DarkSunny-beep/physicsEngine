#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // 1. GLFW 초기화
    if (!glfwInit()) return -1;

    // 2. 창 생성
    GLFWwindow* window = glfwCreateWindow(800, 600, "Physics Engine View", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // 3. 렌더링 루프 (창이 닫힐 때까지 반복)
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        // 화면 색깔 바꾸기 (테스트용)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}