#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(CMatriz3D * _pm3D, QString _fileName, int _viewtype, QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    viewtype=_viewtype;
    object = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    planX = 0;
    planY = 0;
    planZ = 0;
    pm3D = _pm3D;
    if (pm3D) {
        if( pm3D->GetFormat() == 13 ) {
            pore = 1;
            tonsList << 0 << 1;
        } else {
            int nx = pm3D->NX();
            int ny = pm3D->NY();
            int nz = pm3D->NZ();
            for (int k = 0; k < nz; k++)
                for (int j = 0; j < ny; j++)
                    for (int i = 0; i < nx; i++)
                        if ( ! tonsList.contains( pm3D->data3D[i][j][k] ) )
                            tonsList << pm3D->data3D[i][j][k];
            pore = tonsList[0];
            foreach(int temp, tonsList) {
                QVariant var(temp);
                qstrTonsList << var.toString();
            }
        }
        planX = pm3D->NX()/2;
        planY = pm3D->NY()/2;
        planZ = pm3D->NZ()/2;
    }
    trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    fullFileName = _fileName;
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

CMatriz3D * GLWidget::getPM3D() {
    return pm3D;
}

void GLWidget::initializeGL() {
    object = makeObject();
    setXRotation(xRot - 380);
    setYRotation(yRot - 740);
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

void GLWidget::paintEvent(QPaintEvent *event) {
    event = event; // evitar warning
    makeCurrent(); 							// Faz este widget ser o atual contexto de composição OpenGL.
    glMatrixMode(GL_PROJECTION); 				// GL_MODELVIEW // GL_COLOR // GL_PROJECTION
    glPushMatrix();
    qglClearColor(trolltechPurple.dark());
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_POINT_SMOOTH); //desenhar pontos com a filtragem adequada.
    //glEnable(GL_POINT_SPRITE); //Calcular as coordenadas da textura de pontos com base na textura ambiente e definições de parâmetros ponto.
    //Caso contrário, coordenadas de textura são constantes entre pontos.
    /*
    GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    GLfloat luzAmbiente[4]   = {0.2,  0.2,  0.2, 1.0};
    GLfloat luzDifusa[4]	    = {0.7,  0.7,  0.7, 1.0};	   	// "cor"
    GLfloat luzEspecular[4]  = {1.0,  1.0,  1.0, 1.0};		// "brilho"
    GLfloat especularidade[4]= {1.0,  1.0,  1.0, 1.0}; 		// Capacidade de brilho do material
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    */
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);	// Ativa o uso da luz ambiente

    setupViewport(width(), height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    glCallList(object);
    /*
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
*/
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawInstructions(&painter);
    painter.end();
}

void GLWidget::resizeGL(int width, int height) {
    setupViewport(width, height);
}

QSize GLWidget::sizeHint() const {
    return QSize(800, 800);
}

GLuint GLWidget::makeObject() {
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    //glEnable(GL_NORMALIZE);
    glPointSize(1.0);
    glBegin(GL_POINTS); //GL_POINTS
    /*
    static GLfloat logoDiffuseColor[4] = {trolltechPurple.red()/255.0, trolltechPurple.green()/255.0, trolltechPurple.blue()/255.0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, logoDiffuseColor);
    */
    if( pm3D == NULL )
        pm3D = new CMatriz3D(fullFileName.toStdString());
    double w = 0.001;
    int nx = pm3D->NX();
    int ny = pm3D->NY();
    int nz = pm3D->NZ();

    if (viewtype==VIEW3D){
        for (int k = 0; k < nz; k++)
            for (int j = 0; j < ny; j++)
                for (int i = 0; i < nx; i++){
                    if( ( (k==0 || k==nz-1) &&  (i==0 || j==0 || i==nx-1 || j==ny-1) ) ||
                            ( (j==0 || j==ny-1) &&  (i==0 || k==0 || i==nx-1 || k==nz-1) ) ){
                        //glNormal3d(0.0, 0.0, -1.0);
                        glColor3f(0.0, 1.0, 0.0);
                        glVertex3d( ((i-(nx/2))*w) , ((j-(ny/2))*w) , ((k-(nz/2))*w) );
                    } else if( pm3D->data3D[i][j][k] == pore /*&& (k==0 || i==0 || j==0 || k==nz-1 || i==nx-1 || j==ny-1)*/ ){
                       //glNormal3d(0.0, 0.0, -1.0);
                       glColor3f(0.0, 0.0, 0.0);
                       glVertex3d( ((i-(nx/2))*w) , ((j-(ny/2))*w) , ((k-(nz/2))*w) );
                    }
                }
    } else {
        for (int k = 0; k < nz; k++)
            for (int j = 0; j < ny; j++)
                for (int i = 0; i < nx; i++){
                    if( ( (k==0 || k==nz-1) &&  (i==0 || j==0 || i==nx-1 || j==ny-1) ) ||
                            ( (j==0 || j==ny-1) &&  (i==0 || k==0 || i==nx-1 || k==nz-1) ) ){
                        //glNormal3d(0.0, 0.0, -1.0);
                        glColor3f(0.0, 1.0, 0.0);
                        glVertex3d( ((i-(nx/2))*w) , ((j-(ny/2))*w) , ((k-(nz/2))*w) );
                    }else if( (i==planX || j==planY || k==planZ) && pm3D->data3D[i][j][k] == pore){
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex3d( ((i-(nx/2))*w) , ((j-(ny/2))*w) , ((k-(nz/2))*w) );
                    }else if( i==planX || j==planY || k==planZ ){
                        glColor3f(1.0, 1.0, 1.0);
                        glVertex3d( ((i-(nx/2))*w) , ((j-(ny/2))*w) , ((k-(nz/2))*w) );
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
}

void GLWidget::drawInstructions(QPainter *painter) {
    QString text = tr("Click and drag with the left or right mouse button to rotate the 3D Image.");
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
                                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                      QColor(0, 0, 0, 127));
    painter->setPen(Qt::white);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                      QColor(0, 0, 0, 127));
    painter->drawText((width() - rect.width())/2, border,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, text);
}
