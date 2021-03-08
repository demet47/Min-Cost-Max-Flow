#include <iostream>
#include<fstream>
#include "BipartiteGraph.h"
using namespace std;
int main(int arg, char* argv[])
{
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	int numOfCases;
	input >> numOfCases;
	while (numOfCases--) {
		int cardinality;
		input >> cardinality;
		BipartiteGraph graph(cardinality);
		for (int i = 0; i < cardinality; i++) {
			for (int j = 0; j < cardinality; j++) {
				int num;
				input >> num;
				graph.connexionMatrix->at(i)->at(j) = num;
			}
		}

		graph.createGraph();
		output << graph.maxVoltage() << endl;
	}
	return 0;
}