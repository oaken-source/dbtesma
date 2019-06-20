/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Grapentin                                    *
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

#include "funcdepgraph.h"

namespace data {

/** public ********************************************************************/

  void FuncdepGraph::push_back(data::Column *c)
  {
    Node n = { c, 0, std::vector<data::Column*>(), false};
    _nodes.push_back( n );
  }

  void FuncdepGraph::push_back(data::Funcdep *fd)
  {
    data::Column *rhs = *(fd->rhs_begin());
    findInNodes(rhs)->_parents += fd->lhs_size();

    std::vector<data::Column*>::iterator i = fd->lhs_begin();
    for(; i != fd->lhs_end(); i++)
    {
      findInNodes(*i)->_children.push_back(rhs);
    }
  }

  data::Column* FuncdepGraph::pop_back()
  {
    std::vector<Node>::iterator i = _nodes.begin();
    for(; i != _nodes.end(); i++)
    {
      if((*i)._parents == 0 && !(*i)._returned)
      {
        std::vector<data::Column*>::iterator j = (*i)._children.begin();
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

/** private *******************************************************************/

  data::FuncdepGraph::Node* FuncdepGraph::findInNodes(data::Column *c)
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
