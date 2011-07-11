/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Henning                                      *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of the GNU General Public License as published by     *
 *    the Free Software Foundation, either version 3 of the License, or        *
 *    (at your option) any later version.                                      *
 *                                                                             *
 *    This program is distributed in the hope that it will be useful,          *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *    GNU General Public License for more details.                             *
 *                                                                             *
 *    You should have received a copy of the GNU General Public License        *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ******************************************************************************/
 
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
