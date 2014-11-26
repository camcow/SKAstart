#include "MotionGraphController.h"


MotionGraphController::MotionGraphController(MotionGraph &input)
{
	g = input;
	cout << "initializing Motion Graph Controller \n Reading all frames to test first \n then Checking for neighbors \n" << endl;
	// pretty much tests to see if all frames are readable;
	readAllFrames();

}
bool MotionGraphController::isTransitionPoint(MotionGraph::DirectedGraph::vertex_descriptor m)
{
	
	// adjacency iterators or neighbors
	std::pair<MotionGraph::neighbor_iterator, MotionGraph::neighbor_iterator> neighbors =
		boost::adjacent_vertices(m, g.dgraph);
	//iterate through all neighbors
	int neighborCount = 0;
	for (; neighbors.first != neighbors.second; ++neighbors.first)
	{
	//	std::cout << "neighbors for  " << g.dgraph[m].frame_data.fileName << g.dgraph[m].frame_data.frame_number << " is" << g.dgraph[*neighbors.first].frame_data.fileName << g.dgraph[*neighbors.first].frame_data.frame_number << endl;
		neighborCount++;
	}
// if it has 2 or more neighbors then that means it is a transition
	if (neighborCount >= 2)
	{
		cout << "IS TRANSITION POINT" << endl;
		return(true);
	}
	//cout << "not a transition point" << endl;
	return(false);
}

void MotionGraphController::readAllFrames()
{
	pair<MotionGraph::vertex_iter, MotionGraph::vertex_iter> vp;
	int jo = 0;
	for (vp = vertices(g.dgraph); vp.first != vp.second;  vp.first++)
	{
		//cout << "FileName: " << g.dgraph[*vp.first].frame_data.fileName << endl;
		//cout << "Frame Number" << g.dgraph[*vp.first].frame_data.frame_number << endl;
		//cout << "Vertex Name" << g.dgraph[*vp.first].frame_data.vertexName << endl;
		//cout << jo << endl;
		//cout << endl;
		//cout << endl;
		testLinearOfMotionGraph(*vp.first);
		jo++;
	//	Sleep(2000);
		
	}
}

bool MotionGraphController::testLinearOfMotionGraph(MotionGraph::DirectedGraph::vertex_descriptor m)
{

	// adjacency iterators or neighbors
	std::pair<MotionGraph::neighbor_iterator, MotionGraph::neighbor_iterator> neighbors =boost::adjacent_vertices(m, g.dgraph);
	//iterate through all neighbors
	int neighborCount = 0;
	for (; neighbors.first != neighbors.second; ++neighbors.first)
	{
		//	std::cout << "neighbors for  " << g.dgraph[m].frame_data.fileName << g.dgraph[m].frame_data.frame_number << " is" << g.dgraph[*neighbors.first].frame_data.fileName << g.dgraph[*neighbors.first].frame_data.frame_number << endl;
		neighborCount++;

	}
	// no neighbors for end of each file
	if (neighborCount == 0)
	{
		//for no neighbors aka end of linear file
		std::cout << "No neighbors for  " << g.dgraph[m].frame_data.fileName << "frame number: " << g.dgraph[m].frame_data.frame_number << endl;
	}
	// if it has 2 or more neighbors then that means it is a transition
	if (neighborCount >= 2)
	{
		cout << "IS TRANSITION POINT" << endl;
		return(true);
	}
	//cout << "not a transition point" << endl;
	return(false);
}