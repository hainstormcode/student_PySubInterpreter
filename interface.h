#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H
#include <vector>
using namespace std;



typedef vector<string> programType;

class Interface
{
	public:
		void startInterface();
		void helpUtility();
		void quit();
		void help();
		void read(string);
		void show();
		void clear();
		void exitHelp();
		void quitHelp();
		void helpHelp();
		void readHelp();
		void showHelp();
		void clearHelp();
		void commandsHelp();
		void setColor(int);
		void resetColor();
		void mintOn();
		void mint2On();
	private:
		programType programCode;
};



#endif