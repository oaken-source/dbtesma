#include "varcharwrapper.h"

namespace DATA { namespace WRAPPER {

/** public ******************************************************************************************************/

    VarcharWrapper::VarcharWrapper()
    {
        /** special constructor to fill alphabet **/
        _alphabet.clear();
        for(unsigned int i=65;i<=90;i++)
            _alphabet.insert((i-65),QChar::fromAscii(i));
    }

    VarcharWrapper::~VarcharWrapper()
    {
        /** defaule destructor **/
    }

    void VarcharWrapper::setUp()
    {
        /** prepare hashmap and fill with random strings before main calculation **/
        _key = 0;
        _hash.clear();
        for(unsigned int i=0;i<PREPARED_VARCHAR_HASH_SIZE;i++)
        {
            _hash.insert(i,getRandomValue());
        }
    }

    QString VarcharWrapper::lookUp(unsigned int key)
    {
        /** get specified entry in hash map
            params:
              - key: key to look up value from
        **/
        _lastKey = key;
        return *(_hash.find(key));
    }

    QString VarcharWrapper::lookUp()
    {
        /** get next entry in hash map **/
        incrementKey();
        return *(_hash.find(_key));
    }

/** private *****************************************************************************************************/

    void VarcharWrapper::incrementKey()
    {
        /** increment hash access key within given limits **/
        _key = (_key + rand()%100) % PREPARED_VARCHAR_HASH_SIZE;
        _lastKey = _key;
    }

    QString VarcharWrapper::getRandomValue()
    {
        /** generate random value **/
        QString ret;
        for(unsigned int i=0;i<RANGE_DIGITS;i++)
            ret.append(getRandomCharFromAlphabet());
        return ret;
    }

    QChar VarcharWrapper::getRandomCharFromAlphabet()
    {
        /** returns random char from specified alphabet **/
        unsigned int i = rand() % _alphabet.size();
        return _alphabet[i];
    }

}} // namespaces
