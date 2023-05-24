#include "studentasLib.h"
#include "studentas.h"
#include "vektorius.h"
#include <vector>

int main()
{

	// Pradeti v1 užpildymo laiko matavima
	
	auto v1Start = system_clock::now();
	
	int sz = 100000;
	std::vector<int> v1;
	for (int i = 1; i <= sz; ++i)
	  v1.push_back(i);
	// Baigti v1 užpildymo laiko matavima
	
	auto v1End = system_clock::now();
	auto v1Elapsed = duration_cast<milliseconds>(v1End - v1Start);
	cout<<endl<<"~std::vector<int> laikas: "<<v1Elapsed.count()<<"ms."<<endl<<endl;
	
	// Pradeti v2 užpildymo laiko matavima
	auto v2Start = system_clock::now();
	
	vektorius<int> v2;
	for (int i = 1; i <= sz; ++i)
	  v2.push_back(i);
	// Baigti v2 užpildymo laiko matavima
	
	auto v2End = system_clock::now();
	auto v2Elapsed = duration_cast<milliseconds>(v2End - v2Start);
	cout<<endl<<"~vektorius<int> laikas: "<<v2Elapsed.count()<<"ms."<<endl<<endl;




	studentas temp;

	// Pradeti v3 užpildymo laiko matavima
	
	auto v3Start = system_clock::now();

	std::vector<studentas> v3;
	for (int i = 1; i <= sz; ++i)
	  v3.push_back(temp);
	// Baigti v3 užpildymo laiko matavima
	
	auto v3End = system_clock::now();
	auto v3Elapsed = duration_cast<milliseconds>(v3End - v3Start);
	cout<<endl<<"~std::vector<studentas> laikas: "<<v3Elapsed.count()<<"ms."<<endl<<endl;
	
	// Pradeti v4 užpildymo laiko matavima
	auto v4Start = system_clock::now();
	
	vektorius<studentas> v4;
	for (int i = 1; i <= sz; ++i)
	  v4.push_back(temp);
	// Baigti v4 užpildymo laiko matavima
	
	auto v4End = system_clock::now();
	auto v4Elapsed = duration_cast<milliseconds>(v4End - v4Start);
	cout<<endl<<"~vektorius<studentas> laikas: "<<v4Elapsed.count()<<"ms."<<endl<<endl;

	
}
