#include "GLViewer.h"
#include <qtimer.h>
#include <QMouseEvent>
#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <thread>
#include <qvector2d.h>

GLViewer::GLViewer(QWidget* parent) : xRot(0), yRot(0), zRot(0), scaleSize(1.0), xTrans(0), yTrans(0) {

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
	
	if (event->buttons() & Qt::LeftButton) {

		float mouseMovementAngle = 0;

		QVector2D startPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

		std::this_thread::sleep_for(std::chrono::milliseconds(120));
		QVector2D firstPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

		std::this_thread::sleep_for(std::chrono::milliseconds(120));
		QVector2D secondPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

		std::this_thread::sleep_for(std::chrono::milliseconds(120));
		QVector2D thirdPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

		QVector2D vector1 = firstPoint - startPoint;
		QVector2D vector2 = secondPoint - startPoint;
		QVector2D vector3 = thirdPoint - startPoint;


		float lengthVector1 = vector1.length();
		float lengthVector2 = vector2.length();
		float lengthVector3 = vector3.length();

		float angle1 = acos(QVector2D::dotProduct(vector1, vector2) / lengthVector1 / lengthVector2);
		float angle2 = acos(QVector2D::dotProduct(vector3, vector2) / lengthVector3 / lengthVector2);

		mouseMovementAngle = angle1 + angle2;

		if (mouseMovementAngle > 0.10) {
			rotateZState = 1;

			//determine direction of mouse movement
			float directionCheck = vector1.x() * vector3.y() - vector1.y() * vector3.x();

			if (directionCheck < 0)
				clockWise = false;
			else
				clockWise = true;
		}
		else
			rotateZState = 0;
	}
}

void GLViewer::mouseReleaseEvent(QMouseEvent *event) {
	rotateZState = -1;
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
	if (event->buttons() & Qt::LeftButton && rotateZState != -1) {
		if (rotateZState == 1) {
			if (clockWise)
				setZRotation(zRot - 4);
			else
				setZRotation(zRot + 4);
		}
		else {
			setXRotation(xRot + 4 * dy);
			setYRotation(yRot + 4 * dx);
		}
	}
	//Translation
	else if (event->buttons() & Qt::RightButton) {
		xTrans -= (float)dx / 5;
		yTrans += (float)dy / 5;
	}
	//Zooming
	else if (event->buttons() & Qt::MiddleButton) {
		scaleSize -=   (float) dy/10;
	}
	lastPos = event->pos();

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