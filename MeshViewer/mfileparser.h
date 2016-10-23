#pragma once

#ifndef MFILEPARSER_INCLUDED
#define MFILEPARSER_INCLUDED

#include <qstring.h>
#include <QTextStream>
#include <qfile.h>
#include <qmessagebox.h>
#include "mesh.h"

#include <qtextbrowser.h>

class MFileParser {

public:
	MFileParser(QString fileName, Mesh& mesh, QTextBrowser* parent);
	~MFileParser();

private:
	QString fileData;
	QString verticesData;
	QString facesData;

	Mesh* m_mesh;

	std::vector<HE_edge*> edgeStorage;

	void convertToVertices();
	void convertToFacesAndEdges();
	void calculateEdgePairs();

	/*void calculateVertexEdges();*/

};






#endif //MFILEPARSER_INCLUDED