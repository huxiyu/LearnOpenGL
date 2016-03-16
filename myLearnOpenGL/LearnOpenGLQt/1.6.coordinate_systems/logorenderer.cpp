/************************************************************************/
/*																										    */
/************************************************************************/

#include "logorenderer.h"
#include <QtGui/qmatrix4x4.h>
#include <QImage>
#include <QDateTime>

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::initialize()
{
    printContext(QOpenGLContext::currentContext());

    initializeOpenGLFunctions();

    textureTest_Init();

    m_fAngle = 0;
    m_fScale = 1;
    m_fps = 0.0f;
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::resize(int width, int height)
{
    glViewport(0, 0, width, height);
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
        //     lastTime = currentTime;
        if (m_fps > 50)
        {
            //            qDebug() << "FPS::" << m_fps;
        }
    }

    textureTest();

    m_fAngle += 1.1f;
    glFlush();
}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::textureTest_Init()
{
    bool exception = createShader(m_nProgram, ":/shader/shader.vert", ":/shader/shader.frag");
    if (exception)
    {
        qDebug() << "ERROR::CREATE::SHADER";
    }

    // Set up vertex data (and buffer(s)) and attribute pointers
    // Qt OpenGL 坐标系 Y轴取反
    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
        0.5f,  -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };

    GLuint indices[] = { // Note that we start from 0!
                         0, 1, 3, // First Triangle
                         1, 2, 3  // Second Triangle
                       };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);

    // 2. 把我们的顶点数组复制到一个VBO中，提供给OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 2. 复制我们的索引数组到一个索引缓冲中，提供给OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 3. 设置颜色属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3.设置纹理坐标指针
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //4. 解绑 VBO VAO EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 导入和生成纹理
    // ====================
    // Texture 1
    // ====================
    glGenTextures(1, &texture1);
    // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 设置纹理参数
    /// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load, create texture and generate mipmaps
    QImage container = QImage(FileSystem::getPath("resources/textures/container.jpg"));
    container = container.convertToFormat(QImage::Format_RGB888);
    Q_ASSERT(!container.isNull());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, container.width(), container.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, container.constBits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    // ===================
    // Texture 2
    // ===================
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    QImage smile = QImage(FileSystem::getPath("resources/textures/awesomeface.png"));
    smile = smile.convertToFormat(QImage::Format_RGB888);
    Q_ASSERT(!smile.isNull());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, smile.width(), smile.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, smile.constBits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

}

/************************************************************************/
/*																										    */
/************************************************************************/

void LogoRenderer::textureTest()
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind Textures using texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(m_nProgram, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(m_nProgram, "ourTexture2"), 1);

    // Activate shader
    glUseProgram(m_nProgram);

    // transform
    QMatrix4x4 transform;
    //    transform.perspective(45.0f, );
    //    transform.translate( 0.5f, 0.5f, 0.0f);
        transform.rotate(-m_fAngle, 0.0f, 0.0f, 1.0f);

    // Get matrix's uniform location and set matrix
    GLint transformLoc = glGetUniformLocation(m_nProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform.data());

    // Draw container
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






