// learn_languages.cpp : définit le point d'entrée de l'application.
//


#include "learn_languages.h"


using namespace std;


int main()
{
	
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); 
	SetConsoleCP(CP_UTF8);   
#endif


	Parser parser("../../../../langues2.txt");

	Manager manager(parser);
	manager.start();

	return 0;
}
