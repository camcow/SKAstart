#include "MotionGraphController.h"


MotionGraphController::MotionGraphController(MotionGraph &input)
{
	g = input;
	cout << "initializing Motion Graph Controller \n Reading all frames to test first \n then Checking for neighbors \n" << endl;
	// pretty much tests to see if all frames are readable;
	readAllFrames();
	readInMotionSequences();
	//read all the ids in the vector of MsVNames
	readAllSequenceIDs();
	//test if I can get the motionsequenceContiner by sequence ID going to be used to replace MsV and MsVNames
	MotionGraphController::MotionSequenceContainer test=returnMotionSequenceContainerFromID("swing6.bvh");
	cout << test.SeqID << endl;

}

MotionGraphController::~MotionGraphController()
{
	//delete all the motion sequences;
	for (int i = 0; i < MsVector.size(); i++)
	{

		delete MsVector.at(i).MS;
	}
	
}

bool MotionGraphController::timeToTransition(float time)
{// need to find out how to figure out when the time matches with the frame number. 
	MotionSequence *MS;
	MS = returnMotionSequenceContainerFromID(status.SeqID).MS;
	int currentFrame = computeCurrentFrame(time);
	if (currentFrame == status.FrameNumberTransition);
	return(true);
}


long MotionGraphController::computeCurrentFrame(float _time)
{
	//determine how long we have been playing the current motion
	float time_in_sequence = _time - last_transition_time;
	//determine how many frames have been played in the current motion
	long frames_in_sequence = time_in_sequence * frame_rate;
	//offset frames played by the start frame in the current motion
	long current_frame = frames_in_sequence + last_transition_frame;
	return(current_frame);
}
float MotionGraphController::getValue(CHANNEL_ID _channel, float _time){
	// if this is the first motion being played
	if (last_transition_time == NULL)
	{
		last_transition_time = _time;
		last_transition_frame = 0;
	}

	//if we are not transitioning at any time in the future
	long currentFrame;
	MotionSequence *MS;
	if (!status.isTransitioning)
	{

		MS = returnMotionSequenceContainerFromID(status.SeqID).MS;
		return(MS->getValue(_channel, _time));
	}
	//if its choosing the transition's Motion Sequence
	// add another conditional statment on whether the _time variable is at the frame number we want to transition to. AKA what frame number or time do we transition at for the new motion sequence
	else if (status.isTransitioning &&  timeToTransition(_time))
	{
		MS = returnMotionSequenceContainerFromID(status.TransitionToSeqId).MS;

		//set this for currentFrameCalculations
		last_transition_time = _time;
		last_transition_frame = status.FrameNumberTransition;

		//its going to transition, so replace the SeqID and Framenumber of what we are going to transition to
		status.SeqID = status.TransitionToSeqId;
		status.FrameNumber = status.FrameNumberTransition;

		//if there are more transitions to do in the path list
		if (path.size() > 0)
		{
			vertexTargets temp = path.front();
			path.pop_front();
			status.TransitionToSeqId = temp.SeqID;
			status.FrameNumberTransition = temp.FrameNumber;

		}
		//iterate the motion graph to the transitiing motion sequence.
		
		return(MS->getValue(_channel, _time));

	}
	//if it is transitioning but is not at the right time
	else{
	
		MS = returnMotionSequenceContainerFromID(status.SeqID).MS;
		return(MS->getValue(_channel, _time));
	}

}

MotionGraph::DirectedGraph::vertex_descriptor MotionGraphController::FindVertex(string sequenceID, int frameNumber)
{
	pair<MotionGraph::vertex_iter, MotionGraph::vertex_iter> vp;
	int i;
	for (vp = vertices(g.dgraph), i = 0; vp.first != vp.second; ++vp.first, i++) {
		MotionGraph::DirectedGraph::vertex_descriptor v = *(vp.first);
		if (g.dgraph[v].frame_data.fileName == sequenceID)
		{
			if (g.dgraph[v].frame_data.frame_number == frameNumber)
			{
				return(v);
			}
		}
	}
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

void MotionGraphController::readInMotionSequences()
{

	cout << "reading motion Sequences" << endl;

	namespace fs = ::boost::filesystem;

	fs::path p(BVH_MOTION_FILE_PATH2);
	if (!exists(p))    // does p actually exist?
		cout << "doesn't exist" << endl;
	fs::directory_iterator end_itr;

	// cycle through the directory
	for (fs::directory_iterator itr(p); itr != end_itr; ++itr)
	{
		// If it's not a directory, list it. If you want to list directories too, just remove this check.
		if (is_regular_file(itr->path())) {
			// assign current file name to current_file and echo it out to the console.
			string current_file = itr->path().string();
			current_file = itr->path().filename().string();
			cout << current_file << endl;

			DataManager dataman;
			dataman.addFileSearchPath(BVH_MOTION_FILE_PATH2);
			char* BVH_filename = NULL;
			string character_BVH2(current_file);
			try
			{
				BVH_filename = dataman.findFile(character_BVH2.c_str());
				if (BVH_filename == NULL)
				{
					logout << "AnimationControl::loadCharacters: Unable to find character BVH file <" << character_BVH2 << ">. Aborting load." << endl;
					throw BasicException("ABORT");
				}
				pair<Skeleton*, MotionSequence*> read_result;
				try
				{
					read_result = data_manager.readBVH(BVH_filename);
				}
				catch (const DataManagementException& dme)
				{
					logout << "AnimationControl::loadCharacters: Unable to load character data files. Aborting load." << endl;
					logout << "   Failure due to " << dme.msg << endl;
					throw BasicException("ABORT");
				}

				Skeleton* skel = read_result.first;
				MotionSequence * ms = read_result.second;
				//old code
				/*
				MsV.push_back(ms);
				MsV.size();
				MsVNames.push_back(current_file);

					*/

				//test for new structure
				MotionSequenceContainer test;
				test.MS = ms;
				test.SeqID = current_file;
				MsVector.push_back(test);

				
				cout << "done " << MsVector.size() << endl;

			}
			catch (BasicException& e) { cout << e.msg << endl; }
		}
	}
	cout << "the size of the vector is : " << MsVector.size() << endl;
	
}

void MotionGraphController::readAllSequenceIDs()
{
	for (int i = 0; i < MsVector.size(); i++)
	{

		cout << "names " << i << " :" << MsVector.at(i).SeqID << endl;

	}


}

MotionGraphController::MotionSequenceContainer MotionGraphController::returnMotionSequenceContainerFromID(string ID)
{
	for (int i = 0; i < MsVector.size(); i++)
	{
		if (MsVector.at(i).SeqID == ID)
			return MsVector.at(i);
	}

}

void MotionGraphController::readAllFrames()
{
	pair<MotionGraph::vertex_iter, MotionGraph::vertex_iter> vp;
	int jo = 0;
	for (vp = vertices(g.dgraph); vp.first != vp.second; vp.first++)
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
	std::pair<MotionGraph::neighbor_iterator, MotionGraph::neighbor_iterator> neighbors = boost::adjacent_vertices(m, g.dgraph);
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

///OLD CODE
//old code 
/*
int MotionGraphController::findSeqID(string ID)
{
	for (int i = 0; i < MsVNames.size(); i++)
	{
		if (MsVNames.at(i) == ID)
			return i;
	}


}*/