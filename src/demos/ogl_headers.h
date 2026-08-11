// 현재 사용중인 운영체제(OS)에 맞춰 OpenGL을 안전하게 불러오기 위한 헤더 파일.
// 전처리기
#pragma once
#ifdef __APPLE__
    #include <GLUT/glut.h> //나의 경우 맥이므로 이 #include ~ 만 살아남는다.
#elif defined(__gnu_linux__)
    #include <GL/glut.h>
#else
    #include <gl/glut.h>
#endif

