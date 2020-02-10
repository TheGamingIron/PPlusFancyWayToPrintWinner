#include <iostream>
#include <fstream>
#include <string>
#include <vector>
bool IsStandard(std::string theString);
//Note : This method uses my own 'compiler/reader',not sure but this is my first time writing it : ) and I had lots of fun
//This 'compiler' reads line by line and is designed specifically to output string based on a dummy
//function called void ToString(T obj) which is intended to be so 

//This method not only fulfills the conditions set by the contest, it also allows custom 
//data classes For instance class Node to be output as a String as "Node" which resolves operator << not found issue
//and also for Enum
//For instance  
//enum TestEnum {
//First,
//Second
//};      

//Calling ToString(TestEnum::First) This will output "TestEnum::First"


//The problems that I acknowledge is since the compiler reads line by line,
//any strange indentation will throw the reader off.For Instance declaring classes in such a way :
// class
// Node {  

// };

//It wont find the class 'Node'

//Future Improvements : Reading from multiple files instead of just the main.cpp,considered expanding
//upon it but restricted by time

class Node {
public:
	int x, y;
};

enum TestEnum {
	First,
	Second
};

//This vector is to satisfy the contest rules , storing my output here
std::vector<std::string> outputString;

//Standard Types for my own compiler to identify and read accordingly
std::vector<std::string> StandardTypes = { "bool","int","double","float","long","char","void","std::string" };
//This stores the ToString("...") the parameter to be passed in to be converted to String()
std::vector<std::string> foundPrintName;


//This Function is just a dummy function for my own compiler to read
template <class T>
void ToString(T obj){	
	
	
}							

//This function is to check for a specific string in the file (Reader)
bool ContainString(std::string theString,std::string entireString,bool& checkFunction,bool specific) {
	std::string addedString,beforeString;
	
	int index = 0, curIndex = 0;
	
	while (true) {
		
		if (!isspace(entireString[index]) ) {
			addedString += entireString[index];			
			if (addedString == "//") {
				return false;
			}
			if (!specific) {
				//Search for all variations of the string without considering the entire string
				//Because the parameters will never be the same Like in ToString(x) , whatever we pass in
				//x can never be found and thus the function will never be found
				if (addedString[curIndex] == theString[curIndex]) {
					curIndex++;
					if (curIndex >= theString.size()) {

						return true;
					}
				}
			}
			
		}
		else {
			//Adding a custom class to the standard type vector
			if (beforeString == "class") {
				StandardTypes.push_back(addedString);
			}
			beforeString = addedString;
			if (specific) {
				if (addedString == theString) {
					return true;
				}
			}
			//This is to check the lines from functions , ifs and variables
			if (IsStandard(beforeString)) {
				checkFunction = true;
			}
			else {
				checkFunction = false;
			}
			addedString = "";
			curIndex = 0;

		}

		index++;

		if (index >= entireString.size()) {
			break;
		}
		
	}
	

	return false;
}



bool IsStandard(std::string theString) {
	bool ref = false;
	for (int i = 0; i < StandardTypes.size(); i++) {

		if (StandardTypes[i] == theString) {
			return true;
		}
	}

	return false;
}


Node a = Node();
int checkInt = 10;
std::string cStuff = " ";
long thisLong = 8;
long long extraLong = 5;
double m_Decimal = 20.0;

int main() {

	ToString(a);  // output = "Node"
	ToString(checkInt); // output = "int"
	ToString(cStuff); //output = "std::string"
	ToString(thisLong); // output = "long"
	ToString(extraLong); // output = "long long"
	ToString(m_Decimal); // output = "double"
	ToString(TestEnum::First);   // output = same as input 
	ToString("thisStringIsFun"); // output = same as input 
	ToString(true);  // output = same as input 
	ToString(231); // output = same as input 
	ToString(1.5); // output = same as input 


	//Find The ToString Dummy funtion
	//Get the variable name(the parameters being passed in for instance ToString(a) a is the name)
	//Find that name in the main.cpp
	//Look at the type before the name 
	//Print out that type if it is in the standard type
	//Else print it out normally


	std::fstream mainFile("main.cpp");
	if (mainFile.is_open()) {
		std::string curString, sString;
		int index = 0;

		while (!mainFile.eof()) {
			std::getline(mainFile, curString);

			bool isCallFunction = false;
			if (ContainString("ToString", curString, isCallFunction,false)) {

				if (!isCallFunction) {
							
					int s = 0;

					while (true) {
						if (curString[s] == '(')
						{
							s++; while (curString[s] != ')') {
								sString += curString[s]; s++;
							}
							break;
						}
						s++;
					}

					foundPrintName.push_back(sString);
					sString = "";



				}
			}
			
		}
		mainFile.seekg(0, std::ios::beg);
		std::vector<bool> printedOut = std::vector<bool>(foundPrintName.size());
		while (!mainFile.eof()) {
			std::getline(mainFile, curString);
			bool isType = false;
	
			for (int i = 0; i < foundPrintName.size(); i++) {
				if (ContainString(foundPrintName[i], curString, isType,true)) {
					if (isType) {

						std::string givenType;
						int s = 0;

						while (curString[s] != foundPrintName[i][0]) {
							if (!isspace(curString[s])) {
								givenType += curString[s];

							}
							s++;
						}
						printedOut[i] = true;
						outputString.push_back(givenType);
					
						

					}

				}
			
			}



		}

		for (int i = 0; i < foundPrintName.size(); i++) {
			if (!printedOut[i]) {
				outputString.push_back(foundPrintName[i]);
				
			}
		}

	}
	int curIterator = 0;
	do {
		std::cout << outputString[curIterator] << std::endl;
	} while (++curIterator < outputString.size());



	
	std::cin.get();
	return 0; 
}