#ifndef _Q_FLUXUS_WIDGET_H_
#define _Q_FLUXUS_WIDGET_H_

#include <QGLWidget>

class QFluxusWidget : public QGLWidget 
{
    Q_OBJECT

public:
    QFluxusWidget(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent * ev);
    void keyReleaseEvent(QKeyEvent * ev);

    void mouseMoveEvent(QMouseEvent * ev);
    void mousePressEvent(QMouseEvent * ev);
    void mouseReleaseEvent(QMouseEvent * ev);
};


#endif
