#ifndef BIPARTITEGRAPH_H
#define BIPARTITEGRAPH_H
#include<vector>
#include<utility>
using namespace std;


class BipartiteGraph {
public:
	BipartiteGraph(int cardinality);
	vector<vector<int>*>* connexionMatrix;
	vector<int>* matchingLtoR;
	vector<int>* matchingRtoL;
	int maxVoltage();
	void createGraph();
	bool findNegativeCycle();
	~BipartiteGraph();
	bool detectCycle(vector<int>* pre);
	vector<vector<pair<int, int>*>*>* edgesFromIndex;
	vector<int>* trace(vector<int>* pre, int v);
private:
	int cardinality;
	vector<pair<int, int>> currentEdges;
	void reorientEdges(vector<int>* cycle);
};

#endif