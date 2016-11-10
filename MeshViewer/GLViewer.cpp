#include "GLViewer.h"
#include <qtimer.h>
#include <QMouseEvent>
#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <thread>
#include <qvector2d.h>
#include <QOpenGLFunctions>

GLViewer::GLViewer(QWidget* parent) : xRot(0), yRot(0), zRot(0), scaleSize(1.0), xTrans(0), yTrans(0), m_hVertexes(0), m_hNormals(0), m_coordVertex(-1)
, m_coordNormal(-1)
, m_matrixVertex(-1)
, m_matrixNormal(-1)
, m_colorFragment(-1)
, m_model(NULL) {

	//Updates GL Widget every 10ms
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);

}

GLViewer::~GLViewer() {
	//freeRenderData();
}

//void GLViewer::freeRenderData() {
//	QOpenGLFunctions funcs(QOpenGLContext::currentContext());
//	if (0 != m_hVertexes)
//	{
//		funcs.glDeleteBuffers(1, &m_hVertexes);
//		m_hVertexes = 0;
//	}
//}




void GLViewer::initializeGL() {
	glClearColor(0.8, 0.8, 0.8, 0.0);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	//shading
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);

	// Create light components.
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 }; // light position
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 }; // light color
	GLfloat diffuse[] = { 0.3, 0.5, 0.6, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	
	// Assign created components to GL_LIGHT0.
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);


	/*static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);*/
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

	//Model drawing functions
	drawModel(Smooth);
	//drawModel(Flat);
	//drawModelWire();
	//drawModelPoints();
	drawBBox();
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

void GLViewer::setModel(Mesh* model) {
	//freeRenderData();
	if (model == nullptr)
	{
		return;
	}
	
	m_model = model;

	/*float* coords = generateVertexBuffer(m_model);

	if (0 != coords)
	{
		QOpenGLFunctions funcs(QOpenGLContext::currentContext());
		funcs.glGenBuffers(1, &m_hVertexes);
		funcs.glGenBuffers(1, &m_hNormals);
		if ((0 != m_hVertexes) && (0 != m_hNormals))
		{
			funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hVertexes);
			funcs.glBufferData(GL_ARRAY_BUFFER,
				(3 * m_model->getVertices().size() * sizeof(float)),
				coords,
				GL_STATIC_DRAW);

			generateNormalsBuffer(m_model, coords);
			funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hNormals);
			funcs.glBufferData(GL_ARRAY_BUFFER,
				(3 * m_model->getVertices().size() * sizeof(float)),
				coords,
				GL_STATIC_DRAW);

			funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else
		{
			freeRenderData();
		}
		releaseVertexBuffer(coords);
	}*/
}




void GLViewer::mousePressEvent(QMouseEvent *event) { 
	lastPos = event->pos();
	
	if (event->buttons() & Qt::LeftButton) {

		float mouseMovementAngle = 0;

		QVector2D startPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		QVector2D firstPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		QVector2D secondPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
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
	
	//Rotation
	if (event->buttons() & Qt::LeftButton && rotateZState != -1) {
		if (rotateZState == 1) {
			if (clockWise)
				setZRotation(zRot - 2);
			else
				setZRotation(zRot + 2);
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
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex3f((float) i, 0, -10);
		glVertex3f((float)i, 0, 10);
		glVertex3f(-10, 0, (float) i);
		glVertex3f(10, 0, (float) i);
	}
	glEnd();
}


void GLViewer::drawZAxis() {
	GLUquadric* gluQuad = gluNewQuadric();
	gluCylinder(gluQuad, 0.01, 0.01, 5, 32, 32);
	glPushMatrix();
	glTranslated(0, 0, 5);
	glutSolidCone(0.05, 0.3, 32, 32);
	glPopMatrix();
}

void GLViewer::drawAxis() {
	//X-axis
	qglColor(Qt::darkRed);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawZAxis();
	glPopMatrix();

	//Y-axis
	qglColor(Qt::darkGreen);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	drawZAxis();
	glPopMatrix();

	//Z-axis
	qglColor(Qt::darkBlue);
	glEnable(GL_COLOR_MATERIAL);
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

void GLViewer::drawModelPoints() {
	if (m_model == nullptr)
		return;

	std::vector<HE_vert*> vertSrc = m_model->getVertices();
	int vertCount = vertSrc.size();

	int vertexCount = vertSrc.size();
	qglColor(Qt::red);
	glPointSize(5);
	for (int i = 0; i < vertexCount; i++) {
		glBegin(GL_POINTS);
		glVertex3f(vertSrc.at(i)->getX(), vertSrc.at(i)->getY(), vertSrc.at(i)->getZ());
		glEnd();
	}
	//// Set Shader Program object' parameters
	////m_shaderProgram.setUniformValue(m_matrixVertex, matrixVertex);
	////m_shaderProgram.setUniformValue(m_matrixNormal, m_matrixRotate);
	//QColor fragmentColor(eFragmentColor);
	//m_shaderProgram.setUniformValue(m_colorFragment,
	//	static_cast<GLfloat>(fragmentColor.red()) / 256.0f,
	//	static_cast<GLfloat>(fragmentColor.green()) / 256.0f,
	//	static_cast<GLfloat>(fragmentColor.blue()) / 256.0f);

	//QOpenGLFunctions funcs(QOpenGLContext::currentContext());

	//// Vertex data
	//glEnableClientState(GL_VERTEX_ARRAY);
	//funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hVertexes);
	//glVertexPointer(3, GL_FLOAT, 0, 0);
	//funcs.glVertexAttribPointer(m_coordVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//// Normal data
	//glEnableClientState(GL_NORMAL_ARRAY);
	//funcs.glBindBuffer(GL_ARRAY_BUFFER, m_hNormals);
	//glNormalPointer(GL_FLOAT, 0, 0);
	//funcs.glEnableVertexAttribArray(m_coordNormal);
	//funcs.glVertexAttribPointer(m_coordNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//// Draw frame
	//glDrawArrays(GL_TRIANGLES, 0, (3 * m_model.getFaces().size()));

	//funcs.glDisableVertexAttribArray(m_coordNormal);
	//funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
}

void GLViewer::drawModelWire() {
	//Check if model exists
	if (m_model == nullptr)
		return;

	std::vector<HE_face*> faceSrc = m_model->getFaces();
	int faceCount = faceSrc.size();

	qglColor(Qt::blue);
	for (int j = 0; j < faceCount; j++)
	{
		HE_vert* v1 = faceSrc.at(j)->getEdge()->getVert();
		HE_vert* v2 = faceSrc.at(j)->getEdge()->getPrev()->getVert();
		HE_vert* v3 = faceSrc.at(j)->getEdge()->getNext()->getVert();

		glBegin(GL_LINE_LOOP);
		// get the three vertices of f
		glVertex3f(v1->getX(), v1->getY(), v1->getZ());
		glVertex3f(v2->getX(), v2->getY(), v2->getZ());
		glVertex3f(v3->getX(), v3->getY(), v3->getZ());
		glEnd();
	}
}

void GLViewer::drawModel(int shading) {
	//Check if model exists
	if (m_model == nullptr)
		return;

	//Check shading type
	if (shading == Flat) 
		glShadeModel(GL_FLAT);
	else if (shading = Smooth) 
		glShadeModel(GL_SMOOTH);
	else
		return;

	std::vector<HE_face*> faceSrc = m_model->getFaces();
	std::vector<QVector3D> normalSrc = m_model->getNormals();
	int faceCount = faceSrc.size();

	qglColor(Qt::darkYellow);
	for (int k = 0; k < faceCount; k++)	{
		HE_vert* v1 = faceSrc.at(k)->getEdge()->getVert();
		HE_vert* v2 = faceSrc.at(k)->getEdge()->getPrev()->getVert();
		HE_vert* v3 = faceSrc.at(k)->getEdge()->getNext()->getVert();
		QVector3D normal1 = normalSrc.at(v1->getID() - 1);
		QVector3D normal2 = normalSrc.at(v2->getID() - 1);
		QVector3D normal3 = normalSrc.at(v3->getID() - 1);


		glBegin(GL_TRIANGLES);
		// get both the normal and coordinates
		glNormal3f(normal1.x(), normal1.y(), normal1.z()); glVertex3f(v1->getX(), v1->getY(), v1->getZ());
		glNormal3f(normal2.x(), normal2.y(), normal2.z()); glVertex3f(v2->getX(), v2->getY(), v2->getZ());
		glNormal3f(normal3.x(), normal3.y(), normal3.z()); glVertex3f(v3->getX(), v3->getY(), v3->getZ());
		glEnd();
	}
}

//float* GLViewer::generateVertexBuffer(Mesh& model)
//{
//	const unsigned int vertexCount = model.getVertices().size();
//	float* pointCoord = new float[3 * vertexCount]();
//	float* abc = new float[3] {1, 2, 3};
//	if (0 != pointCoord)
//	{
//		//const unsigned int* indexes = model.GetIndexes();
//		std::vector<HE_vert*> pointCoordSrc = model.getVertices();
//		float* coord = pointCoord;
//		
//		for (unsigned int vertex = 0; vertex < vertexCount; ++vertex) {
//				HE_vert* coordSrc = pointCoordSrc.at(vertex);
//				(*(coord++)) = (coordSrc->getX());
//				(*(coord++)) = (coordSrc->getY());
//				(*(coord++)) = (coordSrc->getZ());
//		}
//	}
//
//	return pointCoord;
//}
//
//
//void GLViewer::generateNormalsBuffer(Mesh& model, float* coords)
//{
//	std::vector<QVector3D> normalSrc = model.getNormals();
//	
//	for (unsigned int normal = 0; normal < normalSrc.size(); ++normal)
//	{
//		(*(coords++)) = normalSrc.at(normal).x();
//		(*(coords++)) = normalSrc.at(normal).y();
//		(*(coords++)) = normalSrc.at(normal).z();		
//	}
//}


//void GLViewer::releaseVertexBuffer(float* buffer)
//{
//	delete[] buffer;
//}


void GLViewer::drawBBox() {
	if (m_model == nullptr)
		return;

	float* bbox = m_model->getBBoxValue();


	qglColor(Qt::black);
	glLineWidth(1.2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex3f(bbox[1], bbox[3], bbox[5]);
		glVertex3f(bbox[1], bbox[3], bbox[6]);
		glVertex3f(bbox[2], bbox[3], bbox[6]);
		glVertex3f(bbox[2], bbox[3], bbox[5]);

		glVertex3f(bbox[1], bbox[4], bbox[5]);
		glVertex3f(bbox[1], bbox[4], bbox[6]);
		glVertex3f(bbox[2], bbox[4], bbox[6]);
		glVertex3f(bbox[2], bbox[4], bbox[5]);
		
		glVertex3f(bbox[1], bbox[3], bbox[5]);
		glVertex3f(bbox[1], bbox[3], bbox[6]);
		glVertex3f(bbox[1], bbox[4], bbox[6]);
		glVertex3f(bbox[1], bbox[4], bbox[5]);

		glVertex3f(bbox[2], bbox[3], bbox[5]);
		glVertex3f(bbox[2], bbox[3], bbox[6]);
		glVertex3f(bbox[2], bbox[4], bbox[6]);
		glVertex3f(bbox[2], bbox[4], bbox[5]);

		glVertex3f(bbox[1], bbox[3], bbox[5]);
		glVertex3f(bbox[1], bbox[4], bbox[5]);
		glVertex3f(bbox[2], bbox[4], bbox[5]);
		glVertex3f(bbox[2], bbox[3], bbox[5]);

		glVertex3f(bbox[1], bbox[3], bbox[6]);
		glVertex3f(bbox[1], bbox[4], bbox[6]);
		glVertex3f(bbox[2], bbox[4], bbox[6]);
		glVertex3f(bbox[2], bbox[3], bbox[6]);

	glEnd();
	glLineWidth(1);

}