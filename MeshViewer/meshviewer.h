#ifndef MESHVIEWER_H
#define MESHVIEWER_H

#include <QtWidgets/QMainWindow>
#include "ui_meshviewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "GLViewer.h"
#include "qfiledialog.h"
#include "mfileparser.h"


#include <qtextbrowser.h>

class MeshViewer : public QMainWindow
{
	Q_OBJECT

public:
	MeshViewer(QWidget *parent = 0);
	~MeshViewer();

	private Q_SLOTS:
	void importNewFile();

private:
	QTextBrowser* text;
	GLViewer* glViewer;
	Mesh* fileMesh;

	Ui::MeshViewerClass ui;
};

#endif // MESHVIEWER_H
