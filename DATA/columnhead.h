#ifndef COLUMNHEAD_H
#define COLUMNHEAD_H

#include "MISC/enums.h"

#include <QString>

#include <vector>

namespace DATA {

class ColumnHead
{

    /**
      wrapper class for datatypes of column headers
      tasks:
        - contain name of column and datatype
    **/

public:
    ColumnHead(ENUM::COLUMN_TYPES, QString);
    ~ColumnHead();

    QString getName();
    ENUM::COLUMN_TYPES getType();
    ENUM::COLUMN_EXTRA getExtra();
    bool getActiveDep();
    bool getPassiveDep();

    std::vector<DATA::ColumnHead*>::iterator beginActive();
    std::vector<DATA::ColumnHead*>::iterator endActive();
    std::vector<DATA::ColumnHead*>::iterator beginPassive();
    std::vector<DATA::ColumnHead*>::iterator endPassive();

    void addActiveDep(DATA::ColumnHead *);
    void addPassiveDep(DATA::ColumnHead *);

    void setExtra(ENUM::COLUMN_EXTRA);
    void setType(ENUM::COLUMN_TYPES);

private:
    /** datatype of column **/
    ENUM::COLUMN_TYPES _type;
    /** extra information of column **/
    ENUM::COLUMN_EXTRA _extra;
    /** name of column **/
    QString _name;
    /** functional dependency vars **/
    bool _activeDep;
    bool _passiveDep;
    std::vector<DATA::ColumnHead*> _activeDeps;
    std::vector<DATA::ColumnHead*> _passiveDeps;

};

} // namespace

#endif // COLUMNHEAD_H
