/************************************************************************/
/*																										    */
/************************************************************************/

#include "logorenderer.h"
#include <QtGui/qmatrix4x4.h>


/************************************************************************/
/*																										    */
/************************************************************************/

GLuint VAO, VBO;

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::initialize()
{
    initializeOpenGLFunctions();
//    printContext();

    ourShaderTest_Init();

    m_fAngle = 0;
    m_fScale = 1;
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::render()
{
    ourShaderTest();

    m_fAngle += 1.0f;
    glFlush();
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::ourShaderTest_Init()
{
    bool exception = createShader(m_nProgram, ":/shader/shader.vert", ":/shader/shader.frag");
    if (exception)
    {
        qDebug() << "ERROR::CREATE::SHADER";
    }

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions         // Colors
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  -0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1. bind VAO
    glBindVertexArray(VAO);

    // 2.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 3.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //4.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::ourShaderTest()
{
    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_nProgram);

    //Uniform offset
    GLfloat offset = 0.5;
    glUniform1f(glGetUniformLocation(m_nProgram, "xOffset"), offset);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

}

/************************************************************************/
/*																										    */
/************************************************************************/

LogoRenderer::~LogoRenderer()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(m_nProgram);
}

/************************************************************************/
/*																										    */
/************************************************************************/

LogoRenderer::LogoRenderer()
{
}

/************************************************************************/
/*																										    */
/************************************************************************/

bool LogoRenderer::createShader(GLuint& program, QString vertexPath, QString fragmentPath) {
    //    QString vertexPath = ":/shader/shader.vert";
    //    QString fragmentPath = ":/shader/shader.frag";
    bool exception = false;

    // 1. get code
    QString vertexCode;
    QString fragmentCode;

    // open file
    QFile vShaderFile(vertexPath);
    QFile fShaderFile(fragmentPath);

    // read vShader txt
    if (vShaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&vShaderFile);
        vertexCode = in.readAll();
        vShaderFile.close();
    } else { exception = true; }

    //        qDebug() << vertexCode;

    // read fShader txt
    if (fShaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fShaderFile);
        fragmentCode = in.readAll();
        fShaderFile.close();
    } else { exception = true; }

    //        qDebug() << fragmentCode;


    if (exception)
    {
        qDebug() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
        return exception;
    }

    // QString to GLchar*
    QByteArray ba1 = vertexCode.toUtf8();
    const GLchar* vShaderCode = ba1.data();
    QByteArray ba2 = fragmentCode.toUtf8();
    const GLchar* fShaderCode = ba2.data();

    //        qDebug() << vShaderCode << "\n " << fShaderCode;

    // 2. compile shader
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // print error
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
    }

    // fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print error
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }

    // link program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    // print link error
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
    }

    // delete shader
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return exception;
}






