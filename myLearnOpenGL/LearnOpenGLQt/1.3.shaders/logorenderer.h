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
    qreal   m_fAngle;
    qreal   m_fScale;

    // Program ID
    GLuint m_nProgram;

    // VAO VBO glDrawArrays
    void ourShaderTest_Init();
    void ourShaderTest();
};

/************************************************************************/
/*																										    */
/************************************************************************/

#endif

/************************************************************************/
/*																										    */
/************************************************************************/
