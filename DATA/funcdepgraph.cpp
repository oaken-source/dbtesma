
#include "funcdepgraph.h"

namespace DATA {

/** public ******************************************************************************************************/

	void FuncdepGraph::push_back(DATA::Column *c)
	{
		_nodes.push_back( Node { c, 0, std::vector<DATA::Column*>(), false } );
	}
	
	void FuncdepGraph::push_back(DATA::Funcdep *fd)
	{
		DATA::Column *rhs = *(fd->rhs_begin());
		findInNodes(rhs)->_parents += fd->lhs_size();
	
		std::vector<DATA::Column*>::iterator i = fd->lhs_begin();
		for(; i != fd->lhs_end(); i++)
		{
			findInNodes(*i)->_children.push_back(rhs);
		}
	}

	DATA::Column* FuncdepGraph::pop_back()
	{
		std::vector<Node>::iterator i = _nodes.begin();
		for(; i != _nodes.end(); i++)
		{
			if((*i)._parents == 0 && !(*i)._returned)
			{
				std::vector<DATA::Column*>::iterator j = (*i)._children.begin();
				for(; j != (*i)._children.end(); j++)
					findInNodes(*j)->_parents--;

				(*i)._returned = true;
				return (*i)._column;
			}
		}
		return 0;
	}

	void FuncdepGraph::clear()
	{
		_nodes.clear();
	}
	
/** private *****************************************************************************************************/

	DATA::FuncdepGraph::Node* FuncdepGraph::findInNodes(DATA::Column *c)
	{
		std::vector<Node>::iterator i = _nodes.begin();
		for(; i != _nodes.end(); i++)
		{
			if((*i)._column == c)
				return &(*i);
		}
		return 0;
	}	

} // namespaces
