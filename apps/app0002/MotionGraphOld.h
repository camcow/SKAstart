#include "DataInput.h"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"

class MotionGraph
{	
	
	/////////////////////////////Defining what a vertex of our graph///////////////////////////
		struct RootNode
	{
		Quaternion quaternion;
		double PosX, PosY, PosZ;
		
	};

	struct Node
	{
		Quaternion quaternion;	
		int nodeCount;
	};

	//contains an entire motion sequence
	struct Vertex
	{
		string vertexName;
		RootNode root;
		std::vector<Node> nodes;
		// Number of vertexs in the motionGraph starts at 0
		int vertexCount;
		string fileName;
	};
	/////////////////////////////////////end of Vertex description//////////////////////////////////////

	////////////////////////////////////Definition of the composition of an edge/////////////////////// 

	struct Edge{std::string blah;};

	////////////////////////////////////End of Edge Definition////////////////////////////////////////


	//Define the graph using those classes////////
	
	typedef boost::adjacency_list<boost::listS, boost::vecS,boost:: undirectedS, Vertex, Edge > Graph;
	//Some typedefs for simplicity
	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
	
	private:
	vector<string>Files;
	int vertexCount;

	

	public:
	Graph g;
	MotionGraph();
	void graphToFile();
	void fileReader(string fileName);
	void fileLoader();

};