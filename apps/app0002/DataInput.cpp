#include "DataInput.h"

DataInput::DataInput(string fileName)
{
	vector<string>fileNames;
	string temp;
	string FileName;
	cout << endl <<"How many files do you wish to load: ";
	cin>>fileCount;

	// create baseball vector
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing1.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing2.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing3.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing4.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing5.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing6.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing7.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing8.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing9.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing10.bvh");
	baseballFiles.push_back("../../data/motion/BVH/Baseball_Swings/swing11.bvh");

	/*for(int i=0;i<fileCount;i++)
	{
		cout<<endl<<"Please enter the file location of file "<< i <<" to be loaded: ";
		cin>>temp;
		fileNames.push_back(temp);
	}*/
	fileCount=baseballFiles.size();
	cout<<endl<<"Loading in "<< fileCount<<" files"<<endl;
	//for(int i=0;i<fileCount;i++)
	//{
		//readFile(baseballFiles.at(i));
		readFile("../../data/motion/BVH/Baseball_Swings/swing11.bvh");
		//printFrames();
		//WriteFrames();
	//}
}

void DataInput::readFile(string FileName)
{

	ifstream  data(FileName);
	ofstream output2;
	//output1.open(filename1);
	string filename2 = "../../data/motion/BVH/quaternion.bvh";
	output2.open(filename2);
	std::string line;
	int lineNumber = 0;
	int FrameCount = 0;
	cout << FileName << endl;
	lineNumber = 0;

	Frame temp;
	
	// get us to the euler angles, skip rest of file
	while (!data.eof())
	{
		getline(data, line);

		// May need to change but currently 'M' is only
		// the first character of the line in one place of the file.
		// This is two lines prior to euler angles in the document
		if (line[0] == 'M')
		{
			cout << endl << line << endl;
			// line number is used for debugging
		//	lineNumber=lineNumber + 2;
			
			//these two getlines() jump us to the first line of euler angles
			getline(data, line);
			cout << endl << line << endl;
			getline(data, line);
			cout << endl << line << endl;
			//cout << endl << lineNumber << endl;
			break;
		}

		//lineNumber++;
	} 
	while (!data.eof())
	{
		cout << endl << " Frame: " << FrameCount << " created" << endl;
		temp.frameCount = FrameCount;
			//we start grabbing the euler angles here
				//cout << endl << line << endl;
				//Get the position for the root node
				data >> temp.root.PosX;
				data >> temp.root.PosY;
				data >> temp.root.PosZ;
				data >> temp.root.eulerAngle.z;
				data >> temp.root.eulerAngle.x;
				data >> temp.root.eulerAngle.y;
				Quaternion tempQuat;
				tempQuat.fromEuler(temp.root.eulerAngle);
				cout<< endl << tempQuat.w<<" "<<tempQuat.z<<" "<<tempQuat.x<<" "<<tempQuat.y<<endl;
				output2<<temp.root.PosX<<" "<< temp.root.PosY <<" "<< temp.root.PosZ<<" ";
				output2 << tempQuat.w<<" "<<tempQuat.z<<" "<<tempQuat.x<<" "<<tempQuat.y<<" ";
				//system("pause");
				//temp.root.quaternion=tempQuat;
				/*std::cout << std::setprecision(10);
				cout <<endl<< temp.root.PosX << endl;
				cout << endl << temp.root.PosY << endl;
				cout << endl << temp.root.PosZ << endl;
				cout <<endl<< temp.root.quaternion.w << endl;
				cout << endl << temp.root.quaternion.x << endl;
				cout << endl << temp.root.quaternion.y << endl;
				cout << endl << temp.root.quaternion.z << endl;
				system("pause");*/
				//cout << endl << temp.root.eulerAngle.z << endl;
				//cout << endl << temp.root.eulerAngle.x << endl;
				//cout << endl << temp.root.eulerAngle.y << endl;
				//system("Pause");

				// gather non-root joints euler angles in the frame
				for (int j = 0; j < 20; j++)
				{
					Node tempHolder;
						data >> tempHolder.eulerAngle.z;
						data >> tempHolder.eulerAngle.x;
						data >> tempHolder.eulerAngle.y;
						Quaternion tempQuat2;
						tempQuat2.fromEuler(tempHolder.eulerAngle);
						//tempHolder.quaternion=tempQuat;
						output2 << tempQuat2.w<<" "<<tempQuat2.z<<" "<<tempQuat2.x<<" "<<tempQuat2.y<<" ";
						tempHolder.nodeCount = j;
					temp.nodes.push_back(tempHolder);

				}
				output2<<endl;
				
				lineNumber++;
				motion.push_back(temp);

				
				FrameCount++;
				if(lineNumber>2000)
				{
					data.eof();
					output2.eof();
					break;
						
				}
			}
	//}
}

/*
Node DataInput::getNode()
{


}
*/

void DataInput::printFrames()
{
	for (int i = 0; i < motion.size(); i++)
	{
		cout << endl << "frame " << motion.at(i).frameCount<<endl;
		cout<<"****************************************************";
		cout <<" root node "<< endl << " xposition: " << motion.at(i).root.PosX << " yposition: " << motion.at(i).root.PosY << " zposition: " << motion.at(i).root.PosZ << endl;
		cout << " quaternion w value: " << motion.at(i).root.quaternion.w <<endl << " quaternion x value: " << motion.at(i).root.quaternion.x  << endl<< " quaternion y value: " << motion.at(i).root.quaternion.y<< endl<< " quaternion z value: " << motion.at(i).root.quaternion.z <<endl;
		cout<<"****************************************************";
		
		int nodeCount = motion.at(i).nodes.size();
		for(int j=0;j<nodeCount;j++)
		{
			cout<<endl<<"Node "<<j<<endl;
			cout<<"****************************************************";
			cout << " quaternion w value: " << motion.at(i).nodes.at(j).quaternion.w << endl<< " quaternion x value: " << motion.at(i).nodes.at(j).quaternion.x <<endl <<" quaternion y value: " << motion.at(i).nodes.at(j).quaternion.y <<endl << " quaternion z value: " << motion.at(i).nodes.at(j).quaternion.z <<endl;
			cout<<"****************************************************";
		}
	}
}

void DataInput::WriteFrames()
{
	//string filename1 = "../../data/motion/BVH/quaternion.doc";
	string filename2 = "../../data/motion/BVH/quaternion.bvh";
	//ofstream output1;
	ofstream output2;
	//output1.open(filename1);
	output2.open(filename2);
	for (int i = 0; i < motion.size(); i++)
	{
		//output1 << endl << "frame " << motion.at(i).frameCount<<endl;
		//output1<<"****************************************************";
		//output1 <<" root node "<< endl << " xposition: " << motion.at(i).root.PosX << " yposition: " << motion.at(i).root.PosY << " zposition: " << motion.at(i).root.PosZ << endl;
		//output1 << " quaternion w value: " << motion.at(i).root.quaternion.w <<endl << " quaternion x value: " << motion.at(i).root.quaternion.x  << endl<< " quaternion y value: " << motion.at(i).root.quaternion.y<< endl<< " quaternion z value: " << motion.at(i).root.quaternion.z <<endl;
		//output1<<"****************************************************";
		output2<<FILENAME<<" ";
		output2 << motion.at(i).root.PosX << " "<< motion.at(i).root.PosY <<" "<< motion.at(i).root.PosZ;
		//system("pause");
		int nodeCount = motion.at(i).nodes.size();
		for(int j=0;j<nodeCount;j++)
		{
		//	output1<<endl<<"Node "<<j<<endl;
		//	output1<<"****************************************************";
		//	output1<< " quaternion w value: " << motion.at(i).nodes.at(j).quaternion.w << endl<< " quaternion x value: " << motion.at(i).nodes.at(j).quaternion.x <<endl <<" quaternion y value: " << motion.at(i).nodes.at(j).quaternion.y <<endl << " quaternion z value: " << motion.at(i).nodes.at(j).quaternion.z <<endl;
		//	output1<<"****************************************************";
			
			output2 << " "<< motion.at(i).nodes.at(j).quaternion.w  << " "<< motion.at(i).nodes.at(j).quaternion.z << " "<< motion.at(i).nodes.at(j).quaternion.x << " " << motion.at(i).nodes.at(j).quaternion.y;
			
		}
		output2<<endl;
		cout<< endl<<" Wrote Frame " << i<< " to a file "<<endl;
	}
	//output1.close();
	output2.close();
}

void DataInput::setFileName(string FileName)
{
	FILENAME =FileName;
}
/*
RootNode DataInput::getRoot()
{

}
*/
/*
void DataInput::setRoot(double PosX, double PosY, double PosZ, double eulerZ, double eulerY, double eulerX)
{

}
*/