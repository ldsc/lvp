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
#include <Matriz/TCMatriz3D.h>

QT_BEGIN_NAMESPACE;
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE;

template <typename T> class TCMatriz3D;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    enum {
        VIEW3D,	// 0 - Visualizar em 3D
        MPV       // 1 - Multiplanar Viewing
    };

    //GLWidget(QString _fileName, QWidget *parent = 0);
		GLWidget(TCMatriz3D<int> *_pm3D, QString _fileName, int _viewtype=MPV, QWidget *parent = 0);
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
    void setPlanX(int _plan);
    void setPlanY(int _plan);
    void setPlanZ(int _plan);
    void setPore(int _pore);
    void setViewType(int _viewtype);
    void invertPore( );
    int getPlanX( );
    int getPlanY( );
    int getPlanZ( );
    int getPore( );
    int getViewType( );
		TCMatriz3D<int> * getPM3D();

protected:
    void initializeGL();                        //reimplementada de QGLWidget
    //void paintEvent(QPaintEvent *event);      //reimplementada de QGLWidget
    void paintGL();                             //reimplementada de QGLWidget
    void resizeGL(int width, int height);       //reimplementada de QGLWidget
    void mousePressEvent(QMouseEvent *event);   //reimplementada de QWidget
    void mouseMoveEvent(QMouseEvent *event);    //reimplementada de QWidget
    void wheelEvent(QWheelEvent *event);        //reimplementada de QWidget

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
		TCMatriz3D<int> * pm3D;
    int pore;
    int planX;
    int planY;
    int planZ;
    int viewtype;
    float pointsize; //tamanho dos pontos;
    float distpoints; //distancia entre pontos;
};

#endif
