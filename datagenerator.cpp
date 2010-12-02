#include "datagenerator.h"

#include <QDir>

/** public ******************************************************************************************************/

    DataGenerator::DataGenerator()
    {
        /** initialize instaces of datatype wrappers and file handlers **/
        _csvHandler = new FILEHANDLER::CsvHandler();
        _dumpHandler = new FILEHANDLER::DumpHandler();
        _hg = new DATA::HeaderGenerator();
        _head = new DATA::TableHeader();
    }

    DataGenerator::~DataGenerator()
    {
        /** standard destructor **/
    }

    bool DataGenerator::startUp(QString dirName, std::vector<unsigned int> args, ENUM::RESULT_TABLE_TYPES tableType, ENUM::RESULT_FYLE_TYPES fileType)
    {
        /** main preparation method - for details see more specialized methods
            params:
              - dirName: output directory for generated tables
              - args: vector of user specified metrics
              - tableType: type of the result table
              - fileType: type of the result file
        **/
        initializeRandomness();
        initializeGenerationVars(args, tableType, fileType);
        if(!initializeOutstream(dirName))
            return false;
        else
            return true;
    }

    void DataGenerator::tearDown()
    {
        /** close ofstream after output **/
        _outStream.close();
    }

    void DataGenerator::writeHeader()
    {
        /** let file handler classes write file header **/
       _hg->buildHeader(_head);
        DISPATCH_AND_CALL_FROM_FILE_TYPE(writeHeader(_head, &_outStream))
    }

    int DataGenerator::writeData()
    {
        /** write calculated number of rows to file and return progress to ui **/
        for(unsigned int i=0;i<_calculatedRowsPerIteration;i++)
            writeRow();
        return getProgress();
    }

/** private *****************************************************************************************************/

    void DataGenerator::initializeRandomness()
    {
        /** call qsrand() to randomize **/
        QTime midnight(0,0,0);
        qsrand(midnight.secsTo(QTime::currentTime()));
    }

    void DataGenerator::initializeGenerationVars(std::vector<unsigned int> args, ENUM::RESULT_TABLE_TYPES tableType, ENUM::RESULT_FYLE_TYPES fileType)
    {
        /** set up instance vars from input, call setUp methods of helper classes
            params:
              - args: vector of user specified metrics
              - tableType: type of the result table
              - fileType: type of the result file
        **/
        _args.clear();
        _resultTableType = tableType;
        _resultFileType = fileType;
        std::vector<unsigned int>::iterator i;
        for(i = args.begin();i != args.end();++i)
            _args.push_back(*i);
        _hg->setUp(args[0], args[3], args[4], args[5]);
        _head->setUp();
        _csvHandler->setUp();
        _dumpHandler->setUp();
        calculateRowsPerIteration();
    }

    bool DataGenerator::initializeOutstream(QString dirName)
    {
        /** open new outstream with filename according to file type
            params:
              - dirName: output directory for generated tables
        **/
        QString temp = QDir::fromNativeSeparators(QDir::cleanPath(dirName));
        if(temp != "" && !temp.endsWith("/"))
            temp.append("/");

        QDir *directory = new QDir(temp);
        if(!directory->exists())
        {
            if(!directory->mkpath(temp))
                return false;
        }

        _outFileName = temp;
        _outFileName.append("table_1");
        switch(_resultFileType)
        {
        case ENUM::CSV:
            _outFileName.append(".csv");
            break;
        case ENUM::DUMP:
            _outFileName.append(".dump");
            break;
        }

        _outStream.open(_outFileName.toAscii());
        return true;
    }

    void DataGenerator::calculateRowsPerIteration()
    {
        /** calculates best tradeoff between performance and ui responsibility **/
        switch(_resultTableType)
        {
        case ENUM::SINGLE_TABLE_COLS_ROWS:
            _calculatedRowsPerIteration = _args[1] / 100;
            break;
        case ENUM::SINGLE_TABLE_COLS_SIZE:
            _calculatedRowsPerIteration = (int)(_args[1] * pow(1024,_args[2]) / (_args[0] * 100 * (RANGE_DIGITS + 1)));
            break;
        default:
            _calculatedRowsPerIteration = _args[1] / 100;
        }

        if(expectedTimePerCalculationExceedsOneSecond())
            _calculatedRowsPerIteration = 100000 / _args[0];
    }

    bool DataGenerator::expectedTimePerCalculationExceedsOneSecond()
    {
        /** helper method for calculateRowserIteration() **/
        return (_calculatedRowsPerIteration > 100000 / _args[0]);
    }

    int DataGenerator::getProgress()
    {
        /** calcualte calculation progress and return tu UI **/
        unsigned long long rowsWritten = getRowsWritten();
        int ret = 0;
        switch(_resultTableType)
        {
        case ENUM::SINGLE_TABLE_COLS_ROWS:
           if(rowsWritten >= _args[1]-1)
                ret = FINISHED;
            else
                ret = (rowsWritten*100)/_args[1];
            break;
        case ENUM::SINGLE_TABLE_COLS_SIZE:
            QFile outFile(_outFileName);
            unsigned long long size = outFile.size();
            if(size >= _args[1] * pow(1024,_args[2]))
                ret = FINISHED;
            else
                ret = (size*100)/(_args[1] * pow(1024,_args[2]));
            break;
        }
        if(ret == FINISHED)
            finalizeOutput();
        return ret;
    }

    void DataGenerator::writeRow()
    {
        /** tell file type handler to start row output **/
        DISPATCH_AND_CALL_FROM_FILE_TYPE(writeRow(&_outStream))
    }

    unsigned long long DataGenerator::getRowsWritten()
    {
        /** get _rowsWritten from active file type handler **/
        DISPATCH_AND_RETURN_FROM_FILE_TYPE(unsigned long long, getRowsWritten())
    }

    void DataGenerator::finalizeOutput()
    {
        /** call finalize method of active file type handler **/
        DISPATCH_AND_CALL_FROM_FILE_TYPE(finalizeOutput(&_outStream))
    }
