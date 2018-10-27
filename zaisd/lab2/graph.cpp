#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
// used for std::sort on vector
#include <algorithm>

using namespace std;

const int INFINITY = static_cast<unsigned int>(-1)/2;

void printNodes(map<size_t, int>& nodes)
{
	cout << "Nodes: " << endl;
	for(auto it = nodes.begin(); it != nodes.end(); ++it)
	{
		cout << "[id: " << it->first << ", total weight: " << it->second << "]" << endl;
	}
	cout << endl;
}

/**
	algorithm:
	1. find an increasing path
	2. find the value of the smallest capacity in this path
	3. increase/decrease each path element and total flow
	repeat those steps until increasing path cannot be found
	* backward flow - if there is enough points on the edge we can go backwards and substract from it
*/

struct NeighbourFlow
{
	size_t currentFlow;
	size_t maxFlow;
};

struct PathElement
{
	size_t nodeFrom;
	size_t nodeTo;
	size_t modifier;
	// can be 1 or forward and -1 for backward
	int forward;
};

bool pathContainsNode(vector<PathElement> &path, size_t node)
{
	for(auto it = path.begin(); it != path.end(); ++it)
	{
		if (it->nodeFrom == node || it->nodeTo == node)
		{
			return true;
		}
	}
	return false;
}

bool maxFlowMatrix(vector<vector<NeighbourFlow>>& neighbours)
{
	bool increasingPathFound = false;
	int totalFlow = 0;
	do {
		increasingPathFound = false;
		// 1. find an increasing path
//cout << "checking for increasing path" << endl;
		size_t currNode = 0;
		vector<PathElement> path;
		// go until final node is reached
		while (currNode != neighbours.size() - 1)
		{
//cout << "current node: " << currNode << endl;
			bool nextNodeFound = false;
			// handle forward flow
			for(size_t i=0;i<neighbours[currNode].size();++i)
			{
				NeighbourFlow *forwardDirection = &neighbours[currNode][i];
//cout << forwardDirection->currentFlow << "/" << forwardDirection->maxFlow << " " ;
				if (forwardDirection->maxFlow != 0 && forwardDirection->currentFlow < forwardDirection->maxFlow) 
				{
//cout <<"[forward passed]";
					if (!pathContainsNode(path, i))
					{
						// found next node
						path.push_back({ currNode, i, forwardDirection->maxFlow - forwardDirection->currentFlow, 1 });
						currNode = i;
						nextNodeFound = true;
						break;
					}
				}
			}
			// handle backward flow
			if (nextNodeFound)
			{
				continue;
			}
			for(size_t i=0;i<neighbours.size();++i)
			{
				NeighbourFlow *backwardDirection = &neighbours[i][currNode];
				if (backwardDirection->maxFlow != 0 && backwardDirection->currentFlow > 0)
				{
//cout <<"[backward passed]";
					if (!pathContainsNode(path, i))
					{
						// found next node
						path.push_back({ currNode, i, backwardDirection->currentFlow, -1 });
						currNode = i;
						nextNodeFound = true;
						break;
					}
				}
			}
//cout << endl;
			if (!nextNodeFound)
			{
				// if no next node was found, there is no increasing path
				increasingPathFound = false;
				break;
			}
		}
		// increasing path found if we end up searching for it in the final node
		if (currNode == neighbours.size() - 1)
		{
			increasingPathFound = true;
			// 2. find the value of the smallest capacity in this path
//cout << "found increasing path, calculating smallest capacity" << endl;
			size_t smallestCapacity = INFINITY;
			for(auto it = path.begin(); it != path.end(); ++it)
			{
//cout << "  " << it->nodeFrom << " -> " << it->nodeTo << " [" << (static_cast<int>(it->modifier)*it->forward) << "]" << endl;
				smallestCapacity = min(smallestCapacity, it->modifier);
			}
			if (smallestCapacity == INFINITY)
			{
//cout << "error: smallest capacity not found!" << endl;
				return false;
			}
//cout << "calculated smallest capacity: " << smallestCapacity << endl;
			// 3. increase/decrease each path element and total flow
			for(auto it = path.begin(); it != path.end(); ++it)
			{
//cout << "    update " << it->nodeFrom << " -> " << it->nodeTo << "(" << neighbours[it->nodeFrom][it->nodeTo].currentFlow << ")" << " by " << (static_cast<int>(smallestCapacity) * it->forward) << " => ";
				if (it->forward == 1)
				{
					neighbours[it->nodeFrom][it->nodeTo].currentFlow += (static_cast<int>(smallestCapacity) * it->forward);
				}
				else
				{
					neighbours[it->nodeTo][it->nodeFrom].currentFlow += (static_cast<int>(smallestCapacity) * it->forward);
				}
//cout  << neighbours[it->nodeFrom][it->nodeTo].currentFlow << endl;
			}
			totalFlow += smallestCapacity;
//cout << "total flow updated to: " << totalFlow << endl;
		}
	} while(increasingPathFound);
	cout << "total flow is: " << totalFlow << endl;
	return true;
}




// for list
struct Edge
{
	size_t from;
	size_t to;
	size_t currentFlow;
	size_t maxFlow;
};

bool maxFlowList(vector<Edge>& edges, const size_t start, const size_t end)
{
	cout << "enter algorithm with start node=" << start << ", end node=" << end << endl;
	bool increasingPathFound = false;
	int totalFlow = 0;
	do {
		increasingPathFound = false;
		// 1. find an increasing path
//cout << "checking for increasing path" << endl;
		size_t currNode = start;
		vector<PathElement> path;
		// go until final node is reached
		while (currNode != end)
		{
//cout << "current node: " << currNode << endl;
			bool nextNodeFound = false;
			// handle forward flow
			for (auto it = edges.begin(); it != edges.end(); ++it)
			{
//cout << it->from << "->" << it->to << " | " ;
				if (it->from == currNode && it->currentFlow < it->maxFlow)
				{
					if (!pathContainsNode(path, it->to))
					{
//cout << " passed forward";
						path.push_back({ it->from, it->to, it->maxFlow - it->currentFlow, 1 });
						currNode = it->to;
						nextNodeFound = true;
						break;
					}
				}
			}
//cout << endl;
			// handle backward flow
			if (nextNodeFound)
			{
				continue;
			}
			for (auto it = edges.begin(); it != edges.end(); ++it)
			{
//cout << it->from << "->" << it->to << " | " ;
				if (it->to == currNode)
				{
					if (it->currentFlow > 0)
					{
						if (!pathContainsNode(path, it->from))
						{
//cout << " passed backward "<<it->currentFlow << "/" << it->maxFlow ;
							path.push_back({ it->to, it->from, it->currentFlow, -1 });
							currNode = it->from;
							nextNodeFound = true;
							break;
						}
					}
				}
			}
//cout << endl;
			if (!nextNodeFound)
			{
				// if no next node was found, there is no increasing path
				increasingPathFound = false;
				break;
			}
		}
		// increasing path found if we end up searching for it in the final node
		if (currNode == end)
		{
			increasingPathFound = true;
			// 2. find the value of the smallest capacity in this path
//cout << "found increasing path, calculating smallest capacity" << endl;
			size_t smallestCapacity = INFINITY;
			for(auto it = path.begin(); it != path.end(); ++it)
			{
//cout << "  " << it->nodeFrom << " -> " << it->nodeTo << " [" << it->modifier << "] {"<< it->forward <<"}" << endl;
				smallestCapacity = min(smallestCapacity, it->modifier);
			}
			if (smallestCapacity == INFINITY)
			{
//cout << "error: smallest capacity not found!" << endl;
				return false;
			}
//cout << "calculated smallest capacity: " << smallestCapacity << endl;
			// 3. increase/decrease each path element and total flow
			Edge *edge = nullptr;
			for(auto it = path.begin(); it != path.end(); ++it)
			{
				if (it->forward == 1)
				{
					for(size_t i = 0; i < edges.size(); ++i)
					{
						if(edges.at(i).from == it->nodeFrom && edges.at(i).to == it->nodeTo)
						{
							edge = &edges.at(i);
						}
					}
					if (edge == nullptr)
					{
						cout << "error could not find edge from " << it->nodeFrom << " to " << it->nodeTo << endl;
						return false;
					}
//cout << "    update " << it->nodeFrom << " -> " << it->nodeTo << "(" << edge->currentFlow << ")" << " by " << (static_cast<int>(smallestCapacity) * it->forward) << " => ";
					//neighbours[it->nodeFrom][it->nodeTo].currentFlow += (static_cast<int>(smallestCapacity) * it->forward);
					edge->currentFlow += (static_cast<int>(smallestCapacity) * it->forward);
				}
				else
				{
					
					for(size_t i = 0; i < edges.size(); ++i)
					{
						if(edges.at(i).to == it->nodeFrom && edges.at(i).from == it->nodeTo)
						{
							edge = &edges.at(i);
						}
					}
					if (edge == nullptr)
					{
						cout << "error could not find edge from " << it->nodeFrom << " to " << it->nodeTo << endl;
						return false;
					}
//cout << "    update " << it->nodeFrom << " -> " << it->nodeTo << "(" << edge->currentFlow << ")" << " by " << (static_cast<int>(smallestCapacity) * it->forward) << " => ";
					//neighbours[it->nodeFrom][it->nodeTo].currentFlow += (static_cast<int>(smallestCapacity) * it->forward);
					edge->currentFlow += (static_cast<int>(smallestCapacity) * it->forward);
				}
//cout  << edge->currentFlow << endl;
			}
			totalFlow += smallestCapacity;
//cout << "total flow updated to: " << totalFlow << endl;
		}
	} while(increasingPathFound);
	cout << "total flow is: " << totalFlow << endl;
	return true;
}






int main(int argc, char **argv)
{
	// parsing input file
	if (argc != 3)
	{
		cout << "error, expected: input file and algorithm(m-matrix, l-list)\n";
		return 1;
	}
	// init and check file
	std::ifstream infile(argv[1]);
	if (!infile)
	{
		cout << "error, file " << argv[1] << " not found\n";
		return 1;
	}
	// check algorithm
	const char algorithm = argv[2][0];
	if (algorithm != 'm' && algorithm != 'l')
	{
		cout << "error, wrong algorithm(must be 'm' for matrix or 'l' for list)\n";
		return 1;
	}
	bool result = false;
	if (algorithm == 'm')
	{
		vector<vector<NeighbourFlow>> weights;
		size_t size = 0;
		infile >> size;
		cout << "read matrix of size " << size << endl;
		weights.resize(size);
		for (size_t i = 0; i < size; ++i)
		{
			weights[i].resize(size);
		}
		// parsing file
		for (size_t i = 0; i < size; ++i)
		{
			for (size_t j = 0; j < size; ++j)
			{
				infile >> weights[i][j].maxFlow;
				weights[i][j].currentFlow = 0;
			}
		}
		// print matrix
		for (size_t i = 0; i < size; ++i)
		{
			for (size_t j = 0; j < size; ++j)
			{
				cout << weights[i][j].currentFlow << "/" << weights[i][j].maxFlow << " ";
			}
			cout << endl;
		}
		result = maxFlowMatrix(weights);
	}
	else if (algorithm == 'l')
	{
		vector<Edge> edges;
		// parsing file
		do
		{
			Edge edge;
			edges.push_back(edge);
		} while(infile >> edges.back().from && infile >> edges.back().to && infile >> edges.back().maxFlow);
		edges.pop_back();
		sort(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2) {
			return e1.from < e2.from;
		});
		cout << "edges:" << endl;
		size_t start = INFINITY;
		size_t end = INFINITY;
		for (size_t i = 0; i < edges.size(); ++i)
		{
			edges.at(i).currentFlow = 0;
			cout << edges.at(i).from << " " << edges.at(i).to << " " << edges.at(i).currentFlow << "/" << edges.at(i).maxFlow << endl;
			start = min(start, edges.at(i).from);
			start = min(start, edges.at(i).to);
			if (end == INFINITY)
			{
				end = edges.at(i).from;
			}
			end = max(end, edges.at(i).from);
			end = max(end, edges.at(i).to);
		}
		if(start != INFINITY || end != INFINITY)
		{
			result = maxFlowList(edges, start, end);
		}
	}
	if (!result)
	{
		cout << "error, algorithm failed\n";
        return 1;
	}
	return 0;
}
