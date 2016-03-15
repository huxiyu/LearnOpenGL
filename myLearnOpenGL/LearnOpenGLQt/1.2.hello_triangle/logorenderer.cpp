/************************************************************************/
/*																										    */
/************************************************************************/

#include "logorenderer.h"
#include <QtGui/qmatrix4x4.h>
#include <QDateTime>

/************************************************************************/
/*																										    */
/************************************************************************/

// shaderProgram
GLint shaderProgram;

// Shaders
const GLchar* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 transform;\n"
        "void main()\n"
        "{\n"
        "gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";
const GLchar* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

GLuint VAO, VBO, EBO;

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::initialize()
{
    initializeOpenGLFunctions();
    printContext();

    /* Build and compile our shader program */
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
    }

    // Fragment shader
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time error
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }
    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    vao_vbo_glDrawArrays_Init();
    //    vao_vbo_ebo_glDrawElements_Init();

    m_fAngle = 0;
    m_fScale = 1;
    m_fps = 0.0f;
}

/************************************************************************/
/*																										    */
/************************************************************************/
// render model
static qint64 lastTime = QDateTime::currentMSecsSinceEpoch();
static int nbFrames = 0;
static qint64 currentTime;
void LogoRenderer::render()
{
    // Measure speed
    currentTime = QDateTime::currentMSecsSinceEpoch();
    nbFrames++;
    if ( currentTime - lastTime >= 1000.0 ){ // If last prinf() was more than 1 sec ago
        // printf and reset timer
        m_fps = nbFrames;
        //std::cout << 1000.0/double(nbFrames) << "  ms/frame  " << nbFrames << std::endl;
        nbFrames = 0;
        lastTime += 1000.0;
        //        lastTime = currentTime;
        if (m_fps > 50)
        { qDebug() << "FPS::" << m_fps;}
    }


    vao_vbo_glDrawArrays();
    //    vao_vbo_ebo_glDrawElements();


    m_fAngle += 1.0f;
    glFlush();
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::vao_vbo_glDrawArrays_Init()
{
    /* Set up vertex data (and buffer(s)) and attribute pointers */
    GLfloat vertices[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f,  -0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);

    // 2. 把我们的顶点数组复制到一个顶点缓冲中，提供给OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //4. 解绑 VBO VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::vao_vbo_glDrawArrays()
{
    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    glUseProgram(shaderProgram);

    QMatrix4x4 modelview;
    modelview.rotate(m_fAngle, 0.0f, 1.0f, 0.0f);
    modelview.rotate(m_fAngle, 1.0f, 0.0f, 0.0f);
    modelview.rotate(m_fAngle, 0.0f, 0.0f, 1.0f);
    modelview.scale(m_fScale);
    modelview.translate(0.0f, -0.2f, 0.0f);

    // Uniform offset
    // Get matrix's uniform location and set matrix
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, modelview.data());
    //            glUniform1f(glGetUniformLocation(ourShader.Program, "xOffset"), offset);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::vao_vbo_ebo_glDrawElements_Init()
{
    /* Set up vertex data (and buffer(s)) and attribute pointers */
    GLfloat vertices[] = {
        0.5f, -0.5f, 0.0f,   // 右上角
        0.5f, 0.5f, 0.0f,  // 右下角
        -0.5f, 0.5f, 0.0f, // 左下角
        -0.5f, -0.5f, 0.0f   // 左上角
    };

    GLuint indices[] = { // 起始于0!
                         0, 1, 3, // 第一个三角形
                         1, 2, 3  // 第二个三角形
                       };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);

    // 2. 把我们的顶点数组复制到一个顶点缓冲中，提供给OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 2. 复制我们的索引数组到一个索引缓冲中，提供给OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //4. 解绑 VAO VBO EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::vao_vbo_ebo_glDrawElements()
{
    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    glDeleteBuffers(1, &EBO);

    qDebug() << "~LogoRenderer";
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


