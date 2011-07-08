#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

/** public ******************************************************************************************************/

	DatatypeWrapper& DatatypeWrapper::operator=(const DatatypeWrapper &rhs)
	{
		_range = rhs._range;
		_out = rhs._out;

		return *this;
	}

	DatatypeWrapper::~DatatypeWrapper() {}

	void DatatypeWrapper::populateOutstream(std::ofstream *out)
	{
		_out = out;
	}

	void DatatypeWrapper::setRange(unsigned long long range)
	{
		_range = range;
	}
	
}} // namespaces
