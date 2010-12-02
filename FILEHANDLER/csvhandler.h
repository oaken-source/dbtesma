#ifndef CSVHANDLER_H
#define CSVHANDLER_H

#include "FILEHANDLER/filehandler.h"

namespace FILEHANDLER{

class CsvHandler : public FILEHANDLER::FileHandler
{

    /**
      output helper class for csv files
      tasks:
        - contains intelligence of csv formatted file headers and lines
    **/

public:
    CsvHandler();
    ~CsvHandler();

    void setUp();

    void writeHeader(DATA::TableHeader *, std::ofstream *);
    void writeRow(std::ofstream *);
    void finalizeOutput(std::ofstream *);

    unsigned long long getRowsWritten();

private:
    void writeData(std::vector<QString>, std::ofstream *);
    QString getCellData(unsigned int);
};

} // namespaces

#endif // CSVHANDLER_H
