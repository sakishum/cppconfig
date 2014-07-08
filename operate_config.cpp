/**
 * @file operate_config.cpp
 * @Synopsis  Generic configuration Class.
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2014-05-06
 */
#include "./operate_config.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>

namespace operatorconfig {
CConfig::CConfig(void) : m_Delimiter(std::string(1, '=')), m_Comment(std::string(1, '#')) {
	// Construct a Config without a file;
}

CConfig::CConfig(const std::string &filename, const std::string &delimiter, const std::string &comment) 
: m_Delimiter(delimiter), m_Comment(comment) {
	std::ifstream in(filename.c_str()); 
	if (!in) {
		printf("Couldn\'t open this file!\n");
		throw File_not_found(filename); 
	}
	in >> (*this);	// save in memory by operator>>
	in.close();		// auto release 
}

void CConfig::init(const std::string &filename, const std::string &delimiter, const std::string &comment) {
	setDelimiter(delimiter);
	setComment(comment);
	std::ifstream in(filename.c_str()); 
	if (!in) {
		printf("Couldn\'t open this file!\n");
		throw File_not_found(filename); 
	}
	in >> (*this);	// save in memory by operator>>
	in.close();		// auto release 
}

bool CConfig::keyExists(const std::string &key) const {
	// Indicate wherher key is found
	mapci p = m_Contents.find(key);
	return (p != m_Contents.end());
}

/* Static */
void CConfig::Trim(std::string &inout_s) {
	// Remove leading and trailind whitespace
	static const char whitespace[] = " \n\t\v\r\f";
	inout_s.erase(0, inout_s.find_first_not_of(whitespace));
	inout_s.erase(inout_s.find_last_not_of(whitespace) + 1U);
}

void CConfig::remove(const std::string &key) {
	// Remove key and its value
	m_Contents.erase(m_Contents.find(key));
	return;
}

bool CConfig::fileExist(std::string &filename) {
	bool exist = false;
	std::ifstream in(filename.c_str());
	if (in) {
		exist = true;
		//in.close();	// auto release
	}
	return exist;
}

void CConfig::readFile(std::string &filname, std::string delimiter, std::string comment) {
	m_Delimiter = delimiter;
	m_Comment = comment;
	std::ifstream in(filname.c_str());
	if (!in) throw File_not_found(filname);
	in >> (*this);
	in.close();			// auto release 
}

std::ostream& operator<<(std::ostream &os, const CConfig  &cf) {
	// Save a Config to os
	for (CConfig::mapci p = cf.m_Contents.begin(); p != cf.m_Contents.end(); ++p) {
		os << p->first << " " << cf.m_Delimiter << " " << p->second << std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream &is, CConfig& cf) {
	// Load a Config from is
	// Read in keys and values, kepping internal whitespace
	typedef std::string::size_type pos;
	const std::string &delim = cf.m_Delimiter;	// sparator
	const std::string &comm = cf.m_Comment;		// comment
	const pos skip = delim.length();			// length of sparator
	
	bool terminate = false;
	pos delimPos = 0;
	std::string key("");
	std::string line("");
	std::string nlcopy("");
	std::string nextline("");	// might need to read ahead to see where value ends

	while (is || nextline.length() > 0) {
		// Read an entrie line at a time
		line = "";
		if (nextline.length() > 0) {
			line = nextline;	// we read ahead; use it now
			nextline = "";
		} else {
			std::getline(is, line);
		}

		// Ignore comments ('#')
		line = line.substr(0, line.find(comm));

		// Parse the line if it contains a delimiter ('=')
		delimPos = line.find(delim);
		if (delimPos < std::string::npos) {
			// Extract the key
			key = line.substr(0, delimPos);
			line.replace(0, delimPos+skip, "");

			// See if value continues on the next line
			// Stop at blank line, next line with a key, end of stream,
			// or end of file sentry
			terminate = false;
			while (!terminate && is) {
				std::getline(is, nextline);
				terminate = true;

				nlcopy = nextline;
				CConfig::Trim(nlcopy);
				if (nlcopy == "") {
					continue;
				}

				nextline = nextline.substr(0, nextline.find(comm));
				if (nextline.find(delim) != std::string::npos) {
					continue;
				}

				nlcopy = nextline;
				CConfig::Trim(nlcopy);
				if (nlcopy != "") {
					line += "\n";
				}

				line += nextline;
				terminate = false;
			}

			// Store key and value 
			CConfig::Trim(key);
			CConfig::Trim(line);
			cf.m_Contents[key] = line;	// overwrites if key is repeated
		}
	}
	return is;
}

}  // namespace operatorconfig
