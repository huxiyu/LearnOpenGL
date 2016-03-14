#ifndef OPENGLCONTEXTTOOL
#define OPENGLCONTEXTTOOL

#endif // OPENGLCONTEXTTOOL

#include <QWindow>
#include <QQuickView>

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QDebug>

struct Version {
    const char *str;
    int major;
    int minor;
};

static struct Version versions[] = {
    { "1.0", 1, 0 },
    { "1.1", 1, 1 },
    { "1.2", 1, 2 },
    { "1.3", 1, 3 },
    { "1.4", 1, 4 },
    { "1.5", 1, 5 },
    { "2.0", 2, 0 },
    { "2.1", 2, 1 },
    { "3.0", 3, 0 },
    { "3.1", 3, 1 },
    { "3.2", 3, 2 },
    { "3.3", 3, 3 },
    { "4.0", 4, 0 },
    { "4.1", 4, 1 },
    { "4.2", 4, 2 },
    { "4.3", 4, 3 },
    { "4.4", 4, 4 },
    { "4.5", 4, 5 }
};

struct Profile {
    const char *str;
    QSurfaceFormat::OpenGLContextProfile profile;
};
static struct Profile profiles[] = {
    { "none", QSurfaceFormat::NoProfile },
    { "core", QSurfaceFormat::CoreProfile },
    { "compatibility", QSurfaceFormat::CompatibilityProfile }
};

struct Option {
    const char *str;
    QSurfaceFormat::FormatOption option;
};

static struct Option options[] = {
    { "deprecated functions (not forward compatible)", QSurfaceFormat::DeprecatedFunctions },
    { "debug context", QSurfaceFormat::DebugContext },
    { "stereo buffers", QSurfaceFormat::StereoBuffers },
    // This is not a QSurfaceFormat option but is helpful to determine if the driver
    // allows compiling old-style shaders with core profile.
    { "force version 110 shaders", QSurfaceFormat::FormatOption(0) }
};

struct Renderable {
    const char *str;
    QSurfaceFormat::RenderableType renderable;
};
static struct Renderable renderables[] = {
    { "default", QSurfaceFormat::DefaultRenderableType },
    { "OpenGL", QSurfaceFormat::OpenGL },
    { "OpenGL ES", QSurfaceFormat::OpenGLES }
};

void setContext(QQuickView& window);

void printFormat(const QSurfaceFormat &format);

void printContext();
