#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <iomanip>

#define YELLOW  "\033[33m"    // Yellow color
#define BLUE    "\033[34m"    // DeepBlue color
#define RESET   "\033[0m"     // Reset to default color

using namespace std;

Menu::Menu() {
	header = "";
	footer = "Please select option\n";
	bullet = "--";

}
int Menu::prompt() {
	int selected = -1;
	do
	{
		system("cls"); // clear ur console
		cout <<"\t\t\t"<<YELLOW"========================================================================================================================================\t\t\t"<<RESET<<endl;
		cout << setw(84)<<header << endl;
		for (int i = 0; i < options.size(); i++) {//loop trhough each option 
			cout <<setw(84)<<"[" <<i + 1<< "]"  << bullet << options[i].first;
			if (options[i].second != "") {
				cout << " : " << options[i].second;
			}
			cout << endl;
		}
		cout << "\t\t\t"<<YELLOW"========================================================================================================================================\t\t\t"<<RESET<<endl;
		cout <<"\t\t\t" << footer << endl;
		
		selected = _getch() - '0';
	} while (selected < 1 || selected > options.size());
	return selected;
}
void Menu::addOption(string option) {
	options.push_back({ option,"" });
}
void Menu::setValue(int index, string value) {
	if (index >= 0 && index < options.size()) {
		options[index].second = value;
	}
}
Menu::~Menu() {
	options.clear();
}