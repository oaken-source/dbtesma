#include "csvhandler.h"

namespace FILEHANDLER{

/** public ******************************************************************************************************/

    CsvHandler::CsvHandler()
    {
        /** special constructor to init datawrapper classes **/
        _iw = new DATA::WRAPPER::IntWrapper();
        _vw = new DATA::WRAPPER::VarcharWrapper();
    }

    CsvHandler::~CsvHandler()
    {
        /** standard destructor **/
    }

    void CsvHandler::setUp()
    {
        /** set everything up **/
        _rowsWritten = 0;
        _iw->setUp();
        _vw->setUp();
    }

    void CsvHandler::writeHeader(DATA::TableHeader *head, std::ofstream *out)
    {
        /** write header information to specified outstream
            params:
              - head: vector of column information, provided by HeaderGenerator::buildHeader()
              - out: reference of output stream
        **/
        _head = head;
        std::vector<DATA::ColumnHead>::iterator i = _head->begin();
        (*out) << (*i).getName().toStdString();
        ++i;
        while(i!=_head->end())
        {
            (*out) << "," << (*i).getName().toStdString();
            ++i;
        }
    }

    void CsvHandler::writeRow(std::ofstream *out)
    {
        /** write one row of data to the file
            params:
              - out: target outstream
        **/
        (*out) << "\n" << QString::number(_rowsWritten).toStdString();
        std::vector<QString> row;
        for(unsigned int i=0;i<_head->size();i++)
            row.push_back(getCellData(i));
        _head->ensureFunctionalDependencies(&row);
        writeData(row, out);
        _rowsWritten++;
    }

    unsigned long long CsvHandler::getRowsWritten()
    {
        /** getter for _rowsWritten **/
        return _rowsWritten;
    }

    void CsvHandler::finalizeOutput(std::ofstream *out)
    {
        /** nothing to do here... **/
    }

/** private *****************************************************************************************************/

    void CsvHandler::writeData(std::vector<QString> row, std::ofstream *out)
    {
        /** write one line of data to specified outstream
            params:
              - row: row of data to write
              - out: target outstream
        **/
        std::vector<QString>::iterator i = row.begin(); i++;
        for(;i!=row.end();++i)
            (*out) << "," << (*i).toStdString();
    }

    QString CsvHandler::getCellData(unsigned int i)
    {
        /** return value corresponding to column information
            params:
              - i: number of column
        **/
        DISPATCH_AND_RETURN_FROM_COLUMN_TYPE(QString, lookUp())
    }

} // namespaces
