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
#include <Segmentacao/CObjetoImagem.h>
#include "GL/glu.h"
#include "math.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE

template <typename T> class TCMatriz3D;

/*! GLWidget é a classe que representa a visualização de imagens tridimensionais em OpenGL.*/
class GLWidget : public QGLWidget
{
		Q_OBJECT

	public:
		/// Enumera os tipos de visualizações 3D disponíveis
		enum {
			VIEW3D,	///< 0 - Visualizar em 3D
			MPV,    ///< 1 - Multiplanar Viewing
			RPSL		///< 2 - Rede de Percolação de Sítios e Ligações
		};

		//GLWidget(QString _fileName, QWidget *parent = 0);
		GLWidget(TCMatriz3D<int> *_pm3D, QString _fileName, int _viewtype=MPV, QWidget *parent = 0);
		GLWidget(TCMatriz3D<bool> *_pm3D, QString _fileName, int _viewtype=MPV, QWidget *parent = 0);
		GLWidget(QString _fileName, int _viewtype=RPSL, QWidget *parent = 0);
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
		TCMatriz3D<bool> *getPM3D();
		TCMatriz3D<int> *getPM3Di();

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

		float calcAngle(float _x1, float _y1, float _x2, float _y2);

		GLuint object;
		int xRot;
		int yRot;
		int zRot;
		int nx;
		int ny;
		int nz;
		QPoint lastPos;
		QString fullFileName;
		QColor trolltechPurple;
		//GLfloat angle, fAspect;
		TCMatriz3D<int> * pm3Di;
		TCMatriz3D<bool> * pm3D;
		map<int,CObjetoImagem> matrizObjetos; ///< Matriz de objetos do tipo CObjetoImagem

		int pore;
		int planX;
		int planY;
		int planZ;
		int viewtype;
		float pointsize; //tamanho dos pontos;
		float distpoints; //distancia entre pontos;
		float grayTon; //divisor da escala de tons de cinza (1/numCores)

		GLUquadricObj* quadratic;
};

#endif
