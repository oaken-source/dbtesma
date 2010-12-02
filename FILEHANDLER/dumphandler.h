#ifndef DUMPHANDLER_H
#define DUMPHANDLER_H

#include "FILEHANDLER/filehandler.h"

namespace FILEHANDLER{

class DumpHandler : public FILEHANDLER::FileHandler
{

    /**
      output helper class for db2 dump files
      tasks:
        - contains intelligence of csv formatted file headers and lines
    **/

public:
    DumpHandler();
    ~DumpHandler();

    void setUp();

    void writeHeader(DATA::TableHeader *, std::ofstream *);
    void writeRow(std::ofstream *);
    void finalizeOutput(std::ofstream *);

    unsigned long long getRowsWritten();

private:
    void writeData(std::vector<QString> , std::ofstream *);
    QString getCellData(unsigned int);
    QString getTypeName(ENUM::COLUMN_TYPES);
};

} // namespaces

#endif // DUMPHANDLER_H
