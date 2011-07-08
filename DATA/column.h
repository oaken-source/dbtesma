#ifndef COLUMN_H
#define COLUMN_H

#include "WRAPPER/intwrapper.h"
#include "WRAPPER/varcharwrapper.h"
#include "WRAPPER/charwrapper.h"

#include <map>
#include <vector>

namespace DATA {

class Column
{

	/**
	Column Data Class
	tasks:
		holds column information
		contains column generation logic
	**/

public:

	enum GENERATION_TYPES
	{
		RANGED,
		RANDOM,
		STORED
	} typedef GENERATION_TYPES;

	enum DATATYPES
	{
		INT,
		VARCHAR,
		CHAR
	} typedef DATATYPES;

	enum KEYTYPES
	{
		KEY_NONE,
		KEY_PRIMARY
	} typedef KEYTYPES;

	enum ATTRIBUTES
	{
		ATTR_NONE,
		ATTR_NAME,
		ATTR_DATATYPE,
		ATTR_LENGTH,
		ATTR_KEY,
		ATTR_BASEVALUE,
		ATTR_KEY_GROUP,
		ATTR_UNIQUE,
		ATTR_FOREIGNKEY
	} typedef ATTRIBUTES;

	Column() : _generationMethod(&DATA::Column::generateDataRandom), _attributes(std::map<ATTRIBUTES, std::string>()), _wrapper(NULL), _datatype(DATATYPES::INT), _length(0), _key(KEYTYPES::KEY_NONE), _unique(0), _rows(0), _dex(0), _siz(0), _parentCount(0), _parentColumns(0), _siblingCount(0), _out(new std::ofstream()), _cached(0) {};
	Column(const Column &obj) : _generationMethod(obj._generationMethod), _attributes(obj._attributes), _wrapper(obj._wrapper), _datatype(obj._datatype), _length(obj._length), _key(obj._key), _unique(obj._unique), _rows(obj._rows), _dex(obj._dex), _siz(obj._siz), _parentCount(obj._parentCount), _parentColumns(obj._parentColumns), _siblingCount(obj._siblingCount), _out(obj._out), _cached(obj._cached) {};
	Column& operator=(const Column&);
	~Column();

	/** raw attribute accessing **/
	void setAttribute(ATTRIBUTES, std::string&);
	bool getAttribute(ATTRIBUTES, std::string&);
	
	/** different top-level generation methods **/
	void next();
	void nextNoIncrement();
	void nextHarden();
	void incrementTemp();
	void incrementTempSmall();
	void resetTemp();
	void print();
	void printOffset();
	
	/** getter / setter for actual attributes **/
	void setDatatype(DATATYPES);
	DATATYPES getDatatype();
	void setLength(unsigned long long);
	unsigned long long getLength();
	void setKey(KEYTYPES);
	KEYTYPES getKey();
	void setUniqueValueCount(unsigned long long);
	unsigned long long getUniqueValueCount();
	
	void setGenerationType(GENERATION_TYPES);
	
	void setupMulticolumKeyConstraints(int, int, unsigned long long);
	
	/** primary key related attributes **/
	int getDex();
	void setDex(int);
	int getSiz();
	void setSiz(int);
	unsigned long long getRows();

	void getBasevalue(std::string&);
	bool setBasevalue(std::string&);
	void seedBasevalue();
	
	void populateDatatypeWrapper();
	void populateOutstream(std::ofstream*);

	/** parent column registration for fds, reverse fds and fks **/
	void registerReferences(std::vector<DATA::Column*>&);
	void registerReverseReferences(std::vector<DATA::Column*>&);
	void registerFKReferences(std::vector<DATA::Column*>&);
	
	bool isIndependent();
	
	void registerSiblingCount(unsigned int in) { _siblingCount = in; } 
	
private:

	/** bottom-level generation methods **/
	void generateDataRandomInRange();
	void generateDataRandom();
	void generateDataKeyPrimary();

	void processFuncdep();
	void processFuncdepInRange();
	void processReverseFuncdep();
	void processFKDepSingle();
	void processFKDepDeep();
	
	void (DATA::Column::*_generationMethod)();

	std::map<ATTRIBUTES, std::string> _attributes;

	DATA::WRAPPER::DatatypeWrapper *_wrapper;

	DATATYPES _datatype;	
	unsigned long long _length;
	KEYTYPES _key;
	unsigned long long _unique;

	unsigned long long _rows;
	int _dex;
	int _siz;

	unsigned int _parentCount;
	DATA::Column** _parentColumns;
	
	unsigned int _siblingCount;
	
	std::ofstream *_out;
	
	unsigned long long _cached;
	
};

} // namespaces

#endif // COLUMN_H
