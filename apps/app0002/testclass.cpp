#include "testclass.h";

testclass::testclass(MotionGraphController* mgc)
{
	/*
	MotionSequence *MS;
	MotionSequence *MS2;
	MotionSequence *MS3;
	MotionSequence *MS4;
	
	//temp vertex target list

	MotionGraphController::vertexTargets temp;
	MS = mgc->returnMotionSequenceContainerFromID("swing5.bvh").MS;
	temp.SeqID = "swing5.bvh";
	temp.SeqID2 = "swing6.bvh";
	temp.FrameNumber = MS->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);
	//second transition
	MS2 = mgc->returnMotionSequenceContainerFromID("swing6.bvh").MS;
	temp.SeqID = "swing6.bvh";
	temp.SeqID2 = "swing5.bvh";
	temp.FrameNumber = MS2->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);

	MS3 = mgc->returnMotionSequenceContainerFromID("swing5.bvh").MS;
	temp.SeqID = "swing5.bvh";
	temp.SeqID2 = "swing7.bvh";
	temp.FrameNumber = MS3->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);

	MS4 = mgc->returnMotionSequenceContainerFromID("swing7.bvh").MS;
	temp.SeqID = "swing7.bvh";
	temp.SeqID2 = "swing8.bvh";
	temp.FrameNumber = MS4->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);

	MS4 = mgc->returnMotionSequenceContainerFromID("swing8.bvh").MS;
	temp.SeqID = "swing8.bvh";
	temp.SeqID2 = "swing9.bvh";
	temp.FrameNumber = MS4->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);

	MS4 = mgc->returnMotionSequenceContainerFromID("swing9.bvh").MS;
	temp.SeqID = "swing9.bvh";
	temp.SeqID2 = "swing10.bvh";
	temp.FrameNumber = MS4->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);

	MS4 = mgc->returnMotionSequenceContainerFromID("swing10.bvh").MS;
	temp.SeqID = "swing10.bvh";
	temp.SeqID2 = "swing11.bvh";
	temp.FrameNumber = MS4->numFrames();
	temp.FrameNumber2 = 0;
	path.push_back(temp);



	mgc->setPath(StartSeq, startFrame, path);
	cout << "should start playing" << endl;*/



	MotionSequence *MS;
	MotionSequence *MS2;
	MotionSequence *MS3;
	MotionSequence *MS4;

	//temp vertex target list

	MotionGraphController::vertexTargets temp;
	MS = mgc->returnMotionSequenceContainerFromID("swing5.bvh").MS;
	temp.SeqID = "swing5.bvh";
	temp.SeqID2 = "swing7.bvh";
	temp.FrameNumber = 170;// MS->numFrames();
	temp.FrameNumber2 = 34;// 0;
	path.push_back(temp);

	MS = mgc->returnMotionSequenceContainerFromID("swing7.bvh").MS;
	temp.SeqID = "swing7.bvh";
	temp.SeqID2 = "swing5.bvh";
	temp.FrameNumber =  MS->numFrames();
	temp.FrameNumber2 = 34;// 0;
	path.push_back(temp);


	//second transition
	//to continue playing until the end of the file
	/*
	MS = mgc->returnMotionSequenceContainerFromID("swing7.bvh").MS;
	temp.SeqID = "swing7.bvh";
	temp.SeqID2 = "swing5.bvh";
	temp.FrameNumber =  MS->numFrames();
	temp.FrameNumber2 = 0;// 0;
	path.push_back(temp);
	*/
	mgc->setPath(StartSeq, startFrame, path);
	cout << "should start playing" << endl;
}