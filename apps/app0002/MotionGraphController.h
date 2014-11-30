#ifndef MOTIONGRAPHCONTROLLER_DOT_H
#define MOTIONGRAPHCONTROLLER_DOT_H
#include "MotionGraph.h"
#include <Core/SystemConfiguration.h>
#include <Animation/MotionController.h>
#include <Animation/MotionSequence.h>
#include <Windows.h>
#include <vector>
#include "AppConfig.h"
#include <boost/filesystem.hpp>
#include "boost/filesystem/path.hpp"
#include "DataManagement/DataManager.h"
#include "DataManagement/DataManagementException.h"

using namespace std;

class MotionGraphController : public MotionController
{
	struct state{
		string SeqID;
		int FrameNumber;
		string TransitionToSeqId;
		int FrameNumberTransition;
		bool isTransitioning;
	};
	//used for transitioning sequence input
	// the list or sequence of vertex targets will be saved in a vector.
	struct vertexTargets
	{
		string SeqID;
		 int FrameNumber;
	};

	struct MotionSequenceContainer
	{
		MotionSequence *MS;
		string SeqID;
	};

private:
	vector<MotionSequenceContainer> MsVector;
	

	MotionGraph g;
	state status;
	list<vertexTargets> path;
	//curent vertex on graph for iterating purposes on graph
	MotionGraph::DirectedGraph::vertex_descriptor CurrentVertex;


	float last_transition_time;// system time when the last transition was taken
	long last_transition_frame;//first frame in current motion that was played when the last transition was taken
	float frame_rate = 120.0;

public:
	MotionGraphController(MotionGraph &input);
	~MotionGraphController();

	// need to figure out how to set these up.
	virtual bool isValidChannel(CHANNEL_ID _channel, float _time){ return true; };
	virtual float getValue(CHANNEL_ID _channel, float _time);

	//get current frame of currently played motion sequence
	long computeCurrentFrame(float _time);
	
	// takes in a vertex_descriptor then checks to see if it has any neighbors
	bool isTransitionPoint(MotionGraph::DirectedGraph::vertex_descriptor m);
	//reads in all the motion sequences
	void readInMotionSequences();
	// take transition of first transition
	void takeTransition();
	// take first transition of filename
	void takeTransition(string filename);
	// take transition to the same motion;
	void takeTransition(bool sameMotion);

	//searches whole graph for a specific point. 
	MotionGraph::DirectedGraph::vertex_descriptor MotionGraphController::FindVertex(string sequenceID, int frameNumber);

	// takes commands . 
	//void takeTranistion(vector<#unknownTransitionStructure>);

	// is it time to transition?
	bool MotionGraphController::timeToTransition(float time);
	

	//returns the motionsequence container from ID
	MotionSequenceContainer returnMotionSequenceContainerFromID(string ID);

	/*Debugging code and functions*/
	/////////////////////////////////////////////////////////////////////////////////////

	// only used before connections. Is to test weather inputting files is making frames connect linearly correctly.
	bool testLinearOfMotionGraph(MotionGraph::DirectedGraph::vertex_descriptor m);
	//reads all the frames of the graph
	void readAllFrames();
	//reads all the names of the MotionSequences MsVector
	void readAllSequenceIDs();
};




//graph whatever type it is
//MotionGraph g;

#endif

/* cameron notes



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

//OLD CODE
/*
//finds the int place in the vector where this ID is stored in MsV
int findSeqID(string ID);*/