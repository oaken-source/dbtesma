#include "tableheader.h"

namespace DATA {

/** public ******************************************************************************************************/

    TableHeader::TableHeader()
    {
        /** standard constructor **/
    }

    TableHeader::~TableHeader()
    {
        /** standard destructor **/
    }

    void TableHeader::setUp()
    {
        /** clear the column information **/
        _cols.clear();
    }

    void TableHeader::addColumn(DATA::ColumnHead col)
    {
        /** adds a column to the header
            params:
              - col: column information to be added to the header
        **/
        _cols.push_back(col);
    }

    void TableHeader::addPrimaryKey()
    {
        /** adds a primary key to the current column schema **/
        _cols[0].setType(ENUM::INT);
        _cols[0].setExtra(ENUM::PRIMARY_KEY);
    }

    void TableHeader::applyFunctionalDependencies(unsigned int probFD)
    {
        /** add FD's to table header
            params:
              - probFD: probability of functional dependencies
        **/
        std::vector<DATA::ColumnHead>::iterator i = _cols.begin();
        for(;i<_cols.end();i++)
        {
            unsigned int x = rand()%100;
            if(x<probFD)
            {
                addFunctionalDependency(&(*i));
            }
        }
    }

    void TableHeader::ensureFunctionalDependencies(std::vector<QString> *row)
    {
        /** ensure fds for a whole row
            params:
              - row: one row of data
        **/
        std::vector<DATA::ColumnHead>::iterator i = _cols.begin();
        for(unsigned int x = 0;i!=_cols.end();i++,x++)
        {
            if((*i).getActiveDep())
            {
                ensureFunctionalDependencies(row, i, x);
            }
        }
    }

    std::vector<DATA::ColumnHead>::iterator TableHeader::begin()
    {
        /** returns iterator to _cols **/
        return _cols.begin();
    }

    std::vector<DATA::ColumnHead>::iterator TableHeader::end()
    {
        /** returns iterator to _cols (last) **/
        return _cols.end();
    }

    unsigned int TableHeader::size()
    {
        /** returns size of _cols vector **/
        return _cols.size();
    }

    DATA::ColumnHead TableHeader::operator[](unsigned int i)
    {
        /** implement operator [] for direct access to _cols
            params:
              - i: index
        **/
        return _cols[i];
    }

    DATA::ColumnHead TableHeader::at(unsigned int i)
    {
        /** just like operator []
            params:
              - i: index
        **/
        return _cols[i];
    }

/** private *****************************************************************************************************/

    void TableHeader::addFunctionalDependency(DATA::ColumnHead *i)
    {
        /** adds a random fd to the specified column
            params:
              - i: pointer to active dep
        **/
        unsigned int x = (rand()%(size()-1))+1;
        addFunctionalDependency(i, &_cols[x]);
    }

    void TableHeader::addFunctionalDependency(DATA::ColumnHead *i, DATA::ColumnHead *col)
    {
        /** creates a fd between two specified columns
            params:
              - i: pointer to active dep
              - col: pointer to passive dep
        **/
        if(i!=col && i->getExtra() != ENUM::PRIMARY_KEY && !i->getPassiveDep() && !col->getActiveDep() && !i->getActiveDep() && !col->getPassiveDep())
        {
            i->addActiveDep(col);
            col->addPassiveDep(i);
        }
    }

    void TableHeader::ensureFunctionalDependencies(std::vector<QString> *row, std::vector<DATA::ColumnHead>::iterator i, unsigned int x)
    {
        /** ensure fds for a single column
            params:
              - row: one row of data
              - i: iterator on header infomation
              - x: "equals" i, but is the index of the column in the vector that i points to
        **/
        if((*i).getPassiveDep())
        {
            std::vector<DATA::ColumnHead*>::iterator v = (*i).beginPassive();
            for(;v!=(*i).endPassive();v++)
            {
                std::vector<DATA::ColumnHead>::iterator k = _cols.begin();
                unsigned int y = 0;
                while(&(*k) != (*v)) {y++; k++;}
                ensureFunctionalDependencies(row, k, y);
            }
        }
        std::vector<DATA::ColumnHead*>::iterator j = (*i).beginActive();
        for(;j!=(*i).endActive();j++)
        {
            std::vector<DATA::ColumnHead>::iterator k = _cols.begin();
            unsigned int y = getPosition((*j));
            if(valueAlreadyStored((*row)[x], x, y))
                (*row)[y] = fetchValue((*row)[x], x, y);
            else
                insertValue((*row)[x], x, y, (*row)[y]);
        }
    }

    bool TableHeader::valueAlreadyStored(QString str, unsigned int x, unsigned int y)
    {
        /** check if there is already a value stored
            params:
              - str1: active fd string
              - x: active column number
              - y: passive column number
        **/
        // return (_values.contains(x) && _values[x].contains(y) && _values[x][y].contains(str)); // no lazy evaluation?
        if(!_values.contains(x))
            return false;
        if(!_values[x].contains(y))
            return false;
        if(!_values[x][y].contains(str))
            return false;
        return true;
    }

    QString TableHeader::fetchValue(QString str, unsigned int x, unsigned int y)
    {
        /** return value from map
            params:
              - str1: active fd string
              - x: active column number
              - y: passive column number
        **/
        return (_values[x][y][str]);
    }

    void TableHeader::insertValue(QString str1, unsigned int x, unsigned int y, QString str2)
    {
        /** insert value to map
            params:
              - str1: active fd string
              - x: active column number
              - y: passive column number
              - str2: passive fd string
        **/
        if(_values.contains(x))
        {
            if(_values[x].contains(y))
                _values[x][y].insert(str1, str2);
            else
            {
                _values[x].insert(y, QMap<QString, QString>());
                _values[x][y].insert(str1, str2);
            }
        }
        else
        {
            _values.insert(x, QMap<unsigned int, QMap<QString, QString> >());
            _values[x].insert(y, QMap<QString, QString>());
            _values[x][y].insert(str1, str2);
        }
    }

    unsigned int TableHeader::getPosition(DATA::ColumnHead* col)
    {
        /** get position of column in header
            params:
              - col: pointer to specified column
        **/
        std::vector<DATA::ColumnHead>::iterator i = _cols.begin();
        for(unsigned int x = 0;i!=_cols.end();i++,x++)
            if(col == &(*i))
                return x;
    }

} // namespaces
