#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(TCMatriz3D<bool> * _pm3D, QString _fileName, int _viewtype, QWidget *parent)
	: QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer), parent) { //QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
	viewtype=_viewtype;
	distpoints = 0.001;
	pointsize = 1.0;
	object = 0;
	xRot = 0;
	yRot = 0;
	zRot = 0;
	planX = 0;
	planY = 0;
	planZ = 0;
	fullFileName = _fileName;
	if( ! _pm3D )
		pm3D = new TCMatriz3D<bool>(fullFileName.toStdString());
	else
		pm3D = _pm3D;
	pm3Di = NULL;
	nx = pm3D->NX();
	ny = pm3D->NY();
	nz = pm3D->NZ();
	planX = nx/2;
	planY = ny/2;
	planZ = nz/2;
	pore = 1;
	tonsList << 0 << 1;
	trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
	setAutoFillBackground(false);
	setMinimumSize(200, 200);
	setWindowTitle(tr("3D Visualization [%1]").arg( QFileInfo(fullFileName).fileName() ) );
}

GLWidget::GLWidget(TCMatriz3D<int> * _pm3D, QString _fileName, int _viewtype, QWidget *parent)
	: QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer), parent) { //QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
	viewtype = _viewtype;
	distpoints = 0.001;
	pointsize = 1.0;
	object = 0;
	xRot = 0;
	yRot = 0;
	zRot = 0;
	fullFileName = _fileName;
	if( ! _pm3D )
		pm3Di = new TCMatriz3D<int>(fullFileName.toStdString());
	else
		pm3Di = _pm3D;
	pm3D = NULL;
	nx = pm3Di->NX();
	ny = pm3Di->NY();
	nz = pm3Di->NZ();
	planX = nx/2;
	planY = ny/2;
	planZ = nz/2;
	for (int k = 0; k < nz; k++)
		for (int j = 0; j < ny; j++)
			for (int i = 0; i < nx; i++)
				if ( ! tonsList.contains( pm3Di->data3D[i][j][k] ) )
					tonsList << pm3Di->data3D[i][j][k];
	pore = tonsList[0];
	foreach(int temp, tonsList) {
		QVariant var(temp);
		qstrTonsList << var.toString();
	}
	trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
	setAutoFillBackground(false);
	setMinimumSize(200, 200);
	setWindowTitle(tr("3D Visualization [%1]").arg( QFileInfo(fullFileName).fileName() ) );
}

GLWidget::~GLWidget() {
	//delete pm3D;
	makeCurrent();
	glDeleteLists(object, 1);
}

void GLWidget::setXRotation(int angle) {
	normalizeAngle(&angle);
	if (angle != xRot)
		xRot = angle;
}

void GLWidget::setYRotation(int angle) {
	normalizeAngle(&angle);
	if (angle != yRot)
		yRot = angle;
}

void GLWidget::setZRotation(int angle) {
	normalizeAngle(&angle);
	if (angle != zRot)
		zRot = angle;
}

void GLWidget::invertPore( ) {
	if (tonsList.size() == 2) {
		pore == tonsList[0] ? pore = tonsList[1] : pore = tonsList[0];
		object = makeObject();
		QGLWidget::update();
	}
}

void GLWidget::setPore(int _pore) {
	pore = _pore;
	object = makeObject();
	QGLWidget::update();
}

void GLWidget::setPlanX(int _plan){
	planX = _plan;
	object = makeObject();
	QGLWidget::update();
}

void GLWidget::setPlanY(int _plan){
	planY = _plan;
	object = makeObject();
	QGLWidget::update();
}

void GLWidget::setPlanZ(int _plan){
	planZ = _plan;
	object = makeObject();
	QGLWidget::update();
}

void GLWidget::setViewType(int _viewtype){
	viewtype=_viewtype;
	object = makeObject();
	QGLWidget::update();
}

int GLWidget::getPlanX( ){
	return planX;
}

int GLWidget::getPlanY( ){
	return planY;
}

int GLWidget::getPlanZ( ){
	return planZ;
}

int GLWidget::getPore() {
	return pore;
}

int GLWidget::getViewType(){
	return viewtype;
}

TCMatriz3D<bool> *GLWidget::getPM3D() {
	return pm3D;
}

TCMatriz3D<int> *GLWidget::getPM3Di() {
	return pm3Di;
}

void GLWidget::initializeGL() {
	object = makeObject();
	setXRotation(xRot - 360);
	setYRotation(yRot + 720);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();
	if (event->buttons() & Qt::LeftButton) {
		setXRotation(xRot - 8 * dy);
		setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();
	QGLWidget::update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
	if (event->delta() > 0 && nx*pointsize*2 < 1400) {
		pointsize  *= 1.1f;
		distpoints *= 1.1f;
	} else if ( nx*pointsize > 50) {
		pointsize  /= 1.1f;
		distpoints /= 1.1f;
	}
	object = makeObject();
	QGLWidget::update();
}

//void GLWidget::paintEvent(QPaintEvent *event) {
void GLWidget::paintGL() {
	//event = event; // evitar warning
	makeCurrent(); // Faz este widget ser o atual contexto de composição OpenGL.
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	qglClearColor(trolltechPurple.dark());

	//glEnable(GL_LIGHTING);  //Habilita o uso de iluminação
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);  // Habilita a luz de número 0
	GLfloat lightPosition[4] = { 1.0, 1.0, 1.0, 1.0}; // x, y, z, w
	GLfloat luzAmbiente[4]    = { 1.0, 1.0, 1.0, 1.0}; // r, g, b, a
	//GLfloat luzDifusa[4]	 = {1.0, 1.0, 1.0, 1.0}; // "cor"
	//GLfloat luzEspecular[4]  = {1.0, 1.0, 1.0, 1.0}; // "brilho"
	//GLfloat especularidade[4]= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);

	//material
	GLfloat especularidade[4] = { 0.5, 0.5, 0.5, 1.0 }; // Capacidade de brilho do material
	GLfloat shininess[1] = { 50.0 };
	glShadeModel (GL_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especularidade);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);	// Ativa o uso da luz ambiente
	/*
		glPushMatrix();
		glEnable(GL_CULL_FACE);
		glLightModeli(GL_AMBIENT_AND_DIFFUSE, GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB); // GL_AMBIENT
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_MULTISAMPLE);
		*/
	//glEnable(GL_POINT_SPRITE); //Calcular as coordenadas da textura de pontos com base na textura ambiente e definições de parâmetros ponto.
	//Caso contrário, coordenadas de textura são constantes entre pontos.
	//
	//reshape
	setupViewport(width(), height());

	glLoadIdentity();
	glTranslated(0.0, 0.0, -10.0);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	glCallList(object);

	//exibe instruções
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	drawInstructions(&painter);
	painter.end();
}

void GLWidget::resizeGL(int width, int height) {
	setupViewport(width, height);
}

QSize GLWidget::sizeHint() const {
	return QSize(1536, 1536);
}

GLuint GLWidget::makeObject() {
	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
		static GLfloat logoDiffuseColor[4] = {trolltechPurple.red()/255.0, trolltechPurple.green()/255.0, trolltechPurple.blue()/255.0, 1.0};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, logoDiffuseColor);
		*/
	double w = distpoints;
	int meionx = nx/2;
	int meiony = ny/2;
	int meionz = nz/2;
	double _meionxw = -w*meionx;
	double _meionyw = -w*meiony;
	double _meionzw = -w*meionz;
	double meionxw = w*(meionx-1);
	double meionyw = w*(meiony-1);
	double meionzw = w*(meionz-1);

	glPointSize(pointsize);
	glEnable(GL_POINT_SMOOTH);
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_NORMALIZE);

	//Desenhando linhas do cubo
	glColor3f(0.0, 1.0, 0.0); // cor verde
	glLineWidth (1.5);
	// desenhando quandrado superior
	glBegin(GL_LINE_LOOP); //Exibe uma sequencia de linhas conectando os pontos definidos por glVertex e ao final liga o primeiro com o último
	glVertex3d(_meionxw, meionyw, meionzw);
	glVertex3d( meionxw, meionyw, meionzw);
	glVertex3d( meionxw, meionyw,_meionzw);
	glVertex3d(_meionxw, meionyw,_meionzw);
	glEnd();
	// desenhando quandrado inferior
	glBegin(GL_LINE_LOOP);
	glVertex3d(_meionxw,_meionyw, meionzw);
	glVertex3d( meionxw,_meionyw, meionzw);
	glVertex3d( meionxw,_meionyw,_meionzw);
	glVertex3d(_meionxw,_meionyw,_meionzw);
	glEnd();
	// desenhando linhas ligando os cantos dos quadrados
	glBegin(GL_LINES); //linha 1
	glVertex3d(_meionxw, meionyw, meionzw);
	glVertex3d(_meionxw,_meionyw, meionzw);
	glEnd();
	glBegin(GL_LINES); //linha 2
	glVertex3d( meionxw, meionyw, meionzw);
	glVertex3d( meionxw,_meionyw, meionzw);
	glEnd();
	glBegin(GL_LINES); //linha 3
	glVertex3d(_meionxw, meionyw,_meionzw);
	glVertex3d(_meionxw,_meionyw,_meionzw);
	glEnd();
	glBegin(GL_LINES); //linha 4
	glVertex3d( meionxw, meionyw,_meionzw);
	glVertex3d( meionxw,_meionyw,_meionzw);
	glEnd();
	// desenhando linhas dos eixos
	// eixo x
	glColor3f(0.0, 1.0, 0.0); // cor verde para o eixo x
	glBegin(GL_LINES);
	glVertex3d(-512.0*w, 0.0, 0.0);
	glVertex3d(_meionxw, 0.0, 0.0);
	glVertex3d( 512.0*w, 0.0, 0.0);
	glVertex3d( meionxw, 0.0, 0.0);
	glEnd();
	// eixo y
	glColor3f(0.0, 0.0, 1.0); // cor azul para o eixo y
	glBegin(GL_LINES);
	glVertex3d( 0.0,-512.0*w, 0.0);
	glVertex3d( 0.0,_meionyw, 0.0);
	glVertex3d( 0.0, 512.0*w, 0.0);
	glVertex3d( 0.0, meionyw, 0.0);
	glEnd();
	// eixo z
	glColor3f(1.0, 0.0, 0.0); // cor vermelha para o eixo z
	glBegin(GL_LINES);
	glVertex3d( 0.0, 0.0,-512.0*w);
	glVertex3d( 0.0, 0.0,_meionzw);
	glVertex3d( 0.0, 0.0, 512.0*w);
	glVertex3d( 0.0, 0.0, meionzw);
	glEnd();

	if(pm3D != NULL)
		goto drawByPm3D;
	else if(pm3Di != NULL)
		goto drawByPm3Di;

drawByPm3D:
	//Desenhando o meio poroso
	glBegin(GL_POINTS); //GL_POINTS
	if (viewtype==VIEW3D){
		for (int k = 0; k < nz; k++){
			for (int j = 0; j < ny; j++){
				for (int i = 0; i < nx; i++){
					if( pm3D->data3D[i][j][k] == pore ){
						//glNormal3d(0.0, 0.0, -1.0);
						glColor3f(0.0, 0.0, 0.0);
						glVertex3d( w*(i-meionx) , w*(j-meiony) , w*(k-meionz) );
					}
				}
			}
		}
	} else { //viewtype==MPV
		for (int j = 0; j < ny; j++){
			for (int i = 0; i < nx; i++){
				if (pm3D->data3D[planX][i][j] == pore){
					glColor3f(0.0, 0.0, 0.0);
				} else {
					glColor3f(1.0, 1.0, 1.0);
				}
				glVertex3d( w*(planX-meionx) , w*(i-meiony) , w*(j-meionz) );
				if (pm3D->data3D[i][planY][j] == pore){
					glColor3f(0.0, 0.0, 0.0);
				} else {
					glColor3f(1.0, 1.0, 1.0);
				}
				glVertex3d( w*(i-meionx) , w*(planY-meiony) , w*(j-meionz) );
				if (pm3D->data3D[i][j][planZ] == pore){
					glColor3f(0.0, 0.0, 0.0);
				} else {
					glColor3f(1.0, 1.0, 1.0);
				}
				glVertex3d( w*(i-meionx) , w*(j-meiony) , w*(planZ-meionz) );
			}
		}
	}
	glEnd();
	glEndList();
	return list;

drawByPm3Di:
	//Desenhando o meio poroso
	glBegin(GL_POINTS); //GL_POINTS
	if (viewtype==VIEW3D){
		for (int k = 0; k < nz; k++){
			for (int j = 0; j < ny; j++){
				for (int i = 0; i < nx; i++){
					if( pm3Di->data3D[i][j][k] == pore ){
						//glNormal3d(0.0, 0.0, -1.0);
						glColor3f(0.0, 0.0, 0.0);
						glVertex3d( w*(i-meionx) , w*(j-meiony) , w*(k-meionz) );
					}
				}
			}
		}
	} else { //viewtype==MPV
		for (int j = 0; j < ny; j++){
			for (int i = 0; i < nx; i++){
				if (pm3Di->data3D[planX][i][j] == pore){
					glColor3f(0.0, 0.0, 0.0);
				} else {
					glColor3f(1.0, 1.0, 1.0);
				}
				glVertex3d( w*(planX-meionx) , w*(i-meiony) , w*(j-meionz) );
				if (pm3Di->data3D[i][planY][j] == pore){
					glColor3f(0.0, 0.0, 0.0);
				} else {
					glColor3f(1.0, 1.0, 1.0);
				}
				glVertex3d( w*(i-meionx) , w*(planY-meiony) , w*(j-meionz) );
				if (pm3Di->data3D[i][j][planZ] == pore){
					glColor3f(0.0, 0.0, 0.0);
				} else {
					glColor3f(1.0, 1.0, 1.0);
				}
				glVertex3d( w*(i-meionx) , w*(j-meiony) , w*(planZ-meionz) );
			}
		}
	}
	glEnd();
	glEndList();
	return list;
}

void GLWidget::normalizeAngle(int *angle) {
	while (*angle < 0)
		*angle += 360 * 16;
	while (*angle > 360 * 16)
		*angle -= 360 * 16;
}

void GLWidget::setupViewport(int width, int height) {
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWidget::drawInstructions(QPainter *painter) {
	QString text = tr("Click and drag with the left or right mouse button to rotate the 3D Image.");
	QFontMetrics metrics = QFontMetrics(font());
	int border = qMax(4, metrics.leading());
	QRect rect = metrics.boundingRect(0, 0, width()-2*border, int(height()*0.125), Qt::AlignCenter | Qt::TextWordWrap, text);
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->fillRect(QRect(0, 0, width(), rect.height()+2*border), QColor(0, 0, 0, 127));
	painter->setPen(Qt::white);
	painter->fillRect(QRect(0, 0, width(), rect.height()+2*border), QColor(0, 0, 0, 127));
	painter->drawText((width()-rect.width())/2, border, rect.width(), rect.height(), Qt::AlignCenter | Qt::TextWordWrap, text);
}
