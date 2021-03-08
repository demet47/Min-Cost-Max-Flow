#include "BipartiteGraph.h"
#include<iostream>
#include<stack>
#include<queue>
using namespace std;

typedef pair<int, int>* edge;

BipartiteGraph::BipartiteGraph(int cardinality) {
	this->cardinality = cardinality;
	connexionMatrix = new vector<vector<int>*>();
	for (int i = 0; i < cardinality; i++) {
		connexionMatrix->push_back(new vector<int>(cardinality));
	}
	matchingLtoR = new vector<int>(cardinality);
	matchingRtoL = new vector<int>(cardinality);
}
//then call createGraph, then call eliminateNegativeCycle, then call maxVolt;


bool BipartiteGraph::findNegativeCycle() {
	queue<int> verticeQueue;
	vector<int> dis(cardinality * 2);
	vector<int>* pre = new vector<int>(cardinality * 2);
	vector<int> queueContains(cardinality * 2); // since I don't have auxilary space constraint,
	//I use hashing instead of iterative seeking to check if vertice exists in queue

	for (int i = 0; i < cardinality * 2; i++) {
		dis[i] = 0;
		verticeQueue.push(i);
		queueContains[i] = 0;
		pre->at(i) = -1;
	}

	int iter = 0;
	while (!verticeQueue.empty()) {
		int u = verticeQueue.front();
		queueContains[u] = -1;
		verticeQueue.pop();
		if (u / cardinality == 1) { // if left node
			int edgeTo = matchingRtoL->at(u - cardinality);
			if (dis[u] + connexionMatrix->at(edgeTo)->at(u - cardinality) < dis[edgeTo]) {
				(*pre)[edgeTo] = u;
				dis[edgeTo] = dis[u] + connexionMatrix->at(edgeTo)->at(u - cardinality);
				++iter;
				if (iter == cardinality * 2) {
					iter = 0;
					if (detectCycle(pre)) {
						delete pre;
						return true;
					}
				}
				if (queueContains[edgeTo] == -1) {
					queueContains[edgeTo] = 0;
					verticeQueue.push(edgeTo);
				}
			}
		}
		else {
			for (int j = 0; j < cardinality; j++) {
				if (j == matchingLtoR->at(u)) continue;
				if (dis[u] + connexionMatrix->at(u)->at(j) < dis[j + cardinality]) {
					(*pre)[j+cardinality] = u; 
					dis[j+cardinality] = dis[u] + connexionMatrix->at(u)->at(j);
					++iter;
					if (iter == cardinality * 2) {
						iter = 0;
						if (detectCycle(pre)) {
							delete pre;
							return true;
						}
					}
					if (queueContains[j + cardinality] == -1) {
						queueContains[j + cardinality] = 0;
						verticeQueue.push(j + cardinality);
					}
				}
			}
		}
	}

	if (detectCycle(pre) == true) {
		delete pre;
		return true;
	}
	else {
		delete pre;
		return false;
	}
}


//the vector of cycle contains the connexion node both at the end and in the beggining
vector<int>* BipartiteGraph::trace(vector<int>* pre, int v) {
	stack<int> s;
	vector<int> contains(cardinality * 2 + 2);
	while (contains[v] != 6) {
		s.push(v);
		contains[v] = 6;
		v = pre->at(v);
	}
	vector<int>* cycle = new vector<int>();
	cycle->push_back(v);
	while (s.top() != v) {
		cycle->push_back(s.top());
		s.pop();
	}
	cycle->push_back(v);
	return cycle;
}

void BipartiteGraph::reorientEdges(vector<int>* cycle) {
	if (cycle->at(0) < cardinality) { //if the starting node is left 		
		for (int i = 0; i < cycle->size() - 1; ) {
			int a = cycle->at(i++);
			int b = cycle->at(i++);
			connexionMatrix->at(matchingRtoL->at(b - cardinality))->at(b - cardinality) *= -1;
			matchingLtoR->at(a) = b - cardinality;
			matchingRtoL->at(b - cardinality) = a;
			connexionMatrix->at(a)->at(b - cardinality) *= -1;
		}
	}
	else {
		for (int i = 0; i < cycle->size()-1;) {
			int a = cycle->at(++i);
			int b = cycle->at(++i);
			matchingLtoR->at(a) = b - cardinality;
			matchingRtoL->at(b - cardinality) = a;
			connexionMatrix->at(a)->at(b - cardinality) *= -1;
			connexionMatrix->at(cycle->at(i - 1))->at(cycle->at(i - 2) - cardinality) *= -1;
		}
	}
	delete cycle;
}

bool BipartiteGraph::detectCycle(vector<int>* pre) {
	int* haveBeenHere = new int[cardinality * 2 + 2];
	for (int i = 0; i < cardinality * 2 ; i++)
		haveBeenHere[i] = -1;
	for (int n = 0; n < cardinality * 2 ; n++) {
		if (pre->at(n) == -1)
			continue;
		stack<int> s;
		s.push(pre->at(n));

		while (!s.empty()) {
			int current = s.top();
			s.pop();
			if (haveBeenHere[current] == n) {
				reorientEdges(trace(pre, current));
				delete[] haveBeenHere;
				return true;
			}
			haveBeenHere[current] = n;

			int next = pre->at(current);
			if (next >= 0) {
				s.push(next);
			}
		}
	}
	delete[] haveBeenHere;
	return false;
}


void BipartiteGraph::createGraph() {
	vector<int> isOccupied(cardinality);
	for (int i = 0; i < cardinality; i++) {
		int max = 0;
		int indexMax = 0;
		for (int j = 0; j < cardinality; j++) {
			if (connexionMatrix->at(i)->at(j) > max&& isOccupied[j] != 3) {
				max = connexionMatrix->at(i)->at(j);
				indexMax = j;
			}
			connexionMatrix->at(i)->at(j) *= -1;
		}
		connexionMatrix->at(i)->at(indexMax) = max;
		isOccupied[indexMax] = 3;
		matchingLtoR->at(i) = indexMax;
		matchingRtoL->at(indexMax) = i;
	}


	while (findNegativeCycle() == true) {

	}
}



int BipartiteGraph::maxVoltage() {
	int sum = 0;
	for (int i = 0; i < cardinality; i++) {
		sum += connexionMatrix->at(i)->at(matchingLtoR->at(i));
	}
	return sum;
}


BipartiteGraph::~BipartiteGraph() {
	delete matchingLtoR;
	delete matchingRtoL;
	for (int i = 0; i < cardinality; i++)
		delete connexionMatrix->at(i);
	delete connexionMatrix;
}

