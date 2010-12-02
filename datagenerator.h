#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "MISC/enums.h"
#include "MISC/macros.h"
#include "FILEHANDLER/csvhandler.h"
#include "FILEHANDLER/dumphandler.h"
#include "DATA/headergenerator.h"
#include "DATA/WRAPPER/intwrapper.h"
#include "DATA/WRAPPER/varcharwrapper.h"
#include "DATA/tableheader.h"

#include "QString"
#include "QTime"
#include "QFile"

#include <fstream>
#include <math.h>
#include <vector>


class DataGenerator
{

    /**
      main intelligence calss of data generation
      tasks:
        - collects intelligence of subclasses, holds instances of them
        - holds user input vars
        - organizes data generation
    **/

public:
    DataGenerator();
    ~DataGenerator();

    bool startUp(QString, std::vector<unsigned int>, ENUM::RESULT_TABLE_TYPES, ENUM::RESULT_FYLE_TYPES);
    void tearDown();
    void writeHeader();
    int writeData();

private:
    void initializeRandomness();
    void initializeGenerationVars(std::vector<unsigned int>, ENUM::RESULT_TABLE_TYPES, ENUM::RESULT_FYLE_TYPES);
    bool initializeOutstream(QString);
    void calculateRowsPerIteration();
    bool expectedTimePerCalculationExceedsOneSecond();

    void finalizeOutput();
    int getProgress();
    void writeRow();
    unsigned long long getRowsWritten();

    /** instance of csv file handler class **/
    FILEHANDLER::CsvHandler *_csvHandler;
    /** instance ot dump file handler class **/
    FILEHANDLER::DumpHandler *_dumpHandler;
    /** instamce of header generator class **/
    DATA::HeaderGenerator *_hg;

    /** ofstream for output **/
    std::ofstream _outStream;
    /** vector of user-inputted metrics **/
    std::vector<unsigned int> _args;
    /** result table type **/
    ENUM::RESULT_TABLE_TYPES _resultTableType;
    /** result file type **/
    ENUM::RESULT_FYLE_TYPES _resultFileType;

    /** generated header information **/
    DATA::TableHeader *_head;
    /** average numbers of rows to write per iteration **/
    unsigned int _calculatedRowsPerIteration;

    unsigned int _progressCounter;

    /** name of the output file - used for filesize check **/
    QString _outFileName;

};

#endif // DATAGENERATOR_H
