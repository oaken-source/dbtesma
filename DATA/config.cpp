
#include "config.h"

namespace DATA {

/** public ******************************************************************************************************/

	void Config::newTable()
	{	
		DATA::Table *newTable = new DATA::Table();
		_tables.push_back(newTable);
	}	

	void Config::dumpToStdout()
	{
		std::vector<DATA::Table*>::iterator i;
		for(i = _tables.begin(); i != _tables.end(); i++)
		{
			std::string tableName;
			(*i)->getAttribute(DATA::Table::ATTR_NAME, tableName);
			std::cout << "table: " << tableName << " - " << (*i)->getRowCount() << " rows" << std::endl;
			std::vector<DATA::Column*>::iterator k;
			for(k = (*i)->columns_begin(); k != (*i)->columns_end(); k++)
			{
				std::string columnName, columnGroup, basevalue;
				(*k)->getAttribute(DATA::Column::ATTR_KEY_GROUP, columnGroup);
				(*k)->getAttribute(DATA::Column::ATTR_NAME, columnName);
				(*k)->getBasevalue(basevalue);
				std::cout << "  column: " << columnName << " - " << (((*k)->getDatatype() == DATA::Column::INT)?"INT":"VARCHAR") << "(" << (*k)->getLength() << ") - [" << (*k)->getDex() << "][" << (*k)->getSiz() << "] - [" << (*k)->getUniqueValueCount() << "]  (" << basevalue << ")" << std::endl;
			}
			std::vector<DATA::Funcdep*>::iterator v;
			for(v = (*i)->funcdeps_begin(); v != (*i)->funcdeps_end(); v++)
			{
				std::vector<DATA::Column*>::iterator z = (*v)->lhs_begin();
				std::string columnName;
				(*z)->getAttribute(DATA::Column::ATTR_NAME, columnName);
				std::cout << "  fd: " << columnName;
				z++;
				for(; z != (*v)->lhs_end(); z++)
				{
					(*z)->getAttribute(DATA::Column::ATTR_NAME, columnName);
					std::cout << "," << columnName;
				}
				std::cout << " --> ";
				z = (*v)->rhs_begin();
				(*z)->getAttribute(DATA::Column::ATTR_NAME, columnName);
				std::cout << columnName;
				z++;
				for(; z != (*v)->rhs_end(); z++)
				{
					(*z)->getAttribute(DATA::Column::ATTR_NAME, columnName);
					std::cout << "," << columnName;
				}
				std::cout << (((*v)->isReverseFD())?" (reversed)":"");
				std::cout << std::endl;
			}			
		}
	}

	void Config::buildSchema()
	{
		std::vector<DATA::Table*>::iterator i;
		for(i = _tables.begin(); i != _tables.end(); i++)
		{
			std::string tableName;
			(*i)->getAttribute(DATA::Table::ATTR_NAME, tableName);
			
			std::cout << "drop table if exists " << tableName << ";\ncreate table " << tableName << " (";

			std::vector<DATA::Column*>::iterator k, kLast;
			for(k = (*i)->columns_begin(), kLast = (*i)->columns_end(), kLast--; k != (*i)->columns_end(); k++)
			{
				std::string columnName;
				(*k)->getAttribute(DATA::Column::ATTR_NAME, columnName);
				
				std::cout << columnName << " ";
				
				DATA::Column::DATATYPES type = (*k)->getDatatype();
				unsigned int length = (*k)->getLength();

				switch(type)
				{
				case DATA::Column::INT:
					std::cout << ((length > 9)?"big":"") << "int(" << length << ")";
					break;
				case DATA::Column::CHAR:
					std::cout << "char(" << length << ")";
					break;
				case DATA::Column::VARCHAR:
					std::cout << "varchar(" << length << ")";
					break;
				}
				
				if(k != kLast)
					std::cout << ", ";
			}

			std::cout << ");" << std::endl;
			
			std::string pathName = tableName;
			pathName += ".csv";
			HELPER::OsAbstractionHelper::makeAbsolute(pathName);

			std::cout << "load data infile \"" << pathName << "\" into table " << tableName << " fields terminated by ',';" << std::endl;
		}
	}
	
	void Config::buildSchemaWithoutDatatypes()
	{
		std::vector<DATA::Table*>::iterator i;
		for(i = _tables.begin(); i != _tables.end(); i++)
		{
			std::string tableName;
			(*i)->getAttribute(DATA::Table::ATTR_NAME, tableName);
			
			std::cout << "drop table if exists " << tableName << ";\ncreate table " << tableName << " (";

			std::vector<DATA::Column*>::iterator k, kLast;
			for(k = (*i)->columns_begin(), kLast = (*i)->columns_end(), kLast--; k != (*i)->columns_end(); k++)
			{
				std::string columnName;
				(*k)->getAttribute(DATA::Column::ATTR_NAME, columnName);
				
				std::cout << columnName << " " << "varchar(" << (((*k)->getLength() < 255)?255:(*k)->getLength()) << ")";

				if(k != kLast)
					std::cout << ", ";
			}

			std::cout << ");" << std::endl;
			
			std::string pathName = tableName;
			pathName += ".csv";
			HELPER::OsAbstractionHelper::makeAbsolute(pathName);

			std::cout << "load data infile \"" << pathName << "\" into table " << tableName << " fields terminated by ',';" << std::endl;
		}		
	}
	
	void Config::buildSchemaAsJSON()
	{
		std::cout << "{\"tables\":[";
	
		std::vector<DATA::Table*>::iterator i, iLast;
		for(i = _tables.begin(), iLast = _tables.end(), iLast--; i != _tables.end(); i++)
		{
			std::string tableName;
			(*i)->getAttribute(DATA::Table::ATTR_NAME, tableName);
			
			std::cout << "{\"name\":\"" << tableName << "\", \"rowNumbers\":" << (*i)->getRowCount() << ", \"columnModels\":[";

			std::vector<DATA::Column*>::iterator k, kLast;
			for(k = (*i)->columns_begin(), kLast = (*i)->columns_end(), kLast--; k != (*i)->columns_end(); k++)
			{
				std::string columnName;
				(*k)->getAttribute(DATA::Column::ATTR_NAME, columnName);
				
				unsigned int length = (*k)->getLength();
				
				std::cout << "{\"name\":\"" << columnName << "\", \"distributionMode\":\"\", \"cellSize\":" << length << ", \"dataType\":\"";
				
				DATA::Column::DATATYPES type = (*k)->getDatatype();

				switch(type)
				{
				case DATA::Column::INT:
					std::cout << ((length > 9)?"BIG":"") << "INT";
					break;
				case DATA::Column::CHAR:
					std::cout << "CHAR";
					break;
				case DATA::Column::VARCHAR:
					std::cout << "VARCHAR";
					break;
				}
				
				std::cout << "\"}";

				if(k != kLast)
					std::cout << ", ";
			}

			std::cout << "]}";
			if(i != iLast)
				std::cout << ", ";
		}
		std::cout << "], \"InclusionDependencies\":[]}" << std::endl;
	}
	
	DATA::Table* Config::findTableByName(std::string &in)
	{
		std::vector<DATA::Table*>::iterator i;
		std::string tableName;
		for(i = _tables.begin(); i != _tables.end(); i++)
		{
			(*i)->getAttribute(DATA::Table::ATTR_NAME, tableName);
			if(!tableName.compare(in))
				return (*i);
		}
		
		return NULL;
	}

/** private *****************************************************************************************************/

} // namespaces
