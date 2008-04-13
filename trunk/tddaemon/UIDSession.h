#ifndef UIDSESSION_H_
#define UIDSESSION_H_

#include <iostream>
#include <ctime> 
#include <cstdlib>

using namespace std;

class UIDSession
{
public:
	UIDSession();
	virtual ~UIDSession();
	static string getID();
	static void initRand();
};

#endif /*UIDSESSION_H_*/
