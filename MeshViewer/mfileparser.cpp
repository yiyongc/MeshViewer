#include "mfileparser.h"

MFileParser::MFileParser(QString fileName, Mesh& mesh, QTextBrowser* random) {
	m_mesh = &mesh;

	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly))
		QMessageBox::information(0, "info", file.errorString());

	QTextStream fileIn(&file);

	QString line;

	do {
		line = fileIn.readLine();

		//Ignore comments
		if (line.contains("#"))
			continue;

		if (line.contains("Vertex")) {
			line.remove("Vertex ");
			line.replace("  ", ",");
			line.replace(" ", ",");
			verticesData.append(line + "\n");
		}
		else if (line.contains("Face")) {
			line.remove("Face ");
			line.replace("  ", ",");
			line.replace(" ", ",");
			facesData.append(line + "\n");
		}
			
	} while (!line.isNull());

	file.close();

	convertToVertices();

	convertToFacesAndEdges();

	calculateEdgePairs();


	random->setText("Done");

	
}

MFileParser::~MFileParser() {}


void MFileParser::convertToVertices() {
	QString line;
	QStringList lines;
	QStringList vertData;

	lines = verticesData.split("\n");

	for (int i = 0; i < lines.size() - 1 ; i++) {

		line = lines[i];
		vertData = line.split(",");

		HE_vert* v1 = new HE_vert(vertData[0].toInt(), vertData[1].toFloat(), vertData[2].toFloat(), vertData[3].toFloat());

		m_mesh->addVertex(v1);
	}
}

void MFileParser::convertToFacesAndEdges() {
	QString line;
	QStringList lines;
	QStringList faceData;

	lines = facesData.split("\n");

	HE_vert *v1, *v2, *v3;
	HE_edge *e1, *e2, *e3;
	HE_face* face;

	for (int i = 0; i < lines.size() - 1; i++) {
		line = lines[i];
		
		faceData = line.split(",");

		//Faces tell us which vertices are connected to each other
		//we need to create and store the HE edge and HE faces

		v1 = m_mesh->getVertices().at(faceData[1].toInt() - 1);
		v2 = m_mesh->getVertices().at(faceData[2].toInt() - 1);
		v3 = m_mesh->getVertices().at(faceData[3].toInt() - 1);

		//v1-v2
		e1 = new HE_edge(v2);

		face = new HE_face(e1);

		//v2-v3
		e2 = new HE_edge(v3, face, e1);

		//v3-v1
		e3 = new HE_edge(v1, face, e2, e1);
		
		e1->setFace(*face);
		e1->setNext(*e2);
		e1->setPrev(*e3);
		
		e2->setNext(*e3);

		//Add to mesh structure
		//m_mesh->addEdge(e1);
		//m_mesh->addEdge(e2);
		//m_mesh->addEdge(e3);
		edgeStorage.push_back(e1);
		edgeStorage.push_back(e2);
		edgeStorage.push_back(e3);

		m_mesh->addFace(face);

		//Set up vertex with edges
		if (v1->getEdge() == nullptr)
			v1->setEdge(*e1);
		if (v2->getEdge() == nullptr)
			v2->setEdge(*e2);
		if (v3->getEdge() == nullptr)
			v3->setEdge(*e3);
	}
}

void MFileParser::calculateEdgePairs() {
	int size = m_mesh->getEdges().size();
	//std::vector<std::vector<int>> edges;
	HE_edge* e1, *e2;
	int start1, start2, end1, end2;

	////Obtain vector with edge data
	//for (int i = 0; i < size; i++) {
	//	e1 = m_mesh->getEdges().at(i);

	//	std::vector<int> edge;
	//	// Edge contains start vertID, end vertID
	//	edge.push_back(e1->getPrev()->getVert()->getID());
	//	edge.push_back(e1->getVert()->getID());

	//	edges.push_back(edge);
	//}

	//int start1, start2, end1, end2;

	////Do comparison to find vertex pairs
	//for (int j = 0; j < size - 1; j++) {
	//	start1 = edges.at(j)[0];
	//	end1 = edges.at(j)[1];

	//	for (int m = j + 1; m < size; m++) {
	//		start2 = edges.at(m)[0];
	//		end2 = edges.at(m)[1];

	//		if (start1 == end2 && start2 == end1) {
	//			m_mesh->getEdges().at(j)->setPair(*m_mesh->getEdges().at(m));
	//			m_mesh->getEdges().at(m)->setPair(*m_mesh->getEdges().at(j));
	//		}
	//	}
	//}

	//for (int i = 0; i < size - 1; i++) {
	//	e1 = m_mesh->getEdges().at(i);

	//	if (e1->getPair() != nullptr)
	//		continue;

	//	end1 = e1->getVert()->getID();
	//	start1 = e1->getPrev()->getVert()->getID();

	//	for (int j = i + 1; j < size; j++) {
	//		e2 = m_mesh->getEdges().at(j);
	//		
	//		if (e2->getPair() != nullptr)
	//			continue;
	//		
	//		end2 = e2->getVert()->getID();
	//		start2 = e2->getPrev()->getVert()->getID();
	//		if (start1 == end2 && start2 == end1) {
	//			e1->setPair(*e2);
	//			e2->setPair(*e1);
	//		}
	//	}
	//}

	while (edgeStorage.size() != 0) {
		e1 = edgeStorage.at(0);
		edgeStorage.erase(edgeStorage.begin());

		start1 = e1->getPrev()->getVert()->getID();
		end1 = e1->getVert()->getID();

		for (int i = 0; i < edgeStorage.size(); i++) {
			e2 = edgeStorage.at(i);
			end2 = e2->getVert()->getID();
			start2 = e2->getPrev()->getVert()->getID();
			if (start1 == end2 && start2 == end1) {
				edgeStorage.erase(edgeStorage.begin() + i);
				e1->setPair(*e2);
				e2->setPair(*e1);
				m_mesh->addEdge(e1);
				m_mesh->addEdge(e2);
			}
		}
	}

}
//
//void MFileParser::calculateVertexEdges(){
//	int size = m_mesh->getVertices().size();
//	int edgesize = m_mesh->getEdges().size();
//	HE_edge* e1;
//
//	for (int i = 0; i < size; i++) {
//		for (int j = 0; j < edgesize; j++) {
//			e1 = m_mesh->getEdges().at(j);
//
//			int edgeEndVertexID = e1->getVert()->getID();
//
//			if (edgeEndVertexID == i + 1) {
//				m_mesh->getVertices().at(i)->setEdge(*e1); 
//				break;
//			}
//		}
//	}
//}