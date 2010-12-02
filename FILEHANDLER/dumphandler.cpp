#include "dumphandler.h"

namespace FILEHANDLER{

/** public ******************************************************************************************************/

    DumpHandler::DumpHandler()
    {
        /** special constructor to init datawrapper classes **/
        _iw = new DATA::WRAPPER::IntWrapper();
        _vw = new DATA::WRAPPER::VarcharWrapper();
    }

    DumpHandler::~DumpHandler()
    {
        /** standard destructor **/
    }

    void DumpHandler::setUp()
    {
        /** set everything up **/
        _rowsWritten = 0;
        _iw->setUp();
        _vw->setUp();
    }

    void DumpHandler::writeHeader(DATA::TableHeader *head, std::ofstream *out)
    {
        /** write header information to specified outstream
            params:
              - head: vector of column information, provided by HeaderGenerator::buildHeader()
              - out: reference of output stream
        **/
        _head = head;
        std::vector<DATA::ColumnHead>::iterator i = _head->begin();
        (*out) << "CREATE TABLE table_1 \n(\n  " << (*i).getName().toStdString() << getTypeName((*i).getType()).toStdString();
        ++i;
        while(i!=_head->end())
        {
            (*out) << ",\n  " << (*i).getName().toStdString() << getTypeName((*i).getType()).toStdString();
            ++i;
        }
        (*out) << ("\n);\n\nINSERT INTO table_1\nVALUES");
    }

    void DumpHandler::writeRow(std::ofstream *out)
    {
        /** write one row of data to the file
            params:
              - out: target outstream
        **/
        if(_rowsWritten > 0)
            (*out) << ",";
        (*out) << "\n  (" << QString::number(_rowsWritten).toStdString();
        std::vector<QString> row;
        for(unsigned int i=0;i<_head->size();i++)
            row.push_back(getCellData(i));
        _head->ensureFunctionalDependencies(&row);
        writeData(row, out);
        (*out) << ")";
        _rowsWritten++;
    }

    unsigned long long DumpHandler::getRowsWritten()
    {
        /** getter for _rowsWritten **/
        return _rowsWritten;
    }

    void DumpHandler::finalizeOutput(std::ofstream *out)
    {
        /** signalize end of db2 command, not really neccessary though
            params:
              - out: target outstream
        **/
        (*out) << ";";

    }

/** private *****************************************************************************************************/

    void DumpHandler::writeData(std::vector<QString> row, std::ofstream *out)
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

    QString DumpHandler::getCellData(unsigned int i)
    {
        /** return value corresponding to column information
            params:
              - i: number of column
        **/
        DISPATCH_AND_RETURN_FROM_COLUMN_TYPE(QString, lookUp())
    }

    QString DumpHandler::getTypeName(ENUM::COLUMN_TYPES type)
    {
        /** returns dbs type name for given datatype
            params:
              - type: type of the column to get the typename for
        **/
        //dispatch
        QString ret = QString();
        switch(type)
        {
        case ENUM::INT:
            ret = QString(" INT(");
            break;
        case ENUM::VARCHAR:
            ret = QString(" VARCHAR(");
            break;
        }
        ret.append(QString::number(RANGE_DIGITS));
        ret.append(")");
        return ret;
    }

} // namespace
