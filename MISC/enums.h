#ifndef ENUMS_H
#define ENUMS_H

namespace ENUM{

    /**
      global enum definition
      tasks:
        - implement main enumeration datatypes
    **/

enum RESULT_TABLE_TYPES
{
    SINGLE_TABLE_COLS_ROWS,
    SINGLE_TABLE_COLS_SIZE
} typedef RESULT_TABLE_TYPES;

enum RESULT_FYLE_TYPES
{
    CSV,
    DUMP
} typedef RESULT_FILE_TYPES;

enum COLUMN_TYPES
{
    INT,
    VARCHAR
} typedef COLUMN_TYPES;

enum COLUMN_EXTRA
{
    NONE,
    PRIMARY_KEY
} typedef COLUMN_EXTRA;

} //namespace

#endif // ENUMS_H
