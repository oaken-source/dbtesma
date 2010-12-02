#ifndef DATATYPEWRAPPER_H
#define DATATYPEWRAPPER_H

#include "MISC/macros.h"

#include "QHash"
#include "QMap"
#include "QString"
#include "QChar"

#include <cstdlib>
#include <math.h>

namespace DATA { namespace WRAPPER {

class DatatypeWrapper
{

    /**
      abstract parent class for datatype wrappers
    **/

public:
    virtual void setUp() = 0;
    virtual QString lookUp(unsigned int) = 0;
    virtual QString lookUp() = 0;

    unsigned int getLastLookUpKey();

protected:
    virtual void incrementKey() = 0;
    virtual QString getRandomValue() = 0;

    /** access counter **/
    unsigned int _key;
    /** last lookUp reference **/
    unsigned int _lastKey;
    /** hash of precalculated values **/
    QHash<unsigned int, QString> _hash;
};

}} // namespaces

#endif // DATATYPEWRAPPER_H
