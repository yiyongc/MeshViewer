#include "GLViewer.h"
#include <qtimer.h>
#include <QMouseEvent>
#define _USE_MATH_DEFINES
#include <math.h>

GLViewer::GLViewer(QWidget* parent) : xRot(0), yRot(0), zRot(0), scaleSize(1.0), xTrans(0), yTrans(0), mouseMovementAngle(0){

	//Updates GL Widget every 10ms
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);

}

GLViewer::~GLViewer() {}

void GLViewer::initializeGL() {
	glClearColor(0.8, 0.8, 0.8, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//shading
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);

	static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

}

void GLViewer::paintGL() {
	
	// Just clean the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 3, 6, 0, 0, 0, 0, 1, 0);

	// rotate and scale the object
	glRotatef(xRot, 1, 0, 0);
	glRotatef(yRot, 0, 1, 0);
	glRotatef(zRot, 0, 0, 1);
	glScalef(scaleSize, scaleSize, scaleSize);
	glTranslatef(xTrans, yTrans, 0);

	drawGrid();
	drawAxis();
	//drawPyramid();
}

void GLViewer::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);
	float ratio = (float)width / (float)height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, ratio, .1, 100);
}

void GLViewer::qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360)
		angle -= 360 * 16;
}

void GLViewer::mousePressEvent(QMouseEvent *event) { 
	lastPos = event->pos();
	rotateZState = 0;
	//if (event->button() == Qt::LeftButton) {
	//	setXRotation(xRot+30);
	//}
	//if (event->button() == Qt::RightButton) {
	//	setYRotation(yRot + 30);
	//}
	//if (event->button() == Qt::MiddleButton) {
	//	setZRotation(zRot + 30);
	//}
}

void GLViewer::mouseReleaseEvent(QMouseEvent *event) {
}
void GLViewer::mouseMoveEvent(QMouseEvent *event) {
	dx = event->x() - lastPos.x();
	dy = event->y() - lastPos.y();
	
	//Some pseudo code
	//Using timer, take e.g. 3 mouse positions, pos1, pos2, pos3
	//Use vector v1 (pos1) dot product vector v2 (pos2) to get angle between vectors
	//repeat for vector v2 dot v3
	//Using these angles given a threshold, we can determine if mouse is moving straight or in an arc
	//Change rotation state and allow rotations

	//Rotation
	if (event->buttons() & Qt::LeftButton) {
		

		QTimer* angleTimer = new QTimer(this);
		connect(angleTimer, SIGNAL(timeout()), this, SLOT(calculateAngle()));
		angleTimer->start(1000);
		
		

		if (rotateZState )
			setZRotation(zRot + 4 * dx);
		else {
			//setXRotation(xRot + 8 * dy);
			//setYRotation(yRot + 8 * dx);
		}
	}
	//Translation
	else if (event->buttons() & Qt::RightButton) {
		/*setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);*/
		xTrans -= (float)dx / 5;
		yTrans += (float)dy / 5;
	}
	//Zooming
	else if (event->buttons() & Qt::MiddleButton) {
		scaleSize -=   (float) dy/10;
	}
	lastPos = event->pos();

}

void GLViewer::calculateAngle() {
	int x = abs(dx);
	int y = abs(dy);
	
	if (x > 3 && y > 3) {
		mouseMovementAngle = atan2(y, x);
		mouseMovementAngle = mouseMovementAngle / (2 * M_PI) * 360;
		rotateZState = 1;
	}
	else
		rotateZState = 0;
	//if (mouseMovementAngle > 45)
	//	rotateZState = 1;
	//else
	//	rotateZState = 0;
}

void GLViewer::keyPressEvent(QKeyEvent *event) {
}

QSize GLViewer::minimumSizeHint() const
{
	return QSize(100, 100);
}

QSize GLViewer::sizeHint() const
{
	return QSize(800, 800);
}

void GLViewer::setXRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != xRot) {
		xRot = angle;
		emit xRotationChanged(angle);
		updateGL();
	}
}

void GLViewer::setYRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != yRot) {
		yRot = angle;
		emit yRotationChanged(angle);
		updateGL();
	}
}

void GLViewer::setZRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != zRot) {
		zRot = angle;
		emit zRotationChanged(angle);
		updateGL();
	}
}

void GLViewer::drawGrid() {
	qglColor(Qt::black);
	
	glBegin(GL_LINES);
	for (int i = -3; i <= 3; i++) {
		glVertex3f((float) i, 0, -3);
		glVertex3f((float)i, 0, 3);
		glVertex3f(-3, 0, (float) i);
		glVertex3f(3, 0, (float) i);
	}
	glEnd();


}


void GLViewer::drawZAxis() {
	GLUquadric* gluQuad = gluNewQuadric();
	gluCylinder(gluQuad, 0.01, 0.01, 2, 32, 32);
	glPushMatrix();
	glTranslated(0, 0, 2);
	glutSolidCone(0.02, 0.3, 32, 32);
	glPopMatrix();
}

void GLViewer::drawAxis() {
	//X-axis
	qglColor(Qt::darkRed);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawZAxis();
	glPopMatrix();

	//Y-axis
	qglColor(Qt::darkGreen);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	drawZAxis();
	glPopMatrix();

	//Z-axis
	qglColor(Qt::darkBlue);
	drawZAxis();
}

void GLViewer::drawPyramid() {
		qglColor(Qt::red);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, -1);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
	
		glEnd();
		glBegin(GL_TRIANGLES);
		glNormal3f(0, -1, 0.707);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(0, 0, 1.2);
		glEnd();
		glBegin(GL_TRIANGLES);
		glNormal3f(1, 0, 0.707);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(0, 0, 1.2);
		glEnd();
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 1, 0.707);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(0, 0, 1.2);
		glEnd();
		glBegin(GL_TRIANGLES);
		glNormal3f(-1, 0, 0.707);
		glVertex3f(-1, 1, 0);
		glVertex3f(-1, -1, 0);
		glVertex3f(0, 0, 1.2);
		glEnd();
}