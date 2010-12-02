#ifndef VARCHARWRAPPER_H
#define VARCHARWRAPPER_H

#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

class VarcharWrapper : public DATA::WRAPPER::DatatypeWrapper
{

    /**
      varchar field wrapper class
      tasks:
        - generate hashmap with random values for insertion to generated database
        - answer lookUp requests
    **/

public:
    VarcharWrapper();
    ~VarcharWrapper();

    void setUp();
    QString lookUp(unsigned int);
    QString lookUp();

private:
    void incrementKey();
    QString getRandomValue();
    QChar getRandomCharFromAlphabet();

    /** charset for string output **/
    QMap<unsigned int, QChar> _alphabet;
};

}} // namespaces

#endif // VARCHARWRAPPER_H
