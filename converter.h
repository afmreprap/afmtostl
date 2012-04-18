#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>


using namespace std;

struct vertex {
	double x;
	double y;
	double z;
};

class format{
	public:
	bool isEqual(double a, double b);
	format();
	vertex formatLine(string line);
	double formatNum(string num, bool isZ);
	void readOut(string inID, string outID, double xymult, double zmult, double maxxy, double add); 
	private:
	
};