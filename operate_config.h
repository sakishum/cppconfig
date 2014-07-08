/**
 * @file operate_config.h
 * @Synopsis  Generic configuration Class.
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2014-05-06
 */
#ifndef OPERATOR_CONFIG_H__
#define OPERATOR_CONFIG_H__

#include <iostream>
#include <sstream>
#include <string>
#include <map>

namespace operatorconfig {
// Exception types
// 尽量不要将一个嵌套类声明为public，把嵌套类置于名字空间中是更好的方式。
class File_not_found {
public:
	std::string filename;
	File_not_found(const std::string &filename_ = std::string()) : filename(filename_) { }
};

class Key_not_found {
public:
	std::string key;
	Key_not_found(const std::string &key_ = std::string()) : key(key_) { }
};

class CConfig {
	// Method
public:
	CConfig(void);
	CConfig(const std::string &filename, const std::string &delimiter = "=", const std::string &comment = "#");

	void init(const std::string &filename, const std::string &delimiter = "=", const std::string &comment = "#");
	template<typename T> T read(const std::string &in_key) const;	//<! Searchfor key and read value or optional default value, call as read<T> 
	template<typename T> T read(const std::string &in_key, const T &in_value) const;
	template<typename T> bool readInto(T &out_var, const std::string &in_key) const;
	template<typename T> bool readInto(T &out_var, const std::string &in_key, const T &in_value) const;
	bool fileExist(std::string &filename);
	void readFile(std::string &filename, std::string delimiter="=", std::string comment = "#");
	// Check whether key exists in configuration
	bool keyExists(const std::string &in_key) const;

	// Modify keys and values
	template<class T> void add(const std::string &in_key, const T& in_value);
	void remove(const std::string &in_key);

	// Check or change configuration syntax
	std::string getDelimiter(void) const { return m_Delimiter; }
	std::string getComment(void) const { return m_Comment; }
	std::string setDelimiter(const std::string &in_s) { 
		std::string old = m_Delimiter; m_Delimiter = in_s; return old;
	}

	std::string setComment(const std::string &in_s) {
		std::string old = m_Comment; m_Comment = in_s; return old;
	}

	// Write or read configuration 
	friend std::ostream& operator<<(std::ostream& os, const CConfig& cf);
	friend std::istream& operator>>(std::istream& is, CConfig& cf);

	// Data
protected:
	std::string m_Delimiter;	//<! separator between key and value 
	std::string m_Comment;		//<! separator between value and comments
	std::map<std::string, std::string> m_Contents;	//!<extracted keys and values 

	typedef std::map<std::string,std::string>::iterator mapi;
	typedef std::map<std::string,std::string>::const_iterator mapci;

	template<typename T> static std::string T_as_string(const T &t);
	template<typename T> static T string_as_T(const std::string &s);
	static void Trim(std::string &inout_s);
};  // class CConfig

/* Static */
template<typename T>
std::string CConfig::T_as_string(const T &t) {
	// Conver from a T to a string
	// Type T must support << operator
	std::ostringstream ost;
	ost << t;
	return ost.str();
}

/* Static */
template<typename T>
T CConfig::string_as_T(const std::string &s) {
	// Convert from a string to a T
	// Type T must support >> operator
	T t;
	std::istringstream ist(s);
	ist >> t;
	return t;
}

/* Static */
template<>
inline std::string CConfig::string_as_T<std::string>(const std::string &s) {
	// Conver from a T to a string
	// In other words, do nothing
	return s;
}

/* Static */
template<> 
inline bool CConfig::string_as_T<bool>(const std::string& s) {
	// Convert from a string to a bool
	// Interpret "false", "F", "no", "n", "0" as false 
	// Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
	bool b = true;
	std::string sup = s;
	for (std::string::iterator p = sup.begin(); p != sup.end(); ++p) {
		*p = toupper(*p); // make string all caps
	}
	if (sup == std::string("FALSE") || sup == std::string("F")
			|| sup == std::string("NO") || sup == std::string("N")
			|| sup == std::string("0") || sup == std::string("NONE")) {
		b = false;
	}
	return b;
}

template<typename T>
T CConfig::read(const std::string& key) const {
	// Read the value corresponding to key
	mapci p = m_Contents.find(key);
	if (p == m_Contents.end()) throw Key_not_found(key);
	return string_as_T<T>( p->second);
}

template<typename T>
T CConfig::read(const std::string &key, const T &value) const {
	// Return the value corresponding to key or given default value
	// if key is not found 
	mapci p = m_Contents.find(key);
	if (p == m_Contents.end()) {
		printf("Not fount\n");
		return value;
	}
	return string_as_T<T>( p->second);
}

template<typename T>
bool CConfig::readInto(T &var, const std::string& key) const {
	// Get the value corresponding to key and store in var  
	// Return true if key is found  
	// Otherwise leave var untouched  
	mapci p = m_Contents.find(key);
	bool found = (p != m_Contents.end());
	if (found) {
		var = string_as_T<T>( p->second );
	}
	return found;
}

template<typename T> 
bool CConfig::readInto(T &var, const std::string &key, const T &value) const {
	// Get the value corresponding to key and store in var 
	// Return true if key is found
	// Otherwise set var to given default
	mapci p = m_Contents.find(key);
	bool found = (p != m_Contents.end());
	if (found) {
		var = string_as_T<T>( p->second );
	} else {
		var = value;
	}
	return found;
}

template<typename T>
void CConfig::add(const std::string &in_key, const T &value) {
	// Add a key with given value  
	std::string v = T_as_string(value);
	std::string key = in_key;
	Trim(key);
	Trim(v);
	m_Contents[key] = v;
	return;
}

}  // namespace operatorconfig
#endif  // OPERATOR_CONFIG_H__

