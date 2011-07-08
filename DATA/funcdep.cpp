
#include "funcdep.h"

namespace DATA {

/** public ******************************************************************************************************/

	std::vector<DATA::Column*>::iterator Funcdep::lhs_find(DATA::Column *c)
	{	
		std::vector<DATA::Column*>::iterator i = _lhsColumns.begin();
		for(; i != _lhsColumns.end(); i++)
		{
			if((*i) == c)
				return i;
		}
		return _lhsColumns.end();
		
	}

/** private *****************************************************************************************************/

} // namespaces
