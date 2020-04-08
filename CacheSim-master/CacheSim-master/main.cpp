#include "base.h"

int main()
{
	char ch = '\0';
	std::ifstream in("input");
	std::streambuf *cinbuf = std::cin.rdbuf();
	std::cin.rdbuf(in.rdbuf());

	
		InitVariables();
		GetInput(); // get input information
		CalcInfo();
		CreateCache();
		FileTest();
		PrintOutput(); // output the result
		//DestroyCache();
		std::cout << "Continue/Exit(C/E)" << std::endl;
		std::cin >> ch;

	return 0;
}
