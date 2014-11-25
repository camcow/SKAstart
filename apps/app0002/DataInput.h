#ifndef DATAINPUT_H
#define DATAINPUT_H
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream> 
#include <iomanip> 
#include <Math/Vector3D.h>
#include <Math/Quaternion.h>

using namespace std;

struct RootNode
{
	Quaternion quaternion;
	double PosX, PosY, PosZ;
	Vector3D eulerAngle;
	//Quaternion quaternion(Vector3D eulerAngle);
};

struct Node
{
	Quaternion quaternion;	
	Vector3D eulerAngle;
	// node number in the vector of nodes starts at 1
	int nodeCount;
	//Quaternion quaternion(Vector3D eulerAngle);
};

//contains an entire motion sequence
struct Frame
{
	RootNode root;
	std::vector<Node> nodes;
	// Frame number in the vector of Frames starts at 1
	int frameCount;
};

class DataInput
{	
	private:
		std::vector<Frame> motion;
		vector<string> baseballFiles;
		string FILENAME;
		int fileCount;

	public:
		DataInput(string fileName);
		void readFile(string FileName);
		void WriteFrames();
		//Node getNode();
		void setFileName(string FileName);
		//RootNode getRoot();
		void setRoot(double PosX, double PosY, double PosZ, double eulerZ, double eulerY, double eulerX);
		void printFrames();
};

#endif