#include "Connector.h"


std::vector<vector<int>> Connector(std::vector<MotionGraph::Frame> motion1, std::vector<MotionGraph::Frame> motion2)
{
	float comparisonMax = 0.05;  //Comparison value to be the max difference between two frame in the two motions
	float comparison;
	std::vector<node> unsortedComparisons; //Holds nodes of frame pairs with their comparison value
	for (int i = 0; i < motion1.size(); i += 2) //Loop through each frame of first passed motion
	{
		cout << "Looking at frame " << i << " of the first motion\n";
		for (int j = motion1.size(); j < motion2.size(); j += 2) //Loop through each frame of second passed motion
		{
			//if (j % 50 == 0)
				//cout << "Frame " << j << " of motion 2\n";
			comparison = 0;
			node inputNode;
			for (int k = 0; k < 20/*motion1[i].joints.size()*/; k++) //Loop through each quaternion in frame 'i' of first passed motion
			{
				//cout << motion1[i].joints.size() << " " << motion2[i].joints.size() << endl;
				//for (int l = 0; l < motion2[j].joints.size(); l++) //Loop through each quaternion in frame 'j' of second passed motion
				//{
				//create a comparison number for every frame from all X, Y, Z values from joints
				//cout << comparison << endl;
				comparison = comparison + ((motion1[i].joints[k].x - motion2[j].joints[k].x));
				//cout << comparison << endl;
				comparison = comparison + ((motion1[i].joints[k].y - motion2[j].joints[k].y));
				//cout << comparison << endl;
				comparison = comparison + ((motion1[i].joints[k].z - motion2[j].joints[k].z));
				//cout << comparison << endl;
				//cout << motion1[i].joints[k].x << " " << motion1[i].joints[k].y << " " << motion1[i].joints[k].z << endl;
				//cout << motion2[j].joints[k].x << " " << motion2[j].joints[k].y << " " << motion2[j].joints[k].z << endl;
					
				//}

			}
			cout << comparison << endl;
			//If the comparison is less than the comparisonMax value, store the transition frames and comparison value
			if (abs(comparison) < comparisonMax)
			{
				inputNode.motion1FrameNum = i;
				inputNode.motion2FrameNum = j;
				inputNode.comparisonVal = comparison;
				unsortedComparisons.push_back(inputNode);
			}

		}
	}
	//sorting algorithm to make the list smallest to largest comparison values
	vector<node> sortedComparisons;
	while (unsortedComparisons.size() > 0)
	{
		int lowestCompValLocation = 0;
		for (int i = 1; i < unsortedComparisons.size(); i++)
		{
			if (unsortedComparisons[lowestCompValLocation].comparisonVal > unsortedComparisons[i].comparisonVal)
				lowestCompValLocation = i;
		}
		sortedComparisons.push_back(unsortedComparisons[lowestCompValLocation]);
		unsortedComparisons.erase(unsortedComparisons.begin() + lowestCompValLocation);
	}

	//separate if too close together from other already stored transition nodes
	vector<node> finalTransitionList;
	finalTransitionList.push_back(sortedComparisons[0]);
	sortedComparisons.erase(sortedComparisons.begin());
	while (sortedComparisons.size() > 0)
	{
		bool addToList = true;
		for (int i = 0; i < finalTransitionList.size(); i++)
		{
			//If the lowest comparison value in sortedComparisons frame 1 and 2 are within 10 frames of any motion already stored then don't add to final list
			if (sortedComparisons[0].motion1FrameNum < finalTransitionList[i].motion1FrameNum + 5 &&
				sortedComparisons[0].motion1FrameNum > finalTransitionList[i].motion1FrameNum - 5 &&
				sortedComparisons[0].motion2FrameNum < finalTransitionList[i].motion2FrameNum + 5 &&
				sortedComparisons[0].motion2FrameNum < finalTransitionList[i].motion2FrameNum - 5)
				addToList = false;
		}
		if (addToList)
			finalTransitionList.push_back(sortedComparisons[0]);
		sortedComparisons.erase(sortedComparisons.begin());
	}

	//return final transition list
	vector<vector<int>> returnList;
	for (int i = 0; i < finalTransitionList.size(); i++)
	{
		returnList.at(i).push_back(finalTransitionList[i].motion1FrameNum);
		returnList.at(i).push_back(finalTransitionList[i].motion2FrameNum);
	}
	for (int i = 0; i < returnList.size(); i++)
	{
		cout << endl << "(" << returnList.at(i).at(0) << "," << returnList.at(i).at(1) << ")" << endl;
	}

	return returnList;
}