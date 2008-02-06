#include "regExp.h"

regExp::regExp(string value,string & p) {
	reg = new regex(value);
	reg_iterator = new boost::sregex_iterator(p.begin(),p.end(),*reg);
}

regExp::regExp(string value) {
	reg = new regex(value);
	reg_iterator = NULL;
}

regExp::~regExp() {
	delete reg;
	delete reg_iterator;
}

void regExp::newPage(string & p) {
  reg_iterator = new boost::sregex_iterator(p.begin(),p.end(),*reg);
}

void regExp::next() {
	++(*reg_iterator);
}

bool regExp::endOfMatch() {
	return *reg_iterator == boost::sregex_iterator();
}

string regExp::getMatch(int i) {
	smatch what = **reg_iterator;
	//Here's a non safe thread function (Boost 1.33.1)
	return (string) what[i];
}

int regExp::getMatchNumber() {
	smatch what = **reg_iterator;
	return what.size();
}

string regExp::replaceAll(string s, string search, string replace) {
	regex regsearch(search);
	return regex_replace(s,regsearch,replace);
}
