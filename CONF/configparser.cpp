
#include "configparser.h"

namespace CONF {

/** public ******************************************************************************************************/

	Configparser& Configparser::operator=(const Configparser &rhs)
	{
		_filename = rhs._filename;
		_in = rhs._in;
		_context = rhs._context;
	
		return *this;
	}

	Configparser::~Configparser() 
	{
		_in->close();
	}

	bool Configparser::parseAndValidate(DATA::Config *config)
	{
		CONF::Configvalidator *cv = new CONF::Configvalidator();

		bool good = false;
		while(1)
		{
			if(!parse(config))
				break;
			if(!cv->validate(config))
				break;

			good = true;
	
			break;
		}

		return good;	
	}
	
/** private *****************************************************************************************************/

	bool Configparser::parse(DATA::Config *conf)
	{
		_context = NONE;
		std::string line;
		unsigned int lineNo = 0;		

		bool good = true;

		while (_in->good() && good)
		{
			lineNo++;
			getline((*_in),line);
			good = processLine(line, conf);
		}

		if(!good)
		{
			std::stringstream sst;
			sst << "invalid syntax at line " << lineNo << " near '" << line << "'";
			std::string st = sst.str();
			conf->setErrorString(st);
		}

		return good;
	}

	bool Configparser::processLine(std::string &in, DATA::Config *conf)
	{
		HELPER::StringHelper::stripComments(in);
	
		HELPER::StringHelper::trim(in);

		if(!HELPER::StringHelper::empty(in))
		{
			switch(_context)
			{
				case NONE:
					return processLineContextNone(in, conf);
				case TABLE:
					return processLineContextTable(in, conf);
				case COLUMN:
					return processLineContextColumn(in, conf);
				case FUNCDEP:
					return processLineContextFuncDep(in, conf);
			}
		}
		return true;
	}

	bool Configparser::processLineContextNone(std::string &in, DATA::Config *conf)
	{		
		// things that can happen here:
		//   - new table expression
		std::string key;

		key = "table";
		if(HELPER::StringHelper::stripleft(in, key))
		{
			key = "=";
			if(HELPER::StringHelper::stripleft(in, key))
			{
				key = "{";
				if(HELPER::StringHelper::stripleft(in, key))
				{
					if(HELPER::StringHelper::empty(in))
					{
						conf->newTable();
						_context = TABLE;
						return true;
					}
				}
			}
		}

		return false;
	}

	bool Configparser::processLineContextTable(std::string &in, DATA::Config *conf)
	{
		// things that can happen here:
		//   - name attribute
		//   - rows attribute
		//   - new column expression
		//   - new founctional dependency expression
		//   - end table expression 
		std::string key, key_name, key_rows;
		DATA::Table::ATTRIBUTES type = DATA::Table::ATTR_NONE;

		key_name = "name";
		key_rows = "rows";

		if(HELPER::StringHelper::stripleft(in, key_name))
			type = DATA::Table::ATTR_NAME;
		else if(HELPER::StringHelper::stripleft(in, key_rows))
			type = DATA::Table::ATTR_ROWS;
	
		if(type != DATA::Table::ATTR_NONE)
		{
			key = "=";
			if(HELPER::StringHelper::stripleft(in, key))
			{
				std::string value;
				if(HELPER::StringHelper::popQuotedValue(in, value))
				{
					if(HELPER::StringHelper::empty(in))
					{
						conf->setTableAttribute(type, value);
						return true;
					}
				}
			}
		}

		key = "column";
		if(HELPER::StringHelper::stripleft(in, key))
		{
			key = "=";
			if(HELPER::StringHelper::stripleft(in, key))
			{
				key = "{";
				if(HELPER::StringHelper::stripleft(in, key))
				{
					if(HELPER::StringHelper::empty(in))
					{
						conf->newColumn();
						_context = COLUMN;
						return true;
					}
				}
			}
		}
		
		key = "functional_dep";
		if(HELPER::StringHelper::stripleft(in, key))
		{
			key = "=";
			if(HELPER::StringHelper::stripleft(in, key))
			{
				key = "{";
				if(HELPER::StringHelper::stripleft(in, key))
				{
					if(HELPER::StringHelper::empty(in))
					{
						conf->newFuncdep();
						_context = FUNCDEP;
						return true;
					}
				}
			}
		}

		key = "}";
		if(HELPER::StringHelper::stripleft(in, key))
		{
			if(HELPER::StringHelper::empty(in))
			{
				_context = NONE;
				return true;
			}
		}

		return false;
	}

	bool Configparser::processLineContextColumn(std::string &in, DATA::Config *conf)
	{
		// things that can happen here:
		//   - name attribute
		//   - datatype attribute
		//   - length attribute
		//   - key attribute
		//   - basevalue attribute
		//   - unique attribute
		//   - foreignkey attribute
		//   - end column expression
		std::string key, key_name, key_datatype, key_length, key_key, key_basevalue, key_key_group, key_unique, key_foreignkey;
		DATA::Column::ATTRIBUTES type = DATA::Column::ATTR_NONE;

		key_name = "name";
		key_datatype = "datatype";
		key_length = "length";
		key_key_group="key_group";
		key_key = "key";
		key_basevalue = "basevalue";
		key_unique = "unique";
		key_foreignkey = "foreignkey";

		if(HELPER::StringHelper::stripleft(in, key_name))
			type = DATA::Column::ATTR_NAME;
		else if(HELPER::StringHelper::stripleft(in, key_datatype))
			type = DATA::Column::ATTR_DATATYPE;
		else if(HELPER::StringHelper::stripleft(in, key_length))
			type = DATA::Column::ATTR_LENGTH;
		else if(HELPER::StringHelper::stripleft(in, key_key_group))
			type = DATA::Column::ATTR_KEY_GROUP;
		else if(HELPER::StringHelper::stripleft(in, key_key))
			type = DATA::Column::ATTR_KEY;
		else if(HELPER::StringHelper::stripleft(in, key_basevalue))
			type = DATA::Column::ATTR_BASEVALUE;
		else if(HELPER::StringHelper::stripleft(in, key_unique))
			type = DATA::Column::ATTR_UNIQUE;
		else if(HELPER::StringHelper::stripleft(in, key_foreignkey))
			type = DATA::Column::ATTR_FOREIGNKEY;

		if(type != DATA::Column::ATTR_NONE)
		{
			key = "=";
			if(HELPER::StringHelper::stripleft(in, key))
			{
				std::string value;
				if(HELPER::StringHelper::popQuotedValue(in, value))
				{
					if(HELPER::StringHelper::empty(in))
					{
						conf->setColumnAttribute(type, value);
						return true;
					}
				}
			}
		}

		key = "}";
		if(HELPER::StringHelper::stripleft(in, key))
		{
			if(HELPER::StringHelper::empty(in))
			{
				_context = TABLE;
				return true;
			}
		}

		return false;
	}
	
	bool Configparser::processLineContextFuncDep(std::string &in, DATA::Config *conf)
	{
		// things that can happen here:
		//   - lhs attribute
		//   - rhs attribute

		std::string key;

		key = "lhs";
		
		if(HELPER::StringHelper::stripleft(in, key))
		{
			key = "=";
			if(HELPER::StringHelper::stripleft(in, key))
			{
				std::string value;
				if(HELPER::StringHelper::popQuotedValue(in, value))
				{
					if(HELPER::StringHelper::empty(in))
					{
						conf->setFuncdepLhs(value);
						return true;
					}
				}
			}
		}
		
		key = "rhs";
		
		if(HELPER::StringHelper::stripleft(in, key))
		{
			key = "=";
			if(HELPER::StringHelper::stripleft(in, key))
			{
				std::string value;
				if(HELPER::StringHelper::popQuotedValue(in, value))
				{
					if(HELPER::StringHelper::empty(in))
					{
						conf->setFuncdepRhs(value);
						return true;
					}
				}
			}
		}

		key = "}";
		if(HELPER::StringHelper::stripleft(in, key))
		{
			if(HELPER::StringHelper::empty(in))
			{
				_context = TABLE;
				return true;
			}
		}

		return false;
	}

} // namespaces
