
#include "stringhelper.h"

namespace HELPER {

/** public ******************************************************************************************************/

	const char *StringHelper::_whitespace = " \t\r\n";
	const char *StringHelper::_commentDelimiter = "#";
	

	void StringHelper::stripComments(std::string &in)
	{
		std::string::size_type pos;

		pos = in.find(_commentDelimiter);
		if(pos != in.npos)
			in.erase(pos);
	}
	
	void StringHelper::trim(std::string &in)
	{
		trimleft(in);
		trimright(in);
	}
	
	void StringHelper::trimleft(std::string &in)
	{
		std::string::size_type pos;

		pos = in.find_first_not_of(_whitespace);
		if(pos != in.npos)
			in.erase(0, pos);
		else
			in.erase(0);
	}
	
	void StringHelper::trimright(std::string &in)
	{
		std::string::size_type pos;

		pos = in.find_last_not_of(_whitespace);
		if(pos != in.npos)
			in.erase(pos + 1);
		else
			in.erase(0);
	}
	
	bool StringHelper::empty(std::string &in)
	{
		return in.length() == 0;
	}

	bool StringHelper::stripleft(std::string &in, std::string &sub)
	{
		std::string::size_type pos;
		pos = in.find(sub);
		if(pos == 0)
		{
			in.erase(0,sub.length());
			trimleft(in);
			return true;
		}
		else
			return false;
	}

	bool StringHelper::popQuotedValue(std::string &in, std::string &value)
	{
		std::string key;
		key = "\"";
		if(stripleft(in, key))
		{
			std::string::size_type pos;
			pos = in.find(key);
			
			if(pos != in.npos)
			{
				value = in.substr(0, pos);
				in.erase(0, pos+1);
				trimleft(in);
				return true;
			}
		}
		
		return false;
	}

	bool StringHelper::popTableName(std::string &in, std::string &value)
	{
		std::string key;
		key = ":";

		std::string::size_type pos;
		pos = in.find(key);
			
		if(pos != in.npos && pos > 0)
		{
			value = in.substr(0, pos);
			in.erase(0, pos+1);
			trimleft(in);
			return true;
		}
		else
			return false;
	}
	
	bool StringHelper::popCSV(std::string &in, std::string &value)
	{
		if(empty(in))
			return false;
			
		std::string key;
		key = ",";

		std::string::size_type pos;
		pos = in.find(key);
			
		value = in.substr(0, pos);
		
		if(pos == in.npos)
			in.clear();
		else
			in.erase(0, pos+1);
		trimleft(in);

		return true;
	}

	int StringHelper::intval(std::string &in)
	{
		std::stringstream sst(in);
		int x;
		sst >> x;
		return x;
	}

	unsigned long long StringHelper::ullval(std::string &in)
	{
		std::stringstream sst(in);
		unsigned long long x;
		sst >> x;
		return x;
	}

	void StringHelper::strval(std::string &in, unsigned long long value)
	{
		std::stringstream sst;
		sst << value;
		in = sst.str();
	}

	int StringHelper::getIndexFromAlphabet(const char *alphabet, char in, unsigned int length)
	{
		unsigned int i;
		for(i = 0; i < length; i++)
			if(alphabet[i] == in) return i;
		return -1;
	}
	
/** tests *******************************************************************************************************/


} // namespaces













