#include "columnhead.h"

namespace DATA {

/** public ******************************************************************************************************/

    ColumnHead::ColumnHead(ENUM::COLUMN_TYPES type, QString name)
    {
        /** special constructor to set given instance vars
            params:
              - type: type of new column
              - name: name of new column
        **/
        _type = type;
        _name = name;
        _extra = ENUM::NONE;
        _activeDep = false;
        _passiveDep = false;
        _activeDeps.clear();
        _passiveDeps.clear();
    }

    ColumnHead::~ColumnHead()
    {
        /** default destructor **/
    }

    QString ColumnHead::getName()
    {
        /** getter for _name **/
        return _name;
    }

    ENUM::COLUMN_TYPES ColumnHead::getType()
    {
        /** getter for _type **/
        return _type;
    }

    ENUM::COLUMN_EXTRA ColumnHead::getExtra()
    {
        /** getter for _extra **/
        return _extra;
    }

    bool ColumnHead::getActiveDep()
    {
        /** getter for _activeDep **/
        return _activeDep;
    }

    bool ColumnHead::getPassiveDep()
    {
        /** getter for _passiveDep **/
        return _passiveDep;
    }

    void ColumnHead::setExtra(ENUM::COLUMN_EXTRA extra)
    {
        /** setter for _extra
            params:
              - extra: additional flag to be set
        **/
        _extra = extra;
    }

    void ColumnHead::setType(ENUM::COLUMN_TYPES type)
    {
        /** setter for _type
            params:
              - type: datatype for column
        **/
        _type = type;
    }

    void ColumnHead::addActiveDep(DATA::ColumnHead *col)
    {
        /** adds 'active' functional dependency part
            params:
              - col: pointer to passive fd part
        **/
        _activeDep = true;
        _activeDeps.push_back(col);
    }

    void ColumnHead::addPassiveDep(DATA::ColumnHead *col)
    {
        /** adds 'passive' functional dependency part
            params:
              - col: pointer to active fd part
        **/
        _passiveDep = true;
        _passiveDeps.push_back(col);
    }

    std::vector<DATA::ColumnHead*>::iterator ColumnHead::beginActive()
    {
        /** returns iterator to first element of active deps **/
        return _activeDeps.begin();
    }

    std::vector<DATA::ColumnHead*>::iterator ColumnHead::endActive()
    {
        /** returns iterator to last element of active deps **/
        return _activeDeps.end();
    }

    std::vector<DATA::ColumnHead*>::iterator ColumnHead::beginPassive()
    {
        /** returns iterator to first element of passive deps **/
        return _passiveDeps.begin();
    }

    std::vector<DATA::ColumnHead*>::iterator ColumnHead::endPassive()
    {
        /** returns iterator to last element of passive deps **/
        return _passiveDeps.end();
    }

} // namespaces
