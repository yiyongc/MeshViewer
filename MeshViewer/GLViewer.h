#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLWidget>
#include <qopenglshaderprogram.h>
#include <glut.h>
#include "mesh.h"

#define MODE_TRANSLATION 0
#define MODE_ROTATION 1
#define MODE_ZOOM 2

static int mode = 0;

class GLViewer : public QGLWidget {

	Q_OBJECT

public:
	GLViewer(QWidget* parent = 0);
	~GLViewer();
	void setModel(Mesh* model);
	enum {
		eFragmentColor = 0x00FF00,
		Flat = 1,
		Smooth = 2
	};

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void freeRenderData();


	static void qNormalizeAngle(int &angle);

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent* event);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void drawGrid();
	void drawPyramid();
	void drawZAxis();
	void drawAxis();
	void drawModel(int shading);
	void drawModelPoints();
	void drawModelWire();
	
private:
	int xRot;
	int yRot;
	int zRot;
	float xTrans, yTrans;
	float scaleSize;
	QPoint lastPos;

	int dx, dy;
	int rotateZState = -1;
	bool clockWise = false;

	Mesh*					m_model;
	GLuint                  m_hVertexes;
	GLuint                  m_hNormals;
	QOpenGLShaderProgram    m_shaderProgram;
	int                     m_coordVertex;
	int                     m_coordNormal;
	int                     m_matrixVertex;
	int                     m_matrixNormal;
	int                     m_colorFragment;

private:
	static float* generateVertexBuffer(Mesh& model);
	static void generateNormalsBuffer(Mesh& model, float* coords);
	static void releaseVertexBuffer(float* buffer);


	public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);





signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

};



#endif //GLVIEWER_H