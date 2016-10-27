#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLWidget>
#include <glut.h>

#define MODE_TRANSLATION 0
#define MODE_ROTATION 1
#define MODE_ZOOM 2

static int mode = 0;

class GLViewer : public QGLWidget {

	Q_OBJECT

public:
	GLViewer(QWidget* parent = 0);
	~GLViewer();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

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