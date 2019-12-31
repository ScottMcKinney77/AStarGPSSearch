#include "myGraphics.h"
#include "CSVRead.h"
#include "ChainingStringHashMap.h"
#include <set> 

//NOTE: must compile using c++11 (g++ -std=c++11 -o exec declTypeTest.cc)

using namespace std;

class Edge;

class Vertex {
protected:
	string name;
	double x,y;
	vector<Edge*> adjacencyList;
	double distance, heuristic;
	Edge* parent; //For shortest path tracking
public:
	Vertex(string name, double x, double y) : name(name), x(x), y(y){
		distance = INT_MAX;
		parent = NULL;
	}
	string getName(){
		return this->name;
	}
	double getX(){
		return this->x;
	}
	double getY(){
		return this->y;
	}
	void setDistance(double distance){
		this->distance = distance;
	}
	double getDistance(){
		return this->distance;
	}
	double getHeuristic(Vertex* goal){
		return sqrt(pow((this->getX() - goal->getX()),2)+pow((this->getY() - goal->getY()),2));
	}
	vector<Edge*> getAdjacencyList(){
		return this->adjacencyList;
	}
	void addEdge(Edge* e){
		adjacencyList.push_back(e);
	}
	void setParent(Edge* e){
		this->parent = e;
	}
	Edge* getParent(){
		return this->parent;
	}
};

class Edge {
protected:
	Vertex* v1;
	Vertex* v2;
	double weight;
	Color* color;
public:
	Edge(Vertex* v1, Vertex* v2) : v1(v1), v2(v2) {
		weight = sqrt(pow((v1->getX() - v2->getX()),2)+pow((v1->getY() - v2->getY()),2));
		color = new Color("WHITE");
	}
	Vertex* getOtherVertex(Vertex* v){
		if (v->getName() == v1->getName()){
			return v2;
		}
		return v1;
	}
	double getWeight(){
		return this->weight;
	}
	Color* getColor(){
		return this->color;
	}
	void setColor(Color* color){
		this->color = color;
	}
};

/*
*	Expected Input has edges (r) and vertices (i) in same file
*	This method creates Vertex classes for every row labeled i in combined
*	Creation of edges depends on vertices they connect already be created
*	Therefore, edge classes can only be created after all vertices have been created
*	Decided not to rely on input data being ordered all vertexes before all edges
*	Also removes r/i identifiers
*/
void splitIandR(vector<vector<string>>& combined, vector<Vertex*>& vertices, vector<vector<string>>& edges){
	vector<string> newRow;
	for (vector<string> row : combined){
		newRow.clear(); 
		if (row[0] == "i"){
			Vertex* v = new Vertex(row[1], stod(row[3]), stod(row[2]));
			vertices.push_back(v);

		}else if(row[0] == "r"){
			row.erase(row.begin());
			for (string word : row){
				newRow.push_back(word);
			}
			edges.push_back(newRow);
		}else{
			cout << "Data not compatible: \n 	";
			for (string word: row){
				cout << word << ",";
			}
			cout << "\n";
		}
	}
}
ChainingHashMap<Vertex>* hashMapize(vector<Vertex*> vertices){
	ChainingHashMap<Vertex>* retMap = new ChainingHashMap<Vertex>(vertices.size());
	for (Vertex* v : vertices){
		retMap->add(v->getName(), v);
	}
	return retMap;
}
void getMinMaxDims(vector<Vertex*> vertices, double& minLat, double& maxLat, double& minLong, double& maxLong){
	double lat;
	double lon;
	for (Vertex* v : vertices){
		lat = v->getX();
		lon = v->getY(); 
		if (lat > maxLat){
			maxLat = lat; 
		}else if (lat < minLat){
			minLat = lat;
		}
		if (lon < minLong){
			minLong = lon;
		}else if (lon > maxLong){
			maxLong = lon;
		}
	}
}

void drawMap(vector<Vertex*> vertices){
	int vw = 1000; //Static width of window
	int vh = 800; //Static height of window
	int wallBuffer = 80; //distance that the min/max intersections are off the edge of window
	Color* black = new Color("BLACK");
	Color* white = new Color("WHITE");
	GraphicsWindow* graphicsW = new GraphicsWindow(vw,vh,black);

	double minLat, minLong, maxLat, maxLong;
	minLat = INT_MAX;
	minLong = INT_MAX;
	maxLong = INT_MIN;
	maxLat = INT_MIN;
	getMinMaxDims(vertices, minLat, maxLat, minLong, maxLong);
	
	double vertexLat, vertexLong, adjLat, adjLong;
	int drawX, drawY, adjX, adjY;
	for (Vertex* v : vertices){
		vertexLat = (v->getX() - minLat) / (maxLat-minLat);
		vertexLong = (v->getY() - minLong) / (maxLong-minLong);

		drawX = round(vertexLat * (vw - wallBuffer));
		drawY = round(vertexLong * (vh - wallBuffer));

		Vertex* adjacent;
		for (Edge* e : v->getAdjacencyList()){
			adjacent = e->getOtherVertex(v);
			adjLat = (adjacent->getX() - minLat) / (maxLat-minLat);
			adjLong = (adjacent->getY() - minLong) / (maxLong-minLong);

			adjX = round(adjLat * (vw - wallBuffer));
			adjY = round(adjLong * (vh - wallBuffer));
			
			graphicsW->drawLine(drawX + wallBuffer/2 , drawY + wallBuffer/2 , adjX + wallBuffer/2 , adjY + wallBuffer/2 , e->getColor());
		}
	}
	graphicsW->createWindow();
}

Vertex* getLowestF (set<Vertex*>* s, Vertex* goal){
	double curLowest = INT_MAX;
	Vertex* retVertex;
	for (Vertex* v : *s){
		if (v->getHeuristic(goal) + v->getDistance() < curLowest){
			retVertex = v;
			curLowest = v->getHeuristic(goal);
		}
	}
	return retVertex;
}
/* For larger maps, cannot store a vector of parents
*	at each node because of space issue, therefore 
*	this function redetermines shortest path for goal
* 	node after search is complete
*/
void parentBacktrack (Vertex* start, Vertex* goal){
	Vertex* curNode = goal;
	Color* red = new Color("RED");
	do{
		curNode->getParent()->setColor(red);
		curNode = curNode->getParent()->getOtherVertex(curNode);
	}while (curNode->getName() != start->getName());
}

void aStarSearch (Vertex* start, Vertex* goal){
	//Admissible Heuristic is straight line distance from currnode to goal node
	start->setDistance(0.0);
	set<Vertex*>* openSet = new set<Vertex*>();
	set<Vertex*>* closedSet = new set<Vertex*>();
	openSet->insert(start);

	Vertex* curNode;
	while (curNode->getName() != goal->getName()){
		curNode = getLowestF(openSet, goal);
		if (curNode->getName() == goal->getName()){
			break;
		}

		double checkDist;
		Vertex* neighbor;
		for (Edge* e : curNode->getAdjacencyList()){
			neighbor = e->getOtherVertex(curNode);
			checkDist = curNode->getDistance() + e->getWeight();
			if (openSet->find(neighbor) != openSet->end()){ //If node in openSet
				if (checkDist < neighbor->getDistance()){
					neighbor->setParent(e);
					neighbor->setDistance(checkDist);
				}
			}else if (closedSet->find(neighbor) != closedSet->end()){ //If node in closedSet
				if (checkDist < neighbor->getDistance()){
					neighbor->setParent(e);
					neighbor->setDistance(checkDist);
					closedSet->erase(neighbor);
					openSet->insert(neighbor);
				}
			}else{
				neighbor->setParent(e);
				neighbor->setDistance(checkDist);
				openSet->insert(neighbor);
			}
		}
		openSet->erase(curNode);
		closedSet->insert(curNode);
	}
	parentBacktrack(start, goal);
}

int main(){
	string filename = "monroe.txt";
	vector<vector<string>> table = readFromFile(filename);
	
	vector<Vertex*> vertices;
	vector<vector<string>> edges;
	
	splitIandR(table, vertices, edges);
	ChainingHashMap<Vertex>* vertexMap = hashMapize(vertices);
	
	Vertex* v1;
	Vertex* v2;
	
	//Used a hashtable to store vertecies so lookup here is closer to linear time
	//Otherwise this would be a very expensive loop
	Edge* e;
	for (vector<string> row : edges){
		v1 = vertexMap->get(row[1]);
		v2 = vertexMap->get(row[2]);
		e = new Edge(v1, v2);
		v1->addEdge(e);
		v2->addEdge(e);
	}
	Vertex* testStart = vertexMap->get("i50");
	Vertex* testGoal = vertexMap->get("i20");
	aStarSearch(testStart, testGoal);
	//Multiply long/lat by 69.2 to convert to miles
	cout << "Total Distance: " << testGoal->getDistance()*69.2 << " Miles " << "\n";

	drawMap(vertices);
}

