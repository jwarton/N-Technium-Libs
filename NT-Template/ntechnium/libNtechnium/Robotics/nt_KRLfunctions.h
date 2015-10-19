///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// ntKRLfunctions.h
// KUKA Robot Language function writer			///////////////////
// wrties instructions to list					///////////////////
// created by James Warton on 05/24/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef KUKA_KRL_INSTRUCTION_WRITER_JPW_NTECHNIUM
#define KUKA_KRL_INSTRUCTION_WRITER_JPW_NTECHNIUM

#include <iostream>
#include <vector>

using namespace std;

class ntKRLfunction;
typedef ntKRLfunction KRLfunction;

class ntKRLfunction{
private:
	string instruction;

public:

	ntKRLfunction();
	void init();

};
#endif

///SET THIS UP AS BASE CLASS WITH ALL GENERIC FUNCTIONS
///ADDITIONAL WORKCELL SPECIFIC FUNCTIONS INHERIT FROM THIS CLASS