#ifndef FUNCDEPGRAPH_H
#define FUNCDEPGRAPH_H

#include "funcdep.h"

namespace DATA {

class FuncdepGraph
{

	/**
	FuncdepGraph Data Class
	tasks:
		do topological sort! and do it good!
	**/
	
	struct Node
	{
		DATA::Column* _column;
		unsigned int _parents;
		std::vector<DATA::Column*> _children;
		bool _returned;
	} typedef Node;

public:

	FuncdepGraph() : _nodes(std::vector<Node>()) {};
	~FuncdepGraph() {};

	/** add node to graph **/
	void push_back(DATA::Column*);

	/** add edges to graph **/
	void push_back(DATA::Funcdep*);
	
	/** return next element without parents **/
	DATA::Column* pop_back();
	
	void clear();
	
private:

	Node* findInNodes(DATA::Column*);

	std::vector<Node> _nodes;

};

} // namespaces

#endif // FUNCDEPGRAPH_H
