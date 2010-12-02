#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "DATA/columnhead.h"
#include "MISC/macros.h"
#include "DATA/WRAPPER/intwrapper.h"
#include "DATA/WRAPPER/varcharwrapper.h"
#include "DATA/tableheader.h"

#include "QString"

#include <fstream>
#include <vector>

namespace FILEHANDLER{

class FileHandler
{

    /**
      abstract base class for file helpers
    **/

public:
    virtual void writeHeader(DATA::TableHeader *, std::ofstream *) = 0;
    virtual void writeRow(std::ofstream *) = 0;
    virtual void finalizeOutput(std::ofstream *) = 0;

    virtual unsigned long long getRowsWritten() = 0;

protected:
    virtual void writeData(std::vector<QString>, std::ofstream *) = 0;
    virtual QString getCellData(unsigned int) = 0;

    /** saves the number of rows written, for progress calculation **/
    unsigned long long _rowsWritten;
    /** saves header information of current table **/
    DATA::TableHeader *_head;

    /** instance of int datatype wrapper class **/
    DATA::WRAPPER::IntWrapper *_iw;
    /** instance of varchar datatype wrapper class **/
    DATA::WRAPPER::VarcharWrapper *_vw;

};

} // namespaces

#endif // FILEHANDLER_H
