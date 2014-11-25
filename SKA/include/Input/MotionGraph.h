#include <Input/DataInput.h>
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"

class MotionGraph
{	
	private:
	DataInput temp;

	

	public:
	MotionGraph();
	void graphToFile();
	void fileReader();

};