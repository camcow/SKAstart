#include "MotionGraph.h"

MotionGraph::MotionGraph()
{	
	vertexCount=0;
	fileLoader();


}

void MotionGraph::graphToFile()
{
}

void MotionGraph::fileReader(string fileName)
{
	ifstream  data(fileName);
	std::string line;
	cout << endl << " Vertex: " << vertexCount << " created" << endl;
	//char name[255];
	//sprintf (name, "vertex%d", vertexCount);
	Vertex temp;
	//vector<vertex_t>vert;

	int i=0;
	while(!data.eof())
	{
		
		getline(data, line);
		temp=add_vertex(g);
		//g[temp].vertexName=name;
		g[temp].vertexCount = vertexCount;
			//we start grabbing quaternions here
				//cout << endl << line << endl;
				//Get the position for the root node
				data >> g[temp].root.PosX;
				data >> g[temp].root.PosY;
				data >> g[temp].root.PosZ;
				data >> g[temp].root.quaternion.w;
				data >> g[temp].root.quaternion.z;
				data >> g[temp].root.quaternion.x;
				data >> g[temp].root.quaternion.y;
				
				// gather non-root joints euler angles in the frame
				for (int j = 0; j < 20; j++)
				{
					//Node tempHolder;
					data >> g[temp].nodes.at(j).quaternion.w;
					data >> g[temp].nodes.at(j).quaternion.z;
					data >> g[temp].nodes.at(j).quaternion.x;
					data >> g[temp].nodes.at(j).quaternion.y;
					

				}
				vertexCount++;
			
			}
}
void MotionGraph::fileLoader()
{
	//To test motion graph builder we will be loading in a vector of filenames related to baseball MoCAP
	vector<string>baseball;
	// vector generator
	for(int i=0;i<2;i++)
	{
		char temp[255];
		sprintf (temp, "../../data/motion/BVH/converted/testQuaternion%d.bvh", i);
		cout << endl << temp << endl;
		baseball.push_back(temp);
	}

	int size = baseball.size();
	fileReader(baseball.at(0));

}