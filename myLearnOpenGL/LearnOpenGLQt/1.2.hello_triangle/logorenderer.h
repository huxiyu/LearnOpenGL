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


private:
    qreal   m_fAngle;
    qreal   m_fScale;

    // VAO VBO glDrawArrays
    void vao_vbo_glDrawArrays_Init();
    void vao_vbo_glDrawArrays();

    // VAO EBO VBO glDrawElements
    void vao_vbo_ebo_glDrawElements_Init();
    void vao_vbo_ebo_glDrawElements();
};

/************************************************************************/
/*																										    */
/************************************************************************/

#endif

/************************************************************************/
/*																										    */
/************************************************************************/
