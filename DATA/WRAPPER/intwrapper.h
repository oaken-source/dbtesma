#ifndef INTWRAPPER_H
#define INTWRAPPER_H

#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

class IntWrapper : public DATA::WRAPPER::DatatypeWrapper
{

    /**
      integer field wrapper class
      tasks:
        - generate hashmap with random values for insertion to generated database
        - answer lookUp requests
    **/

public:
    IntWrapper();
    ~IntWrapper();

    void setUp();
    QString lookUp(unsigned int);
    QString lookUp();

private:
    void incrementKey();
    QString getRandomValue();
};

}} // namespaces

#endif // INTWRAPPER_H
