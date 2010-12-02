#include "intwrapper.h"

namespace DATA { namespace WRAPPER {

/** public ******************************************************************************************************/

    IntWrapper::IntWrapper()
    {
        /** default constructor **/
    }

    IntWrapper::~IntWrapper()
    {
        /** default destructor **/
    }

    void IntWrapper::setUp()
    {
        /** prepare hashmap and fill with random ints before main calculation **/
        _key = 0;
        _hash.clear();
        for(unsigned int i=0;i<PREPARED_INT_HASH_SIZE;i++)
        {
            _hash.insert(i,getRandomValue());
        }
    }

    QString IntWrapper::lookUp(unsigned int key)
    {
        /** get specified entry in hash map
            params:
              - key: key to look up value from
        **/
        _lastKey = key;
        return *(_hash.find(key));
    }

    QString IntWrapper::lookUp()
    {
        /** get next entry in hash map **/
        incrementKey();
        return *(_hash.find(_key));
    }

/** private *****************************************************************************************************/

    void IntWrapper::incrementKey()
    {
        /** increment hash access key within given limits **/
        _key = (_key + rand()%100) % PREPARED_INT_HASH_SIZE;
        _lastKey = _key;
    }

    QString IntWrapper::getRandomValue()
    {
        /** generate random value **/
        unsigned long ret = 0;
        for(unsigned int i=0;i<RANGE_DIGITS;i++)
            ret += (rand()%10)*pow(10,i);
        return QString::number(ret);
    }

}} // namespaces
