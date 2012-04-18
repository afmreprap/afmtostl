#include "converter.h"

format::format() {}

bool format::isEqual(double a, double b) {
	return fabs(a-b) <= .000001;
}


vertex format::formatLine(string line) {
	int size = line.length();
	vertex v;
	
	//there is a ; after each number, so substr to that position, and then turn the number into normal notation from scientific
	size_t pos1 = line.find(";");
	string doubleX = line.substr(0, pos1);
	
	v.x = formatNum(doubleX, false);
	
	//same, but substr after the first ; to the second
	size_t pos2 = line.rfind(";");
	string doubleY = line.substr(pos1+1, pos2-pos1-1);
	v.y = formatNum(doubleY, false);
	
	//same, substr after second ; to end
	string doubleZ = line.substr(pos2+1);
	v.z = formatNum(doubleZ, true);

	return v;
}



//formats the number from scientific notation is string, because z axis have a slightly different scaling on average, check if
//the input is a z and format differently
double format::formatNum(string num, bool isZ) {
	
	//find the position of e, if there is no e, turn the number into  double and output it
	size_t pos = num.find("e");
	double x;
	double multiplier;
	size_t comp = -1;
	if (pos == comp) {
		x = atof(num.c_str());
		return x;
	}
	
	//otherwise, the normal part of the number comes before e, so turn x into that,
	//and find the power of ten to multiply by(everything after e)
	string n = num.substr(0, pos);
	string mult = num.substr(pos+1);
    x = atof(n.c_str());
	multiplier = atof(mult.c_str());	
	
	//and then return the x and y values scaled by 10^7, and the z values scaled by 10^8
	//a larger isZ addition gives more "spread" on the z axis, as does a larger x,y addition
	if (isZ) {
		//x = x*pow(10, multiplier+10.5);
		//x = x*pow(10, multiplier+7.5);
		x = x*pow(10, multiplier);
		return x;
	}
	
	//x = x*pow(10, multiplier+10);
	//x = x*pow(10, multiplier+7);
	x = x*pow(10, multiplier);
	return x;
}

//reads the data from inID to outID using the x,y scaling value xymult, the z scaling value zmult, and the height addition add
void readOut(string inID, string outID, double xymult, double zmult, double maxxy, double add) {
}


int main() {
	format a;
	ifstream data;
	ofstream out;
	
	//various constants
	int complete = 0;
	double xymult;
	double zmult;
	double add;
	double xyscale;
	double height;
	
	string in_id;
	string out_id;
		
	//this allows the user to determine which file to run the program on (defualts for now)
	cout << "Enter the filename to be read" << endl;
	cin >> in_id;
	
	
	//enter the output file
	cout << "Enter the filename to be read to" << endl;
	cin >> out_id;

	/*
	cout << "enter x and y multiplier" << endl;
	cin >> xymult;
	
	cout << "enter z multiplier";
	cin >> zmult;
	*/
	 
	
	if(data.fail() || out.fail()) {
		cerr << "one of the files didn't open" << endl;
	}
		
	int n = 0;
	int m = 0;
	cout << "enter the dimensions (n x m) which to cut up the piece" << endl;
	cin >> n >> m;
	
	//opens the file determined by id
	data.open(in_id.c_str());
	
	//this gets the maximum xy and z values in the data to decide the scaling factor
	//and the minimum z value to add or subtract a little bit so the object has a base	
	
	double maxxy = 0;
	double maxz = 0;
	double minz = 1000000000;
		
	//all sorts of constants necessary for processing the file
	vertex temp;
	int count = 1;
	string in;
	
	while (data >> in) {
		temp = a.formatLine(in);
		
		if(temp.z > maxz){
			maxz = temp.z;
		}
		
		if (temp.y > maxxy) {
			maxxy = temp.y;
		}
		
		if (temp.y > maxxy) {
			maxxy = temp.y;
		}
		
		if (temp.z < minz) {
			minz = temp.z;
		}
	}
	
	//this resets the file	
	data.close();
	
	//the minimum should always be positive because the add should be positive, and the minimum~add
	if (minz < 0) {
		minz = -1*minz;
	}
	
	xymult = xyscale/maxxy;	
	zmult = height/maxz;
	
	//this may need to be larger to allow the poisson surface reconstruction to work
	add = zmult*minz+height/12;
	

	//get the xyscale and the hieght
	while (complete == 0) {
		cout << "enter the x and y scale (in mms or the number that should be in the .stl file, depending on what units that should be)" << endl;
		
		cin >> xyscale;
		
		//this is assuming a 1 in the .stl file converts to a mm.  I really don't 
		cout << "enter the height (in mms or the number that should be in the .stl file, depending on what units that should be)" << endl;
		cin >> height;
		
		//ignore this
		if (a.isEqual(0, height)) {
		
		}
		
		
		//these are the multipliers that need to be multiplied to get the scale
		xymult = xyscale/maxxy;	
		zmult = height/maxz;
		
		//this may need to be larger to allow the poisson surface reconstruction to work
		add = zmult*minz+height/12;		
		
		//this output the data
		//I tried to do this in a void and there was some weird bug, so it's stuck here now.
		ifstream data;
		ofstream out;
		
		//reopen both files
		data.open(in_id.c_str());
		out.open(out_id.c_str());
		
		//all sorts of constants necessary for processing the file
		vertex temp;
		int count = 1;
		string in;
		
		//while there is still data to input, format the output into the apropriate style (the data is in scientific notation, so convert to int)
		//, then output it in the obj format, v v.y v.y v.z
		while(data >> in) {
			temp = a.formatLine(in);
			double x = temp.x*xymult;
			double y = temp.y*xymult;
			double z = add+temp.z*zmult;
			out << "v" << "  " << x << "    " << y << "    " << z << endl;
			if(a.isEqual(temp.x, 0) || a.isEqual(x, xymult*maxxy)  || a.isEqual(temp.y, 0) || a.isEqual(y, xymult*maxxy)) {
				for(int i = 1; i < z; i++) {
					out << "v" << "  " << x << "    " << y << "    " << i << endl;
				}
			}
			
			out << "v" << "  " << x << "    " << y << "    " << 0 << endl;
			count++;
		}
		data.close();
		out.close();
		
		
		
		
		
		
		
		
		cout << "Run again with different xyscales/heights? (0 if yes, 1 if no)" << endl;
		cin >> complete;
	}
	
		
}