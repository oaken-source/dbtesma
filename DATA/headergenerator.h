#ifndef HEADERGENERATOR_H
#define HEADERGENERATOR_H

#include "DATA/columnhead.h"
#include "DATA/tableheader.h"

#include "QString"

#include <vector>
#include <cstdlib>

namespace DATA {

class HeaderGenerator
{

    /**
      header generation class
      tasks:
        - build header information and assign datatypes for columns
    **/

public:
    HeaderGenerator();
    ~HeaderGenerator();

    void setUp(unsigned int, unsigned int, unsigned int, unsigned int);
    void buildHeader(DATA::TableHeader *);

private:
    ENUM::COLUMN_TYPES getRandomDatatype();

    /** number of columns **/
    unsigned int _cols;
    /** probability of datatypes on columns **/
    unsigned int _probInteger;
    unsigned int _probVarchar;
    /** ratio of functional dependencies **/
    unsigned int _probFD;
};

} //namespaces

#endif // HEADERGENERATOR_H
