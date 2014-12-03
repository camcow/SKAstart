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
#include <Animation/AnimationException.h>
using namespace std;

class MotionGraphController : public MotionController
{
	struct state{
		string SeqID;// current sequence or filename of the motion sequence being played
		int FrameNumber;// current frame number
		int FrameNumberTransition; // what frame number is the transition
		string TransitionToSeqId;// seqId to transition to
		int FrameNumberTransitionTo;// what frame to start on the new motion sequence
		bool isTransitioning;
	};
	//used for transitioning sequence input
	// the list or sequence of vertex targets will be saved in a list.
	struct vertexTargets
	{ 
		///motion A frame 2 ----to---- motion B frame 3;
		string SeqID;
		 int FrameNumber;
		 // what you transition to
		 string SeqID2;
		 int FrameNumber2;
	};

	struct MotionSequenceContainer
	{
		MotionSequence *MS;
		string SeqID;
	};

private:
	vector<MotionSequenceContainer> MsVector;// contains all the motion sequences
	MotionGraph g;
	state status;//status of the current playing animation
	list<vertexTargets> path; // path the motion graph controller will take when animating
	list<vertexTargets> pathBackup;// used to loop through the path once again;
	MotionGraph::DirectedGraph::vertex_descriptor CurrentVertex;//curent vertex on graph for iterating purposes on graph
	float last_transition_time=0;// system time when the last transition was taken
	long last_transition_frame=0;//first frame in current motion that was played when the last transition was taken
	float frame_rate = 120.0f;
	float character_size_scale = 0.2f;//the amount we need to scale the motion sequence

public:
	MotionGraphController(MotionGraph &input);

	~MotionGraphController();

	//called by skeleton
	virtual bool isValidChannel(CHANNEL_ID _channel, float _time);

	//called by skeleton to get value
	virtual float getValue(CHANNEL_ID _channel, float _time);

	//get current frame of currently played motion sequence
	long computeCurrentFrame(float _time);

	// takes in a vertex_descriptor then checks to see if it has any neighbors
	bool isTransitionPoint(MotionGraph::DirectedGraph::vertex_descriptor m);

	//reads in all the motion sequences
	void readInMotionSequences();

	//searches whole graph for a specific point. 
	MotionGraph::DirectedGraph::vertex_descriptor MotionGraphController::FindVertex(string sequenceID, int frameNumber);

	//iterate graph to one vertex ahead or by one frame
	void iterateMotionGraph();

	// used after transition to set the graph to the correct vertex
	void transitionGraph();

	// is it time to transition? 
	bool timeToTransition(float time);

	//updates the information of the status variable to the next target;
	//does not update status.FrameNumber or the current frame being played
	void iterateStatus();

	//returns the motionsequence container from ID
	MotionSequenceContainer returnMotionSequenceContainerFromID(string ID);

	// set the path the animation will follow
	void setPath(list<vertexTargets> inputPath);

	/*Debugging code and functions*/
	/////////////////////////////////////////////////////////////////////////////////////

	// only used before connections. Is to test weather inputting files is making frames connect linearly correctly.
	bool testLinearOfMotionGraph(MotionGraph::DirectedGraph::vertex_descriptor m);

	//reads all the frames of the graph
	void readAllFrames();

	//reads all the names of the MotionSequences MsVector
	void readAllSequenceIDs();

	// this is for testing with motion sequences
	// only to be used if looping the same motion sequence, not if we are using multiple different motion sequences
	int computeMotionSequenceFrame(MotionSequence *MS, float _time);

	// print the status variable
	void printStatus();
};
#endif