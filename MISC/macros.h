#ifndef MACROS_H
#define MACROS_H

    /**
      define global object macros for predefined configuration - under continuous change so don't hard-code them somewhere
    **/

/* return value interpretations of data generator */
#define FINISHED -1
#define INVALID -2
/* default width of generated values */
#define RANGE_DIGITS 8
/* numbers of prepared hash table values for datatypes - use different primes to avoid generating unintended functional dependencies */
#define PREPARED_INT_HASH_SIZE 11
#define PREPARED_VARCHAR_HASH_SIZE 7

/**
  a few prime numbers > 1000:
    1009 1013 1019 1021 1031 1033 1039 1049 1051 1061 1063 1069
**/

/* version */
#define VERSION_NUMBER "1.3.2"

/* function macro to auto-generate dispatcher methods */
#define DISPATCH_AND_CALL_FROM_FILE_TYPE(FUNCTION_NAME)             switch(_resultFileType) \
                                                                    { \
                                                                    case ENUM::CSV: \
                                                                        _csvHandler->FUNCTION_NAME; \
                                                                        break; \
                                                                    case ENUM::DUMP: \
                                                                        _dumpHandler->FUNCTION_NAME; \
                                                                        break; \
                                                                    }

#define DISPATCH_AND_RETURN_FROM_FILE_TYPE(TYPE, FUNCTION_NAME)     TYPE ret; \
                                                                    switch(_resultFileType) \
                                                                    { \
                                                                    case ENUM::CSV: \
                                                                        ret = _csvHandler->FUNCTION_NAME; \
                                                                        break; \
                                                                    case ENUM::DUMP: \
                                                                        ret = _dumpHandler->FUNCTION_NAME; \
                                                                        break; \
                                                                    } \
                                                                    return ret;

#define DISPATCH_AND_RETURN_FROM_COLUMN_TYPE(TYPE, FUNCTION_NAME)   TYPE ret; \
                                                                    switch((*_head)[i].getType()) \
                                                                    { \
                                                                    case ENUM::INT: \
                                                                        ret = _iw->FUNCTION_NAME; \
                                                                        break; \
                                                                    case ENUM::VARCHAR: \
                                                                        ret = _vw->FUNCTION_NAME; \
                                                                        break; \
                                                                    } \
                                                                    return ret;


#endif // MACROS_H
