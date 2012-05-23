#ifndef GLWIDGET_H
#define GLWIDGET_H

//#include <QBrush>
//#include <QFont>
//#include <QImage>
#include <QtOpenGL>
#include <QGLWidget>
#include <QList>
#include <QStringList>
#include <QVariant>
#include <Matriz/CMatriz3D.h>

QT_BEGIN_NAMESPACE;
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE;
class CMatriz3D;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    //GLWidget(QString _fileName, QWidget *parent = 0);
    GLWidget(CMatriz3D *_pm3D, QString _fileName, QWidget *parent = 0);
    ~GLWidget();

    QSize sizeHint() const;
    int xRotation() const { return xRot; }
    int yRotation() const { return yRot; }
    int zRotation() const { return zRot; }
    QString getFullFileName() const { return fullFileName; }
    QList<int> tonsList;
    QStringList qstrTonsList;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void invertPore( );
    void setPore(int _pore);
    int  getPore( );

protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    GLuint makeObject();
    void drawInstructions(QPainter *painter);

    void normalizeAngle(int *angle);
    void setupViewport(int width, int height);

    GLuint object;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QString fullFileName;
    QColor trolltechPurple;
    //GLfloat angle, fAspect;
    CMatriz3D * pm3D;
    int pore;
};

#endif
