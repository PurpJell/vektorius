#include "studentasLib.h"
#include "studentas.h"
#include "vektorius.h"

using namespace std;

// STUDENTO FUNKCIJOS

//! studento konstruktorius, paduodant jam eilutę ir pažymių skaičių
studentas::studentas(string line, int pazCount){

	istringstream iss(line);

	string word; // laikinas string'as nuskaitymui
	iss >> word;
	setSurname(word);
	iss >> word;
	setName(word);

	int paz; // laikinas int'as nuskaitymui

	for (int i = 0; i < pazCount; i++)
	{
		iss >> paz;
		pushPaz(paz);
	}
	iss >> paz;
	pushEgz(paz);

	setVid();
	setMed();
}

//! apskaičiuoja studento pažymių vidurkį
void studentas::setVid() {

	for (int i = 0; i < paz_.size(); i++)
	{
		vid_ += paz_.at(i);
	}

	vid_ /= paz_.size();
}

//! apskaičiuoja studento pažymių medianą
void studentas::setMed() {

	sort(paz_.begin(), paz_.begin() + paz_.size());
	if (paz_.size() % 2 == 0)
	{
		med_ = (1.0 * paz_.at(paz_.size() / 2 - 1) + 1.0 * paz_.at(paz_.size() / 2)) / 2;
	}
	else med_ = 1.0 * paz_.at(paz_.size() / 2);
}

//! užpildo studentų vektorių, pasirenkant studentų ir pažymių skaičių 
void pildyti(vektorius<studentas>& mas, int& pazCount) {

	cout << "Kiek pazymiu irasyti kiekvienam studentui?" << endl;
	cin >> pazCount;
	cout << "Kiek studentu generuoti?" << endl;
	int stCount;
	cin >> stCount;

	studentas temp;
	int paz;

	for (int i = 0; i < stCount; i++)
	{

		temp = studentas();

		for (int j = 0; j < 6; j++)
		{
			temp.setName(temp.vardas() + (char)(rand() % 10 + 97));
			temp.setSurname(temp.pavarde() + (char)(rand() % 10 + 97));
		}

		for (int j = 0; j < pazCount; j++)
		{
			paz = rand() % 11;
			temp.pushPaz(paz);
		}

		paz = rand() % 11;
		temp.setMed();
		temp.setVid();
		temp.pushEgz(paz);

		mas.push_back(temp);

	}
}

//! sugeneruoja studentų failą pagal pasirinktą kiekį studentų 
void generuoti(vektorius<studentas>& mas, int& stCount, int& pazNr)
{
	cout << "Irasykite, kiek irasu generuoti 1k (1), 10k (2), 100k (3), 1M (4), 10M (5)" << endl;
	int records = 1;

	cin >> records;

	switch (records) {
	case 1:
		records = 1000;
		break;
	case 2:
		records = 10000;
		break;
	case 3:
		records = 100000;
		break;
	case 4:
		records = 1000000;
		break;
	case 5:
		records = 10000000;
		break;
	default:
		cout << "Neteisinga ivestis" << endl;
		return;
	}

	vektorius<int> skaic;

	for (int i = 1; i <= records; i++)
	{
		skaic.push_back(i);
	}

	random_shuffle(skaic.begin(), skaic.end());

	cout << "Irasykite, kiek pazymiu generuoti kiekvienam studentui" << endl;
	cin >> pazNr;

	string tempSt = "";

	for (int i = 0; i < pazNr; i++)
	{
		tempSt += "paz" + to_string(i + 1) + " ";
	}
	tempSt += " egz";

	string failas = "klase" + to_string(records) + ".txt";
	ofstream write(failas);

	write << left << setw(20) << "Pavarde" << setw(15) << "Vardas " << tempSt << endl;

	studentas temp;
	int paz;
	int sk;

	for (int i = 0; i < records; i++)
	{
		sk = skaic.at(skaic.size() - 1);
		skaic.pop_back();

		temp.setName("vardas" + to_string(sk));
		temp.setSurname("pavarde" + to_string(sk));
		tempSt = "";

		for (int j = 0; j < pazNr; j++)
		{
			paz = rand() % 11;
			if (paz == 10) tempSt += to_string(paz) + "   ";
			else tempSt += " " + to_string(paz) + "   ";
			temp.pushPaz(paz);
		}
		paz = rand() % 11;
		temp.pushEgz(paz);
		if (paz == 10) tempSt += to_string(paz) + "\n";
		else tempSt += " " + to_string(paz) + "\n";

		write << left << setw(20) << temp.pavarde() << setw(15) << temp.vardas() << setw(3) << tempSt;
	}
}

//! nuskaito studentus į vektorių iš pasirinkto failo 
void nuskaityti(vektorius<studentas> &mas, int& stCount, int& pazCount) {

	string fileToRead;
	cout << "Iveskite failo pavadinima: ";
	cin >> fileToRead;
	//fileToRead = "klase1000000.txt";

	ifstream read(fileToRead);

	if (!read)
	{
		throw "Failas nerastas";
	}

	auto readStart = system_clock::now();

	string input;

	getline(read, input); // praleidzia pirma eilute

	pazCount = -3; // offset by 3 for vardas, pavarde and egz

	istringstream iss(input);
	string word;
	while (iss >> word) {
		pazCount++;
	}

	while (!read.eof())
	{
		getline(read, input);
		studentas temp(input, pazCount);

		if (int(input[0]) != 0)
		{
			mas.push_back(temp);
			stCount++;
		}
	}

	auto readEnd = system_clock::now();

	auto readElapsed =
		duration_cast<milliseconds>(readEnd - readStart);
	cout << endl << "~Nuskaitymo laikas: " << readElapsed.count() << "ms." << endl << endl;
}

//! funkcija, nurodanti, kaip rūšiuoti studentus pagal vardą 
bool daugiau(string s1, string s2)
{
	return s1 > s2;
}

//! suskirsto studentų vektorių pagal vardą ir gražina skirstymo indeksą, kai vardai nėra šabloniniai (vardas100) 
int partition(vektorius<studentas>& mas, int start, int end) // kai vardai nera sabloniniai
{
	string pivot = mas.at(start).vardas();

	int count = 0;
	for (int i = start + 1; i <= end; i++)
	{
		if (!daugiau(mas.at(i).vardas(), pivot)) count++;
	}

	int pivotIndex = start + count;
	swap(mas.at(pivotIndex), mas.at(start));

	int i = start, j = end;

	while (i < pivotIndex && j > pivotIndex)
	{
		while (!daugiau(mas.at(i).vardas(), pivot))
		{
			i++;
		}

		while (daugiau(mas.at(j).vardas(), pivot))
		{
			j--;
		}

		if (i < pivotIndex && j > pivotIndex)
		{
			swap(mas.at(i++), mas.at(j--));
		}
	}

	return pivotIndex;
}

//! surūšiuoja studentų vektorių pagal vardą, kai vardai nėra šabloniniai (vardas100) 
void quicksort(vektorius<studentas>& mas, int start, int end) // kai vardai nera sabloniniai
{
	if (start >= end) return;

	int p = partition(mas, start, end);

	quicksort(mas, start, p - 1);

	quicksort(mas, p + 1, end);
}

//! išveda studentų vektorių pasirinktu būdu 
void print(vektorius<studentas> &mas, char mode) {

	stringstream stream;
	string output;
	ofstream write("res.txt", ios_base::app);

	studentas temp;

	if (mode == 'v' || mode == 'V')
	{
		for (int i = 0; i < mas.size(); i++)
		{
			temp = mas.at(i);
			stream << left << setw(20) << temp.pavarde() << setw(15) << temp.vardas() << setw(3) << fixed << setprecision(2) << temp.vid() << endl;
			if ((i + 1) % 1000 == 0 || (i == mas.size() - 2))
			{
				output = stream.str();
				write << output;
				stream.str("");
			}
		}
	}
	else if (mode == 'm' || mode == 'M')
	{
		for (int i = 0; i < mas.size(); i++)
		{
			temp = mas.at(i);
			stream << left << setw(20) << temp.pavarde() << setw(15) << temp.vardas() << setw(3) << fixed << setprecision(2) << temp.med() << endl;
			if ((i + 1) % 1000 == 0 || (i == mas.size() - 2))
			{
				output = stream.str();
				write << output;
				stream.str("");
			}
		}
	}
}

//! padalina studentų vektorių į kietų studentu vektorių ir prastų studentų vektorių 
void dalinti(vektorius<studentas>& mas, vektorius<studentas>& kieti, vektorius<studentas>& prasti)
{
	auto it = stable_partition(mas.begin(), mas.end(), [](const studentas& s) { return s.vid() >= 5; });

	kieti.assign(mas.begin(), it);

	prasti.assign(it, mas.end());
}

//! perkelia kietus studentus į kietų studentų vektorių, o prastus palieka pradiniame vektoriuje 
void dalinti2(vektorius<studentas>& mas, vektorius<studentas>& kieti)
{
	auto is_kieti = [](const studentas& s) { return s.vid() >= 5; };

	auto kieti_end = stable_partition(mas.begin(), mas.end(), is_kieti);

	kieti.assign(mas.begin(), kieti_end);

	mas.erase(mas.begin(), kieti_end);
}

//! išveda kietų studentų vektorių 
void kout(vektorius<studentas>& kieti, int pazNr)
{
	string tempSt = "";

	for (int i = 0; i < pazNr; i++)
	{
		tempSt += "paz" + to_string(i + 1) + " ";
	}
	tempSt += " egz";

	ofstream wkieti("kieti.txt");
	wkieti << left << setw(20) << "Pavarde" << setw(15) << "Vardas " << tempSt << endl;
	wkieti << "___________________________________________________________________________________________" << endl;

	stringstream stream;
	string output;

	studentas kietas;

	for (int i = 0; i < kieti.size(); i++)
	{
		kietas = kieti.at(i);
		stream << left << setw(20) << kietas.pavarde() << setw(15) << kietas.vardas();
		for (int j = 0; j < pazNr; j++)
		{
			stream << right << setw(4) << kietas.paz_at(j) << " ";
		}
		stream << right << setw(4) << kietas.egz() << endl;

		if ((i + 1) % 1000 == 0 || (i == kieti.size() - 1))
		{
			output = stream.str();
			wkieti << output;
			stream.str("");
		}
	}
}

//! išveda prastų studentų vektorių 
void pout(vektorius<studentas>& prasti, int pazNr)
{

	string tempSt = "";

	for (int i = 0; i < pazNr; i++)
	{
		tempSt += "paz" + to_string(i + 1) + " ";
	}
	tempSt += " egz";

	ofstream wprasti("prasti.txt");
	wprasti << left << setw(20) << "Pavarde" << setw(15) << "Vardas " << tempSt << endl;
	wprasti << "___________________________________________________________________________________________" << endl;

	stringstream stream;
	string output;

	studentas prastas;

	for (int i = 0; i < prasti.size(); i++)
	{
		prastas = prasti.at(i);
		stream << left << setw(20) << prastas.pavarde() << setw(15) << prastas.vardas();
		for (int j = 0; j < pazNr; j++)
		{
			stream << right << setw(4) << prastas.paz_at(j) << " ";
		}
		stream << right << setw(4) << prastas.egz() << endl;

		if ((i + 1) % 1000 == 0 || (i == prasti.size() - 1))
		{
			output = stream.str();
			wprasti << output;
			stream.str("");
		}
	}
}

//! copy konstruktorius studentas klasei 
studentas::studentas(const studentas& kitas) {
	this->vardas_ = kitas.vardas_;
	this->pavarde_ = kitas.pavarde_;
	this->paz_ = kitas.paz_;
	this->egz_ = kitas.egz_;
	this->vid_ = kitas.vid_;
	this->med_ = kitas.med_;
}

//! move konstruktorius studentas klasei 
studentas::studentas(studentas&& kitas) {
	this->vardas_ = kitas.vardas_;
	this->pavarde_ = kitas.pavarde_;
	this->paz_ = kitas.paz_;
	this->egz_ = kitas.egz_;
	this->vid_ = kitas.vid_;
	this->med_ = kitas.med_;
	
	kitas.vardas_ = " ";
	kitas.pavarde_ = "";
	kitas.paz_.clear();
	kitas.egz_ = 0;
	kitas.vid_ = 0;
	kitas.med_ = 0;
	
}
