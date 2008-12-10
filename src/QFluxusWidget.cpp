#include <iostream>
using namespace std;

#include "QFluxusWidget.h"

void ReshapeCallback(int width, int height);
void DisplayCallback();

QFluxusWidget::QFluxusWidget(QWidget *parent)
: QGLWidget(parent)
{
        // receive everything
        setFocusPolicy(Qt::WheelFocus); 
}

void QFluxusWidget::resizeGL(int w, int h)
{
        // call through to the callback used with glut
        ReshapeCallback(w,h);
}

void QFluxusWidget::paintGL()
{
        DisplayCallback();
}

QSize QFluxusWidget::minimumSizeHint() const
{
        return QSize(64,48);
}

QSize QFluxusWidget::sizeHint() const
{
        return QSize(720, 576);
}

