#include "meshviewer.h"

MeshViewer::MeshViewer(QWidget *parent)
	: QMainWindow(parent)
{
	fileMesh = new Mesh();
	//Initialize Window and Body Layout
	QWidget* window = new QWidget;
	QVBoxLayout* windowLayout = new QVBoxLayout;
	QWidget* body = new QWidget;
	QHBoxLayout* bodyLayout = new QHBoxLayout;
	
	//Set up menu options
	QMenuBar* menubar = new QMenuBar;

	QMenu* menu1 = new QMenu("File");

	QAction* action_open_file = new QAction("Open M File", menu1);

	connect(action_open_file, SIGNAL(triggered()), this, SLOT(importNewFile()));

	menu1->addAction(action_open_file);

	menubar->addMenu(menu1);
	menubar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


	//Obtaining OpenGL widget
	glViewer = new GLViewer;
	text = new QTextBrowser;


	//Adding widgets and layouts together
	bodyLayout->addWidget(glViewer);
	bodyLayout->addWidget(text);
	bodyLayout->setStretchFactor(glViewer, 100);
	bodyLayout->setStretchFactor(text, 30);

	body->setLayout(bodyLayout);

	windowLayout->addWidget(menubar);
	windowLayout->addWidget(body);
	window->setLayout(windowLayout);
		
	
	setCentralWidget(window);
	setWindowTitle(tr("M Mesh Viewer"));
	//setWindowState(Qt::WindowMaximized);
}

MeshViewer::~MeshViewer()
{

}


void MeshViewer::importNewFile(){

	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open M Mesh File"),
		"",
		"M file (*.m)"
		);

	if (fileName != NULL){
		//ImportDialog* layerControllerSetting = new ImportDialog(m_LayerController, filename);
		//layerControllerSetting->exec();
		MFileParser fileParser(fileName, *fileMesh, text);
		glViewer->setModel(fileMesh);
	}

}
