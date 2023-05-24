#ifndef STUDENTAS_H
#define STUDENTAS_H

#include "studentasLib.h"
#include "vektorius.h"

/**  
 * klasė, turinti vardo ir pavardės kintamuosius, su getter ir setter funkcijomis bei virtulia funkcija vid()
 */

class zmogus
{
protected:
	//! žmogaus vardas
	string vardas_ = "";
	//! žmogaus pavardė
	string pavarde_ = "";

public:
	// getters
	//! gražina vardą
	inline string vardas() const { return vardas_; }
	//! gražina pavardę
	inline string pavarde() const { return pavarde_; }

	// setters
	//! nustato vardą
	void setName(const string& word) { vardas_ = word; }
	//! nustato pavardę
	void setSurname(const string& word) { pavarde_ = word; }

	//! gražina vidurkį
	virtual inline double vid() const = 0;

};

/**
 * klasė, paveldinti iš klasės zmogus, turinti kintamuosius pažymiams, 
 * "rule of five" funkcijas, getterius, setterius, funkcijas apskaičiuoti 
 * pažymių vidurkį ir medianą, įvesti pažymius bei išvesti objekto vardą 
 * bei pavardę ir pažymius, naudojant operatorių <<
*/
class studentas : public zmogus
{
private:
	vektorius<int> paz_;
	int egz_;
	double vid_ = 0;
	double med_ = 0;
public:

	// constructors
	studentas() : egz_(0) {}
	studentas(string line, int pazCount); // nuskaityti is failo
	studentas(const studentas& kitas); // copy constructor
	studentas(studentas&& kitas); // move constructor

	~studentas() {}

	// getters
	//! gražina studento pažymių vidurkį
	inline double vid() const override { return vid_; }
	//! gražina studento pažymių medianą
	inline double med() const { return med_; }
	//! gražina studento egzamino pažymį
	inline int egz() const { return egz_; }
	//! gražina studento i+1-ąjį pažymį
	inline int paz_at(int i) const { return paz_.at(i); }


	// setters
	//! prideda pažymį studentui
	void pushPaz(const int& paz) { paz_.push_back(paz); }
	//! įveda egzamino pažymį
	void pushEgz(const int& paz) { egz_ = paz; }

	// misc
	//! apskaičiuoja studento pažymių vidurkį
	void setVid();
	//! apskaičiuoja studento pažymių medianą
	void setMed();

	//! copy assignment funkcija
	studentas& operator=(const studentas& kitas) // copy assignment
	{
		if (this != &kitas)
		{
			this->vardas_ = kitas.vardas_;
			this->pavarde_ = kitas.pavarde_;
			this->paz_ = kitas.paz_;
			this->egz_ = kitas.egz_;
			this->vid_ = kitas.vid_;
			this->med_ = kitas.med_;
		}

		return *this;
	}

	//! move assignment funkcija
	studentas& operator=(studentas&& kitas) // move assignment
	{
		if (this != &kitas)
		{
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

		return *this;
	}

	//! išvedimas per << operatorių
	friend ostream& operator<<(ostream& out, const studentas& s)
	{
		out << left << setw(20) << s.pavarde_ << setw(15) << s.vardas_;
		for (int paz : s.paz_)
		{
			out << right << setw(4) << paz;
		}
		out << right << setw(4) << s.egz_ << endl;
		return out;
	}
};

void pildyti(vektorius<studentas>& mas, int& pazCount);
void nuskaityti(vektorius<studentas> &mas, int &stCount, int &pazNr);
void generuoti(vektorius<studentas> &mas, int &stCount, int &pazNr);
bool daugiau(string s1, string s2);
int partition(vektorius<studentas>& mas, int start, int end);
void quicksort(vektorius<studentas>& mas, int start, int end);
void dalinti(vektorius<studentas>& mas, vektorius<studentas> &kieti, vektorius<studentas> &prasti);
void dalinti2(vektorius<studentas> &mas, vektorius<studentas> &kieti);
void print(vektorius<studentas> &mas, char mode);
void kout(vektorius<studentas> &kieti, int pazCount);
void pout(vektorius<studentas> &prasti, int pazCount);

#endif


