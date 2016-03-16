/************************************************************************/
/*																										    */
/************************************************************************/

#ifndef OPENGLCONTEXTTOOL
#define OPENGLCONTEXTTOOL

/************************************************************************/
/*																										    */
/************************************************************************/

#endif // OPENGLCONTEXTTOOL

/************************************************************************/
/*																										    */
/************************************************************************/

#include <QList>
#include "openglcontexttool.h"

/************************************************************************/
/*																										    */
/************************************************************************/

void setContext(QQuickView& window){
    QSurfaceFormat fmt;
    fmt.setVersion(versions[11].major, versions[11].minor); // idx = 11
    fmt.setProfile(profiles[1].profile);  // i = 1
    //            fmt.setOption(options[i].option); // i =
    //            forceGLSL110 = true; // opengl2.0 glsl110
    fmt.setRenderableType(renderables[1].renderable); // i = 1

    // set depth buffer size.
//    fmt.setDepthBufferSize(16);
//    fmt.setStencilBufferSize(8);
//    fmt.setSamples(0);
//    fmt.setRedBufferSize(8);
//    fmt.setBlueBufferSize(8);
//    fmt.setGreenBufferSize(8);
//    fmt.setAlphaBufferSize(8);
//    fmt.setSwapInterval(1);

    // QWindow：
    window.setSurfaceType(QWindow::OpenGLSurface);
    window.setFormat(fmt);
//    window.openglContext()->setFormat(fmt);
}

/************************************************************************/
/*																										    */
/************************************************************************/

void printFormat(const QSurfaceFormat &format)
{
    qDebug() << "OpenGL version: " << format.majorVersion() << "." << format.minorVersion();

    for (size_t i = 0; i < sizeof(profiles) / sizeof(Profile); ++i)
        if (profiles[i].profile == format.profile()) {
            qDebug() << "Profile: " << QString::fromLatin1(profiles[i].str);
            break;
        }

    QString opts;
    for (size_t i = 0; i < sizeof(options) / sizeof(Option); ++i)
        if (format.testOption(options[i].option))
            opts += QString::fromLatin1(options[i].str) + QStringLiteral(" ");
    qDebug() << "Options: " << opts;

    for (size_t i = 0; i < sizeof(renderables) / sizeof(Renderable); ++i)
        if (renderables[i].renderable == format.renderableType()) {
            qDebug() << "Renderable type: " << QString::fromLatin1(renderables[i].str);
            break;
        }

    qDebug() << "Depth buffer size: " << QString::number(format.depthBufferSize());
    qDebug() << "Stencil buffer size: " << QString::number(format.stencilBufferSize());
    qDebug() << "Samples: " << QString::number(format.samples());
    qDebug() << "Red buffer size: " << QString::number(format.redBufferSize());
    qDebug() << "Green buffer size: " << QString::number(format.greenBufferSize());
    qDebug() << "Blue buffer size: " << QString::number(format.blueBufferSize());
    qDebug() << "Alpha buffer size: " << QString::number(format.alphaBufferSize());
    qDebug() << "Swap interval: " << QString::number(format.swapInterval());
}

/************************************************************************/
/*																										    */
/************************************************************************/

void printContext(QOpenGLContext *context)
{
//    QOpenGLContext *context = QOpenGLContext::currentContext();
    Q_ASSERT(context);

    QString vendor, renderer, version, glslVersion;
    const GLubyte *p;
    QOpenGLFunctions *f = context->functions();
    if ((p = f->glGetString(GL_VENDOR)))
        vendor = QString::fromLatin1(reinterpret_cast<const char *>(p));
    if ((p = f->glGetString(GL_RENDERER)))
        renderer = QString::fromLatin1(reinterpret_cast<const char *>(p));
    if ((p = f->glGetString(GL_VERSION)))
        version = QString::fromLatin1(reinterpret_cast<const char *>(p));
    if ((p = f->glGetString(GL_SHADING_LANGUAGE_VERSION)))
        glslVersion = QString::fromLatin1(reinterpret_cast<const char *>(p));

    qDebug() << "*** Context information ***";
    qDebug() << "Vendor: " << vendor;
    qDebug() << "Renderer: " << renderer;
    qDebug() << "OpenGL version: " << version;
    qDebug() << "GLSL version: " << glslVersion;

    qDebug() << "\n*** QSurfaceFormat from context ***";
    printFormat(context->format());

    //    qDebug() << (tr("\n*** QSurfaceFormat from window surface ***"));
    //    printFormat(m_surface->format());

    qDebug(("\n*** Qt build information ***"));
    const char *gltype[] = { "Desktop", "GLES 2", "GLES 1" };
    qDebug() << QString("Qt OpenGL configuration: ")
             << QString::fromLatin1(gltype[QOpenGLContext::openGLModuleType()])
            ;
    qDebug() << "Qt OpenGL library handle: "
             << QString::number(qintptr(QOpenGLContext::openGLModuleHandle()), 16)
                ;

    //    QList<QByteArray> extensionList = context->extensions().toList();
    //    std::sort(extensionList.begin(), extensionList.end());
    //    qDebug() << "\nFound " << extensionList.count() << " extensions:";
    //    Q_FOREACH (const QByteArray &ext, extensionList)
    //        qDebug() << QString::fromLatin1(ext);

}

/************************************************************************/
/*																										    */
/************************************************************************/
