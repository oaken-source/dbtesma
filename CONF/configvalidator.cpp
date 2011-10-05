/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Henning                                      *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of the GNU General Public License as published by     *
 *    the Free Software Foundation, either version 3 of the License, or        *
 *    (at your option) any later version.                                      *
 *                                                                             *
 *    This program is distributed in the hope that it will be useful,          *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *    GNU General Public License for more details.                             *
 *                                                                             *
 *    You should have received a copy of the GNU General Public License        *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ******************************************************************************/
 
#include "configvalidator.h"

namespace CONF {

/** public ******************************************************************************************************/

	Configvalidator& Configvalidator::operator=(const Configvalidator &rhs)
	{
		_conf = rhs._conf;
		return *this;
	}

	Configvalidator::~Configvalidator() 
	{

	}

	bool Configvalidator::validate(DATA::Schema *conf)
	{
		_conf = conf;

		bool good = false;

		while(1)
		{	
			if(!validateTableCount())
				break;
			if(!validateTableNames())
				break;
			if(!validateTableAttributes())
				break;
			if(!validateTableColumns())
				break;
			if(!validateTableFuncdeps())
				break;
			if(!validateForeignKeyConstraints())
				break;
			if(!revalidateFuncdeps())
				break;
				
			good = true;

			break; // anyway
		}
		
		if(conf->hasHardenedFds())
			good = validateHardenedFds();
	
		return good;
	}

/** private *****************************************************************************************************/

	bool Configvalidator::validateTableCount()
	{
		/** ensure we have at least one table in table set **/
		if(_conf->size() == 0)
		{
			std::string error = "no tables";
			_conf->setErrorString(error);
			return false;
		}
		else
			return true;
	}

	bool Configvalidator::validateTableNames()
	{
		/** ensure unique table names in table set **/
		std::map<std::string, bool> tableNames;
		
		std::vector<DATA::Table*>::iterator i = _conf->begin();
		while(i != _conf->end())
		{
			std::string name;
			if((*i)->getAttribute(DATA::Table::ATTR_NAME, name))
			{
				if(tableNames.find(name) == tableNames.end())
					tableNames[name] = true;
				else
				{
					std::string error = name + ": name not unique";
					_conf->setErrorString(error);
					return false;
				}
			}
			else
			{
				std::string error = "unnamed table";
				_conf->setErrorString(error);
				return false;
			}
			i++;
		}		

		return true;
	}

	bool Configvalidator::validateTableAttributes()
	{
		/** fill missing row count values in table set **/
		std::vector<DATA::Table*>::iterator i = _conf->begin();
		while(i != _conf->end())
		{
			std::string rows_str;
			if((*i)->getAttribute(DATA::Table::ATTR_ROWS, rows_str))
			{
				unsigned long long rows = HELPER::Strings::intval(rows_str);
				(*i)->setRowCount(rows);
			}
			else
			{
				std::string name;
				(*i)->getAttribute(DATA::Table::ATTR_NAME, name);
				std::string error = name + ": row count required";
				_conf->setErrorString(error);
				return false;
			}

			i++;
		}		

		return true;
	}

	bool Configvalidator::validateTableColumns()
	{
		bool good = false;
		/** ensure column data is valid **/
		
		std::vector<DATA::Table*>::iterator i = _conf->begin();
		while(i != _conf->end())
		{
			while(1)
			{	
				good = false;

				if(!validateColumnCount(*i))
					break;
				if(!validateColumnNames(*i))
					break;
				if(!validateColumnDatatypes(*i))
					break;
				if(!validateColumnSize(*i))
					break;
				
				populateDatatypeWrappers(*i);
				
				if(!validateColumnKey(*i))
					break;
				if(!validateColumnBasevalue(*i))
					break;
				if(!validateColumnUniqueCount(*i))
					break;

				(*i)->normalize();
	
				good = true;
	
				break; // anyway
			}

			if(!good)
				break;

			i++;
		}		

		return good;
	}
	
	bool Configvalidator::validateTableFuncdeps()
	{
		bool good = true;
		/** ensure functional dependencies are valid **/
		
		std::vector<DATA::Table*>::iterator i = _conf->begin();
		while(i != _conf->end())
		{
			if(!(good &= validateFuncdepColumns(*i)))
				break;
			
			explodeFuncdeps(*i);
			
			if(!(good &= validateFuncdeps(*i)))
				break;
			
			i++;
		}		

		return good;	
	}

	bool Configvalidator::validateColumnCount(DATA::Table *table)
	{
		/** ensure we have at least one table in table set **/
		if((int)table->columns_size() == 0)
		{
			std::string tableName;
			table->getAttribute(DATA::Table::ATTR_NAME, tableName);
			std::string error = tableName + ": no columns";
			_conf->setErrorString(error);
			return false;
		}
		else
			return true;
	}

	bool Configvalidator::validateColumnNames(DATA::Table *table)
	{
		/** ensure unique column names in column set **/
		std::map<std::string, bool> columnNames;

		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		while(i != table->columns_end())
		{
			std::string name;
			if((*i)->getAttribute(DATA::Column::ATTR_NAME, name))
			{
				if(name.find(",") != name.npos)
				{
					std::string tableName;
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": " + name + ": name must not contain character ','";
					_conf->setErrorString(error);
					return false;					
				}
				else if(columnNames.find(name) == columnNames.end())
					columnNames[name] = true;
				else
				{
					std::string tableName;
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": " + name + ": name not unique";
					_conf->setErrorString(error);
					return false;
				}
			}
			else
			{
				std::string tableName;
				table->getAttribute(DATA::Table::ATTR_NAME, tableName);
				std::string error = tableName + ": unnamed column";
				_conf->setErrorString(error);
				return false;
			}
			i++;
		}

		return true;
	}

	bool Configvalidator::validateColumnDatatypes(DATA::Table *table)
	{
		/** ensure columns have only valid datatypes **/
		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		while(i != table->columns_end())
		{
			std::string datatype;
			if((*i)->getAttribute(DATA::Column::ATTR_DATATYPE, datatype))
			{
				if(datatype.compare("int") == 0)
					(*i)->setDatatype(DATA::Column::INT);
				else if(datatype.compare("varchar") == 0)
					(*i)->setDatatype(DATA::Column::VARCHAR);
				else if(datatype.compare("char") == 0)
					(*i)->setDatatype(DATA::Column::CHAR);
				else
				{
					std::string columnName, tableName;
					(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": " + columnName + ": unrecognized datatype \"" + datatype + "\"";
					_conf->setErrorString(error);
					return false;
				}
			}
			else
			{
				(*i)->setDatatype(DATA::Column::INT);
			}

			i++;
		}

		return true;
	}

	bool Configvalidator::validateColumnSize(DATA::Table *table)
	{
		/** fill missing column value length in column set **/
		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		while(i != table->columns_end())
		{
			std::string length_str;
			if((*i)->getAttribute(DATA::Column::ATTR_LENGTH, length_str))
			{
				unsigned long long length = HELPER::Strings::intval(length_str);
				(*i)->setLength(length);
			}
			else
			{
				std::string columnName, tableName;
				(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
				table->getAttribute(DATA::Table::ATTR_NAME, tableName);
				std::string error = tableName + ": " + columnName + ": column width required";
				_conf->setErrorString(error);
				return false;
			}

			i++;
		}

		return true;
	}
	
	bool Configvalidator::validateFuncdepColumns(DATA::Table *table)
	{
		std::vector<DATA::Funcdep*>::iterator i = table->funcdeps_begin();
		while(i != table->funcdeps_end())
		{
			std::string columnsString;
			(*i)->getLhsString(columnsString);
			
			while(1)
			{
				std::string column;
				if(HELPER::Strings::popCSV(columnsString, column))
				{
					
					DATA::Column *c = table->findColumnByName(column);
					if(c == 0)
					{
						std::string tableName;
						table->getAttribute(DATA::Table::ATTR_NAME, tableName);
						std::string error = tableName + ": functional_dep: \"" + column + "\" is not valid";
						_conf->setErrorString(error);
						return false;
					}
					else
					{
						(*i)->addLhsColumn(c);
					}
				}
				else
				{
					std::string tableName;
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": functional_dep: no lhs column specified";
					_conf->setErrorString(error);
					return false;
				}

				if(HELPER::Strings::empty(columnsString))
					break;
			}
			
			(*i)->getRhsString(columnsString);
			
			while(1)
			{
				std::string column;
				if(HELPER::Strings::popCSV(columnsString, column))
				{
					
					DATA::Column *c = table->findColumnByName(column);
					if(c == 0)
					{
						std::string tableName;
						table->getAttribute(DATA::Table::ATTR_NAME, tableName);
						std::string error = tableName + ": functional_dep: \"" + column + "\" is not valid";
						_conf->setErrorString(error);
						return false;
					}
					else
					{
						(*i)->addRhsColumn(c);
					}
				}
				else
				{
					std::string tableName;
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": functional_dep: no rhs column specified";
					_conf->setErrorString(error);
					return false;
				}

				if(HELPER::Strings::empty(columnsString))
					break;
			}
			
			i++;
		}
		
		return true;
	}

	void Configvalidator::explodeFuncdeps(DATA::Table *table)
	{
		std::vector<DATA::Funcdep*> newFuncdeps;
		std::vector<DATA::Funcdep*>::iterator i = table->funcdeps_begin();
		while(i != table->funcdeps_end())
		{
			std::vector<DATA::Column*>::iterator j = (*i)->rhs_begin();
			for(; j != (*i)->rhs_end(); j++)
			{
				DATA::Funcdep *fd = new DATA::Funcdep(*(*i));
				fd->replaceRhs(*j);
				newFuncdeps.push_back(fd);
			}
			
			i++;
		}
		
		table->setFuncdeps(newFuncdeps);
	}
	
	bool Configvalidator::validateFuncdeps(DATA::Table *table)
	{
		/** remove self-references **/
		std::vector<DATA::Funcdep*>::iterator i = table->funcdeps_begin();
		while(i != table->funcdeps_end())
		{
			if((*i)->lhs_find(*((*i)->rhs_begin())) != (*i)->lhs_end())
			{
				(*i)->lhs_erase((*i)->lhs_find(*((*i)->rhs_begin())));
				if((*i)->lhs_size() == 0)
				{
					std::string tableName, columnName;
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					(*((*i)->rhs_begin()))->getAttribute(DATA::Column::ATTR_NAME, columnName);
					std::string error = tableName + ": functional_dep: rhs column \"" + columnName + "\" has self references";
					_conf->setErrorString(error);
					return false;
				}
			}
			i++;
		}
		
		/** validate funcdep generation direction **/
		std::map<DATA::Column*, std::vector<DATA::Funcdep*> > rhsvalues;
		
		i = table->funcdeps_begin();
		while(i != table->funcdeps_end())
		{
			DATA::Column *rhs = *((*i)->rhs_begin());
			if(rhsvalues.find(rhs) == rhsvalues.end())
			{
				rhsvalues[rhs] = std::vector<DATA::Funcdep*>();
				rhsvalues[rhs].push_back(*i);
			}
			else
				rhsvalues[rhs].push_back(*i);

			i++;
		}

		std::vector<DATA::Funcdep*> newFuncdeps = std::vector<DATA::Funcdep*>();
		std::map<DATA::Column*, bool> occupiedColumns = std::map<DATA::Column*, bool>();
		
		std::map<DATA::Column*, std::vector<DATA::Funcdep*> >::iterator r = rhsvalues.begin();
		for(; r != rhsvalues.end(); r++)
		{
			DATA::Column *rhs = (*r).first;
			if((*r).second.size() == 1)
			{
				newFuncdeps.push_back((*r).second[0]);
				occupiedColumns[rhs] = true;
			}
			else
			{
				DATA::Funcdep *head_fd = 0;
				DATA::Funcdep *last_fd = (*r).second.back();
			
				std::vector<DATA::Funcdep*>::iterator f = (*r).second.begin();
				for(; f != (*r).second.end(); f++)
				{
					bool good = true;
					std::vector<DATA::Column*>::iterator c = (*f)->lhs_begin();
					for(; c != (*f)->lhs_end(); c++)
						good &= ((occupiedColumns.find(*c) == occupiedColumns.end()) && (rhsvalues.find(*c) == rhsvalues.end()) && ((*c)->isIndependent()));
						
					if(!good)
					{
						if(head_fd == 0)
						{
							head_fd = (*f);
							newFuncdeps.push_back(*f);
							occupiedColumns[rhs] = true;
						}
						else
						{
							std::string tableName, columnName;
							table->getAttribute(DATA::Table::ATTR_NAME, tableName);
							rhs->getAttribute(DATA::Column::ATTR_NAME, columnName);
							std::string error = tableName + ": functional_dep: could not resolve dependencies for \"" + columnName + "\"";
							_conf->setErrorString(error);
							return false;
						}
					}
					else if((head_fd == 0) && ((*f) == last_fd))
					{
						head_fd = (*f);
						newFuncdeps.push_back(*f);
						occupiedColumns[rhs] = true;
					}
					else
					{
						c = (*f)->lhs_begin();
						for(; c != (*f)->lhs_end(); c++)
						{
							occupiedColumns[(*c)] = true;
							DATA::Funcdep *fd = new DATA::Funcdep(false, true);
							fd->addRhsColumn(*c);
							fd->addLhsColumn(rhs);
							newFuncdeps.push_back(fd);
						}
					}
				}
			}
		}
		
		table->setFuncdeps(newFuncdeps);

		if(!table->sortColumns())
		{
			std::string tableName, columnName;
			table->getAttribute(DATA::Table::ATTR_NAME, tableName);
			std::string error = tableName + ": circular functional dependencies";
			_conf->setErrorString(error);
			return false;
		}
		
		return true;
	}
	
	bool Configvalidator::revalidateFuncdeps()
	{
		bool good = true;
		/** ensure dependencies are still valid after processing of mc fkeys **/
		
		std::vector<DATA::Table*>::iterator i = _conf->begin();
		while(i != _conf->end())
		{
			if(!(good &= revalidateTableFuncdeps(*i)))
				break;
			
			i++;
		}		

		return good;	
	}
	
	bool Configvalidator::revalidateTableFuncdeps(DATA::Table *table)
	{
		/** ensure unique rhs **/
		std::map<DATA::Column*, bool> rhsvalues;
		
		std::vector<DATA::Funcdep*>::iterator i = table->funcdeps_begin();
		while(i != table->funcdeps_end())
		{
			if(rhsvalues.find(*((*i)->rhs_begin())) == rhsvalues.end())
				rhsvalues[*((*i)->rhs_begin())] = true;	
			else
			{
				std::string tableName, columnName;
				table->getAttribute(DATA::Table::ATTR_NAME, tableName);
				(*((*i)->rhs_begin()))->getAttribute(DATA::Column::ATTR_NAME, columnName);
				std::string error = tableName + ": functional_dep: rhs column \"" + columnName + "\" member of f-key";
				_conf->setErrorString(error);
				return false;
			}
			i++;
		}		
		
		if(!table->resortColumns())
		{
			std::string tableName, columnName;
			table->getAttribute(DATA::Table::ATTR_NAME, tableName);
			std::string error = tableName + ": conflicting f-keys and functional deps";
			_conf->setErrorString(error);
			return false;
		}
		
		return true;	
	}
	
	void Configvalidator::populateDatatypeWrappers(DATA::Table *table)
	{
		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		for(; i != table->columns_end(); i++)
			(*i)->populateDatatypeWrapper();
	}

	bool Configvalidator::validateColumnKey(DATA::Table *table)
	{
		/** ensure columns have only valid key types **/
		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		while(i != table->columns_end())
		{
			std::string key;
			if((*i)->getAttribute(DATA::Column::ATTR_KEY, key))
			{
				if(key.compare("primary") == 0)
				{
					(*i)->setKey(DATA::Column::KEY_PRIMARY);
					(*i)->setGenerationType(DATA::Column::STORED);
					std::string key_group;
					if((*i)->getAttribute(DATA::Column::ATTR_KEY_GROUP, key_group))
					{
						table->addColumnToPrimaryKeyGroup(key_group, (*i));
					}
				}
				else
				{
					std::string columnName, tableName;
					(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": " + columnName + ": unrecognized key type \"" + key + "\"";
					_conf->setErrorString(error);
					return false;
				}
			}
			else
			{
				(*i)->setKey(DATA::Column::KEY_NONE);
			}

			i++;
		}

		return true;
	}

	bool Configvalidator::validateColumnBasevalue(DATA::Table *table)
	{
		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		while(i != table->columns_end())
		{
			std::string basevalue_str;
			if((*i)->getAttribute(DATA::Column::ATTR_BASEVALUE, basevalue_str))
			{
				if(!(*i)->setBasevalue(basevalue_str))
				{
					std::string columnName, tableName;
					(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": " + columnName + ": invalid basevalue \"" + basevalue_str + "\"";
					_conf->setErrorString(error);
					return false;
				}
			}
			else
			{
				(*i)->seedBasevalue();
			}

			i++;
		}

		return true;
	}

	bool Configvalidator::validateColumnUniqueCount(DATA::Table *table)
	{
		/** ensure columns have only valid unique counts **/
		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		while(i != table->columns_end())
		{
			std::string unique_str;
			if((*i)->getAttribute(DATA::Column::ATTR_UNIQUE, unique_str))
			{
				unsigned long long uniques = HELPER::Strings::ullval(unique_str);
				(*i)->setUniqueValueCount(uniques);				
				(*i)->setGenerationType(DATA::Column::RANGED);
			}
			else
				(*i)->setUniqueValueCount(table->getRowCount());
			i++;
		}

		return true;
	}

	bool Configvalidator::validateForeignKeyConstraints()
	{
		std::vector<DATA::Table*>::iterator i = _conf->begin();
		while(i != _conf->end())
		{
			if(!validateForeignKeyConstraints(*i))
				return false;
			i++;
		}		
	
		return true;
	}

	bool Configvalidator::validateForeignKeyConstraints(DATA::Table *table)
	{
		std::map<DATA::Table*, std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > > > FKDepGraph = std::map<DATA::Table*, std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > > >();
		
		std::vector<DATA::Column*>::iterator i = table->columns_begin();
		while(i != table->columns_end())
		{
			std::string foreignkey_str;
			if((*i)->getAttribute(DATA::Column::ATTR_FOREIGNKEY, foreignkey_str))
			{
				std::string table_name;
				if(HELPER::Strings::popTableName(foreignkey_str, table_name))
				{
					DATA::Table *t = _conf->findTableByName(table_name);
					if(t == 0)
					{
						std::string columnName, tableName;
						(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
						table->getAttribute(DATA::Table::ATTR_NAME, tableName);
						std::string error = tableName + ": " + columnName + ": table not found: \"" + table_name + "\"";
						_conf->setErrorString(error);
						return false;
					}
					DATA::Column *c = t->findColumnByName(foreignkey_str);
					if(c == 0)
					{
						std::string columnName, tableName;
						(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
						table->getAttribute(DATA::Table::ATTR_NAME, tableName);
						std::string error = tableName + ": " + columnName + ": column not found: \"" + foreignkey_str + "\"";
						_conf->setErrorString(error);
						return false;
					}
					std::string key_group;
					if(c->getAttribute(DATA::Column::ATTR_KEY_GROUP, key_group))
					{
						//-> foreign key group element found
						
						int c_dex = c->getDex();
						
						if(c_dex && !(*i)->isIndependent())
						{
							std::string columnName, tableName;
							(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
							table->getAttribute(DATA::Table::ATTR_NAME, tableName);
							std::string error = tableName + ": " + columnName + ": calculated column not referencing primary key head column";
							_conf->setErrorString(error);
							return false;
						}
						// push foreign key group element into lookup vector
						
						if(FKDepGraph.find(t) == FKDepGraph.end())
							FKDepGraph[t] = std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > >();
						if(FKDepGraph[t].find(key_group) == FKDepGraph[t].end())
							FKDepGraph[t][key_group] = std::map<unsigned int, std::vector<DATA::Column*> >();
						if(FKDepGraph[t][key_group].find(c_dex) == FKDepGraph[t][key_group].end())
							FKDepGraph[t][key_group][c_dex] = std::vector<DATA::Column*>();
						FKDepGraph[t][key_group][c_dex].push_back(*i);
						
						std::string base;
						c->getBasevalue(base);
						int c_siz = c->getSiz();
						unsigned long long uniques = c->getUniqueValueCount();
						(*i)->setBasevalue(base);
						(*i)->setUniqueValueCount(uniques / c_siz);
						(*i)->setSiz(c_siz);
						(*i)->setDex(c_dex);
						
						if(!c_dex)
							(*i)->setGenerationType(DATA::Column::RANGED);
					}
					else
					{
						std::string base;
						c->getBasevalue(base);
						unsigned long long uniques = c->getUniqueValueCount();
						(*i)->setBasevalue(base);
						(*i)->setUniqueValueCount(uniques);
						(*i)->setGenerationType(DATA::Column::RANGED);
					}
				}
				else
				{
					std::string columnName, tableName;
					(*i)->getAttribute(DATA::Column::ATTR_NAME, columnName);
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": " + columnName + ": invalid foreign key: \"" + foreignkey_str + "\"";
					_conf->setErrorString(error);
					return false;
				}
			}

			i++;
		}
		
		std::map<DATA::Table*, std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > > >::iterator t = FKDepGraph.begin();
		for(; t != FKDepGraph.end(); t++)
		{
			std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > >::iterator g = (*t).second.begin();
			for(; g != (*t).second.end(); g++)
			{
				if((*g).second.find(0) == (*g).second.end())
				{
					std::string tableName;
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": unreferenced primary key head column in foreign key";
					_conf->setErrorString(error);
					return false;
				}
				
				std::vector<DATA::Column*> lhs_head_cols = (*g).second[0];
				std::string lhs_base;
				DATA::Column *lhs_base_pt = lhs_head_cols[0];
				lhs_base_pt->getAttribute(DATA::Column::ATTR_NAME, lhs_base);
				
				std::string lhs_add = "";
				DATA::Column *lhs_add_pt = 0;
				std::map<unsigned int, std::vector<DATA::Column*> >::iterator d = (*g).second.begin();
				d++;
				for(; d != (*g).second.end(); d++)
				{
					std::string rhs;
					std::string lhs = lhs_base + lhs_add;
					DATA::Column *rhs_pt = 0;
					std::vector<DATA::Column*>::iterator c = (*d).second.begin();
					for(; c != (*d).second.end(); c++)
					{			
						(*c)->getAttribute(DATA::Column::ATTR_NAME, rhs);
						rhs_pt = (*c);
						table->newFuncdep(true, false);
						table->setFuncdepRhs(rhs);
						table->setFuncdepLhs(lhs);
						table->addFuncdepRhsColumn(rhs_pt);
						table->addFuncdepLhsColumn(lhs_base_pt);
						if(lhs_add_pt != 0)
							table->addFuncdepLhsColumn(lhs_add_pt);
					}
					
					lhs_add = ",";
					lhs_add += rhs;
					lhs_add_pt = rhs_pt;
				}
			}
		}
		return true;
	}
	
	bool Configvalidator::validateHardenedFds()
	{
		std::vector<DATA::Table*>::iterator i = _conf->begin();
		while(i != _conf->end())
		{
			if(!validateHardenedFds(*i))
				return false;
			i++;
		}		
	
		return true;
	}
	
	bool Configvalidator::validateHardenedFds(DATA::Table *table)
	{
		if(!table->funcdeps_size())
			return true;
			
		unsigned int rowsToHarden = 0;
		
		std::map<DATA::Column*, bool> usedCols = std::map<DATA::Column*, bool>();
	
		/** search for interferences **/
		std::vector<DATA::Funcdep*>::iterator i = table->funcdeps_begin();
		for(; i != table->funcdeps_end(); i++)
		{
			rowsToHarden += (*i)->lhs_size();
			std::vector<DATA::Column*>::iterator c = (*i)->lhs_begin();
			for(; c != (*i)->lhs_end(); c++)
			{
				if(usedCols.find(*c) != usedCols.end())
				{
					std::string tableName;
					table->getAttribute(DATA::Table::ATTR_NAME, tableName);
					std::string error = tableName + ": hardened fds not available for interferencing functional dependencies";
					_conf->setErrorString(error);
					return false;
				}
				usedCols[*c] = true;
			}
			if(usedCols.find(*((*i)->rhs_begin())) != usedCols.end())
			{
				std::string tableName;
				table->getAttribute(DATA::Table::ATTR_NAME, tableName);
				std::string error = tableName + ": hardened fds not available for interferencing functional dependencies";
				_conf->setErrorString(error);
				return false;
			}
			usedCols[*((*i)->rhs_begin())] = true;
		}
		
		/** check for sufficient table rows **/ 
		if(table->getRowCount() <= rowsToHarden * 2)
		{
			std::string tableName;
			table->getAttribute(DATA::Table::ATTR_NAME, tableName);
			std::string error = tableName + ": insufficient row count for hardening functional dependencies";
			_conf->setErrorString(error);
			return false;
		}
		table->setRowCount(table->getRowCount() - rowsToHarden * 2);
		table->setRowsToHarden(rowsToHarden);
		
		return true;
	}

} // namespaces
