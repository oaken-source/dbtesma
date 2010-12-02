#ifndef TABLEHEADER_H
#define TABLEHEADER_H

#include "DATA/columnhead.h"

#include "QMap"

#include <vector>
#include <cstdlib>

namespace DATA {

class TableHeader
{
public:
    TableHeader();
    ~TableHeader();

    void setUp();

    void addColumn(DATA::ColumnHead);
    void addPrimaryKey();
    void applyFunctionalDependencies(unsigned int);
    void ensureFunctionalDependencies(std::vector<QString> *);

    std::vector<DATA::ColumnHead>::iterator begin();
    std::vector<DATA::ColumnHead>::iterator end();
    unsigned int size();

    DATA::ColumnHead operator[](unsigned int);
    DATA::ColumnHead at(unsigned int);

private:
    void addFunctionalDependency(DATA::ColumnHead *);
    void addFunctionalDependency(DATA::ColumnHead *, DATA::ColumnHead *);

    bool valueAlreadyStored(QString, unsigned int, unsigned int);
    QString fetchValue(QString, unsigned int, unsigned int);
    void insertValue(QString, unsigned int, unsigned int, QString);

    void ensureFunctionalDependencies(std::vector<QString> *, std::vector<DATA::ColumnHead>::iterator, unsigned int);

    unsigned int getPosition(DATA::ColumnHead *);

    /** column information **/
    std::vector<DATA::ColumnHead> _cols;
    /** fd map **/
    QMap<unsigned int, QMap<unsigned int, QMap<QString, QString> > > _values;

};

} // namespaces

#endif // TABLEHEADER_H
