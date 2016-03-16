/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef THREADRENDERER_H
#define THREADRENDERER_H

#include <QQuickItem>

class RenderThread;

class ThreadRenderer : public QQuickItem
{
    Q_OBJECT

public:
    Q_PROPERTY(int viewPortWidth READ getViewPortWidth WRITE setViewPortWidth NOTIFY viewPortWidthChanged)
    Q_PROPERTY(int viewPortHeight READ getViewPortHeight WRITE setViewPortHeight NOTIFY viewPortHeightChanged)

public:
    // start Q_INVOKABLE function
    Q_INVOKABLE QString qmlTest(int num);
    Q_INVOKABLE float getFPS(void);
    // end Q_INVOKABLE function


public:
    ThreadRenderer();

    static QList<QThread *> threads;

public Q_SLOTS:
    void ready();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    RenderThread *m_renderThread;

 //
public:
    // view port width
    int m_viewPortWidth;
    // view port height
    int m_viewPortHeight;
    // get view port width
    int getViewPortWidth() const {return m_viewPortWidth;}
    // set view port width
    void setViewPortWidth( const int &viewPortWidth);
    // get view port height
    int getViewPortHeight() const {return m_viewPortHeight;}
    // set view port height
    void setViewPortHeight( const int &viewPortHeight);
signals:
    // notify when view change
    void viewPortWidthChanged();
    // notify when view change
    void viewPortHeightChanged();

};

#endif
