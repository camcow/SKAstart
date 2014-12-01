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
	status.FrameNumber = 0;
	status.SeqID = "swing5.bvh";
	status.isTransitioning = true;
	status.TransitionToSeqId = "swing5.bvh";
	MotionSequence *MS;
	MS = returnMotionSequenceContainerFromID(status.SeqID).MS;
	status.FrameNumberTransition = MS->numFrames();
	status.FrameNumberTransitionTo = 0;
	//temp vertex target list

	MotionGraphController::vertexTargets temp;
	temp.SeqID = "swing5.bvh";
	temp.SeqID2 = "swing6.bvh";
	temp.FrameNumber = MS->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);
	//second transition
	temp.SeqID = "swing6.bvh";
	temp.SeqID2 = "swing5.bvh";
	temp.FrameNumber = MS->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);
	printStatus();
	cout << "update status" << endl;
	updateStatus();

	cout << "TIME QUESTIONS" << endl;
	cout << "max frames " << MS->numFrames() << endl;
	cout << "max frames " << MS->numFrames()/120 << endl;
	cout <<"time1 0.011: "<< computeCurrentFrame(0.011) << endl;
	cout << "time2 0.011: " << computeMotionSequenceFrame(MS,0.011) << endl;
	cout << "time3 3.011: " << computeCurrentFrame( 3.637000) << endl;
	cout << "time3 3.011: " << computeMotionSequenceFrame(MS, 3.637000) << endl;
	last_transition_time = 3.54799962;
	last_transition_frame = 0;
	cout << "last_transition_time : " << last_transition_time<<endl;// system time when the last transition was taken
	cout << "last_transition_frame : " << last_transition_frame << endl;
	cout << "frame test " << computeCurrentFrame(3.54799962) << endl;
	cout << "frame test real: " << computeMotionSequenceFrame(MS, 3.54799962) << endl;
	last_transition_time = 0;
	last_transition_frame = 0;
	cout << endl;
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
	if (currentFrame >= status.FrameNumberTransition)
		return(true);
	else
		return(false);
}

int MotionGraphController::computeMotionSequenceFrame(MotionSequence *motion_sequence, float _time)
{
	float duration = motion_sequence->getDuration();
	long cycles = long(_time / duration);

	float sequence_time = _time - duration*cycles;
	if (sequence_time > duration) sequence_time = 0.0f;

	int frame = int(motion_sequence->numFrames()*sequence_time / duration);
	return(frame);
}

long MotionGraphController::computeCurrentFrame(float _time)
{
	//determine how long we have been playing the current motion
	float time_in_sequence = _time - last_transition_time;
	//determine how many frames have been played in the current motion
	long frames_in_sequence = time_in_sequence * frame_rate;
	//offset frames played by the start frame in the current motion
	long current_frame = frames_in_sequence + last_transition_frame;
	//cout << "last_transition_time " << last_transition_time << endl;
	//cout << "last_transition_frame " << last_transition_frame << endl;
	//cout << "COMPUTE CURRENT FRAME" << current_frame << endl;
	return(current_frame);
}
bool MotionGraphController::isValidChannel(CHANNEL_ID _channel, float _time)
{
	MotionSequence *motion_sequence = returnMotionSequenceContainerFromID(status.SeqID).MS;
	if (motion_sequence == NULL)
	{
		throw AnimationException("MotionGraphController has no attached MotionSequence");
		return false;
	}
	return motion_sequence->isValidChannel(_channel);
}
void MotionGraphController::updateStatus()
{
	//transitions to follow
	if (path.size() > 0)
	{
		status.isTransitioning = true;
		// take the first one off the list;
		vertexTargets temp = path.front();
		path.pop_front();
		//first seq
		status.SeqID = temp.SeqID;
		//first seq framenumber where we transition 
		status.FrameNumberTransition = temp.FrameNumber;
		//what we are transitioning to
		status.TransitionToSeqId = temp.SeqID2;
	//what frame we are transitioning to on the second seq;
		status.FrameNumberTransitionTo = temp.FrameNumber2;
	}
	// none left so repeat
	else
	{
		status.isTransitioning = true;
		// the last transition is now the new seqId aka what is playing
		status.SeqID = status.TransitionToSeqId;
		MotionSequence *motion_sequence = returnMotionSequenceContainerFromID(status.SeqID).MS;
		//where to transition on the last frame
		status.FrameNumberTransition = motion_sequence->numFrames();
		// set transition to to fame 0 of the same motion sequence
		status.FrameNumberTransitionTo = 0;
	}
	//iterate the motio

//	printStatus();



}

// all broken
float MotionGraphController::getValue(CHANNEL_ID _channel, float _time){
	
	//if we are not transitioning at any time in the future
	MotionSequence *MS;
	if (!status.isTransitioning)
	{

		MotionSequence *motion_sequence = returnMotionSequenceContainerFromID(status.SeqID).MS;
		if (motion_sequence == NULL)
			throw AnimationException("MotionSequenceController has no attached MotionSequence");

		if (!isValidChannel(_channel, _time))
		{
			string s = string("MotionSequenceController received request for invalid channel ")
				+ " bone: " + toString(_channel.bone_id) + " dof: " + toString(_channel.channel_type);
			throw AnimationException(s.c_str());
		}

		float duration = motion_sequence->getDuration();
		long cycles = long(_time / duration);

			float	sequence_time = _time - duration*cycles;
		if (sequence_time > duration) sequence_time = 0.0f;
		int frame = int(motion_sequence->numFrames()*sequence_time / duration);
		int frame2 =computeMotionSequenceFrame(motion_sequence,_time);
		

		// after repeating it doesn't work with my time.
		//cout << "TIME: " << _time << endl;
		//cout<<"my test " << computeCurrentFrame(_time) << endl;
		//cout << "working " << frame << endl;
		//cout << "working2 " << frame2 << endl;

		float value = motion_sequence->getValue(_channel, frame);
		//float value = motion_sequence->getValue(_channel, computeCurrentFrame(_time));

		//update status
		status.FrameNumber = frame2;

		return(value);
	
	}
	//if its choosing the transition's Motion Sequence
	// add another conditional statment on whether the _time variable is at the frame number we want to transition to. AKA what frame number or time do we transition at for the new motion sequence
	else if (status.isTransitioning &&  timeToTransition(_time))
	{

		MotionSequence *motion_sequence = returnMotionSequenceContainerFromID(status.TransitionToSeqId).MS;
		if (motion_sequence == NULL)
			throw AnimationException("MotionSequenceController has no attached MotionSequence");

		if (!isValidChannel(_channel, _time))
		{
			string s = string("MotionSequenceController received request for invalid channel ")
				+ " bone: " + toString(_channel.bone_id) + " dof: " + toString(_channel.channel_type);
			throw AnimationException(s.c_str());
		}


		//set this for currentFrameCalculations
		last_transition_time = _time;
		last_transition_frame = status.FrameNumberTransitionTo;
		// set the frame number to the frame we transition to.
		status.FrameNumber = status.FrameNumberTransitionTo;
		//update the status
		updateStatus();

	
		// after repeating it doesn't work with my time.

		float duration = motion_sequence->getDuration();
		long cycles = long(_time / duration);

		float	sequence_time = _time - duration*cycles;
		if (sequence_time > duration) sequence_time = 0.0f;
		int frame = int(motion_sequence->numFrames()*sequence_time / duration);
		int frame3 = computeCurrentFrame(_time);
		//cout << "TIME: " << _time << endl;
		//cout << "last_transition_time " << last_transition_time << endl; 
		//cout << "last_transition_frame" << endl; 
		//cout << "my test " << computeCurrentFrame(_time) << endl;
		//cout << "working " << frame << endl;
		
		return(motion_sequence->getValue(_channel, computeCurrentFrame(_time)));

	}
	//if it is transitioning but is not at the right time
	else{
		MotionSequence *motion_sequence = returnMotionSequenceContainerFromID(status.SeqID).MS;
		if (motion_sequence == NULL)
			throw AnimationException("MotionSequenceController has no attached MotionSequence");

		if (!isValidChannel(_channel, _time))
		{
			string s = string("MotionSequenceController received request for invalid channel ")
				+ " bone: " + toString(_channel.bone_id) + " dof: " + toString(_channel.channel_type);
			throw AnimationException(s.c_str());
		}

		float duration = motion_sequence->getDuration();
		long cycles = long(_time / duration);

		float	sequence_time = _time - duration*cycles;
		if (sequence_time > duration) sequence_time = 0.0f;
		int frame = int(motion_sequence->numFrames()*sequence_time / duration);
		int frame2 = computeMotionSequenceFrame(motion_sequence, _time);
		int frame3 = computeCurrentFrame(_time);

		// after repeating it doesn't work with my time.
		//cout << "TIME: " << _time << endl;
		//cout << "my test " << computeCurrentFrame(_time) << endl;
		//cout << "working " << frame << endl;
		//cout << "working2 " << frame2 << endl;
		
		//float value = motion_sequence->getValue(_channel, frame);
		float value = motion_sequence->getValue(_channel, computeCurrentFrame(_time));
		//cout << value << endl;
		//update status
		status.FrameNumber = frame3;
		if (testTime != _time)
	{
		//cout << "time change" << endl;
		testTime = _time;
	}
	

		return(value);
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

void MotionGraphController::printStatus()
{

	cout << "STATUS VARIABLE" << endl;
	cout << "SeqID: " << status.SeqID << endl;
	cout << "FrameNumber: " << status.FrameNumber << endl;
	cout << "FrameNumberTransition: " << status.FrameNumberTransition << endl;
	//cout << "FramelengthSeq1: " << status.FramelengthSeq1 << endl;
	cout << "TransitionToSeqId: " << status.TransitionToSeqId << endl;
	cout << "FrameNumberTransitionTo: " << status.FrameNumberTransitionTo << endl;
	cout << "isTransitioning: " << status.isTransitioning << endl;
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