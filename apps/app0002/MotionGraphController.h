#ifndef MOTIONGRAPHCONTROLLER_DOT_H
#define MOTIONGRAPHCONTROLLER_DOT_H
#include "MotionGraph.h"
#include <Core/SystemConfiguration.h>
#include <Animation/MotionController.h>
#include <Windows.h>
#include <vector>
using namespace std;
class MotionGraphController : public MotionController
{
	public:
	MotionGraphController(MotionGraph &input);
	MotionGraph g;
	//~MotionGraphController();

	//channels need to figure out how to set these up.
	virtual bool isValidChannel(CHANNEL_ID _channel, float _time){ return true; };
	virtual float getValue(CHANNEL_ID _channel, float _time){ return 0; };


	// takes in a vertex_descriptor then checks to see if it has any neighbors
	bool isTransitionPoint(MotionGraph::DirectedGraph::vertex_descriptor m);


	// take transition of first transition
	void takeTransition();
// take first transition of filename
	void takeTransition(string filename);
	// take transition to the same motion;
	void takeTransition(bool sameMotion);









	/*Debugging code and functions*/
	/////////////////////////////////////////////////////////////////////////////////////

	// only used before connections. Is to test weather inputting files is making frames connect linearly correctly.
	bool testLinearOfMotionGraph(MotionGraph::DirectedGraph::vertex_descriptor m);
	void readAllFrames();
};




//graph whatever type it is
//MotionGraph g;

#endif

/* cameron notes


// name spcaes are messeed up. there is anohter class called frame already
// this points to Frame in dataInput
/*
Frame g1;
//this points to the frame structure in motionGraph class
MotionGraph::Frame g2;



*/

// need something find a vertex by filename and frame number on graph
//look ahead and find the nearest transition points. this should return the filename and frame number or whole frame.
//transition commands.
//currently when inputting the things to the graph it is all un-connected. my code changed that 
// we need to make each file's info have edges connect to each other.
// we need a way to make each file have edges to its verticies in the graph, but not connect to the other files's verticies . 

/*

I think I have found such mechanism. It is called labeled_graph and is a part of BGL. Instead of using adjacency_list, one can use a predefined wrapper labeled_graph:

typedef boost::labeled_graph<
boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, Data >,
std::string
> Graph;
After defining a graph like this, it is possible to access vertices in the following manner:

Graph g;

boost::add_vertex( "Alpha", g );
g["Alpha"].name  = "Alpha";
g["Alpha"].value = 10;

boost::add_vertex( "Beta", g );
g["Beta"].name  = "Beta";
g["Beta"].value = 20;

boost::add_edge_by_label( "Alpha", "Beta", g );

The side effect of this is that one need to use graph() member function to make some algorithms work:

std::vector< Graph::vertex_descriptor > container;
boost::topological_sort( g.graph(), std::back_inserter( container ) ) ;

For some reason, labeled_graph is not described in BGL documentation, but it appears in the example folder.



*/