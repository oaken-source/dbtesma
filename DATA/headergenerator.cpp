#include "headergenerator.h"

namespace DATA {

/** public ******************************************************************************************************/

    HeaderGenerator::HeaderGenerator()
    {
        /** standard constructor **/
    }

    HeaderGenerator::~HeaderGenerator()
    {
        /** standard destructor **/
    }

    void HeaderGenerator::setUp(unsigned int cols, unsigned int probInteger, unsigned int probVarchar, unsigned int probFD)
    {
        /** preparation method - sets required metrics information
            params:
              - cols: number of columns
              - probInteger: probability of column becoming int
              - probVarchar: probability of column becoming varchar
              - probFD: probability of column getting funcitonal dependency
        **/
        _cols = cols;
        _probInteger = probInteger;
        _probVarchar = probVarchar;
        _probFD = probFD;
    }

    void HeaderGenerator::buildHeader(DATA::TableHeader* head)
    {
        /** builds header - interpretation is done by file handler classes
            params:
              - head: header information
        **/
        for(unsigned int i=0;i<_cols;i++)
        {
            QString colName = "column_";
            colName.append(QString::number(i));
            head->addColumn(DATA::ColumnHead(getRandomDatatype(), colName));
        }
        head->addPrimaryKey();
        head->applyFunctionalDependencies(_probFD);
    }

/** private *****************************************************************************************************/

    ENUM::COLUMN_TYPES HeaderGenerator::getRandomDatatype()
    {
        /** returns random supported datatype **/
        unsigned int x = rand()%100;
        if(x <= _probInteger)
            return ENUM::INT;
        else
            return ENUM::VARCHAR;
    }

} // namespaces
