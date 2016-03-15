/************************************************************************/
/*																										    */
/************************************************************************/

#ifndef LOGORENDERER_H
#define LOGORENDERER_H

/************************************************************************/
/*																										    */
/************************************************************************/

#include "openglcontexttool.h"
#include <QtGui/qopenglfunctions_3_3_core.h>

/************************************************************************/
/*																										    */
/************************************************************************/

class LogoRenderer : protected QOpenGLFunctions_3_3_Core
{
public:
    LogoRenderer();
    ~LogoRenderer();

    void render();
    void initialize();
    bool createShader(GLuint& program, QString vertexPath, QString fragmentPath);

private:
    float m_fps;

    qreal   m_fAngle;
    qreal   m_fScale;

    // Program ID
    GLuint m_nProgram;

    // VBO VAO EBO
    GLuint VBO, VAO, EBO;

    // 纹理
    GLuint texture1;
    GLuint texture2;

    // VAO VBO glDrawArrays
    void textureTest_Init();
    void textureTest();
};

/************************************************************************/
/*																										    */
/************************************************************************/

#endif

/************************************************************************/
/*																										    */
/************************************************************************/
