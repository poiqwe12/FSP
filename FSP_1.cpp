// FSP.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#define ROZMiAR 25 
using namespace std;

class Fsp
{
public:
	int n;							// ilość zadań
	int m;							// ilość maszyn
	int tabP[ROZMiAR][ROZMiAR];		// tablica czasów trwania zadan
	int tabS[ROZMiAR][ROZMiAR];		// tablica czasów rozpoczęcia zadan
	int tabC[ROZMiAR][ROZMiAR];		// tablica czasów zakonczenia zadan
	int tabK[ROZMiAR];				// tablica permutacji zadań;

	Fsp();
	int Policztab();
	int PrzegladZupelny();
	void UstawienieDomysnejPermutacji();
	int AlgorytmJohanson();
};
Fsp::Fsp()
{
	for (int j = 0; j < ROZMiAR; ++j)
	{
		tabK[j] = j;
	}
	for (int j = 0; j < ROZMiAR; ++j)
	{
		for (int i = 0; i < ROZMiAR; ++i)
		{
			tabP[i][j] = 0;		// tablica czasów trwania zadan
			tabS[i][j] = 0;		// tablica czasów rozpoczęcia zadan
			tabC[i][j] = 0;		// tablica czasów zakonczenia zadan
		}
	}
}
int Fsp::Policztab()
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if ((i == 0) && (j == 0))    // Pierwsza maszyna pierwsze zadanie
			{
				tabS[j][i] = 0;
				tabC[j][i] = tabP[tabK[j]][i];
			}
			else
			{
				if (i == 0)				 // Zadania na pierwszej maszynie ida bez przerw
				{
					tabS[j][i] = tabC[j - 1][i];
					tabC[j][i] = tabS[j][i] + tabP[tabK[j]][i];
				}
				else					// Nastepne maszyny 
				{
					if (j == 0)			// Pierwsze zadanie na kolejnych maszynach
					{
						tabS[j][i] = tabC[j][i - 1];
						tabC[j][i] = tabS[j][i] + tabP[tabK[j]][i];
					}
					else
					{
						if (tabC[j - 1][i] >= tabC[j][i - 1])
						{
							tabS[j][i] = tabC[j - 1][i];
							tabC[j][i] = tabS[j][i] + tabP[tabK[j]][i];
						}
						else
						{
							tabS[j][i] = tabC[j][i - 1];
							tabC[j][i] = tabS[j][i] + tabP[tabK[j]][i];
						}
					}
				}
			}
		}
	}
	return tabC[n - 1][m - 1];
}
int Fsp::PrzegladZupelny()
{
	int Cmax = INT16_MAX;
	int permutacjaOptymalna[ROZMiAR];
	UstawienieDomysnejPermutacji();
	do
	{
		Policztab();
		if (tabC[n - 1][m - 1] < Cmax)
		{
			cout << tabC[n - 1][m - 1] << endl;
			Cmax = tabC[n - 1][m - 1];
			for (int i = 0; i < n; ++i)
			{
				permutacjaOptymalna[i] = tabK[i];
			}
		}
	} while (next_permutation(tabK, tabK + n));

	for (int i = 0; i < n; ++i)
	{
		tabK[i] = permutacjaOptymalna[i];
	}
	Policztab();

	return Cmax;
}

void Fsp::UstawienieDomysnejPermutacji()
{
	for (int i = 0; i < ROZMiAR; ++i)
	{
		tabK[i] = i;
	}
}

int Fsp::AlgorytmJohanson()
{
	int kolejnoscDlaMaszynyM1[ROZMiAR];
	int kolejnoscDlaMaszynyM2[ROZMiAR];
	int indexM1 = 0;
	int indexM2 = 0;
	int zm;
	bool zmiana = 1;
	if (m == 2)    // Jeżeli problem składa sie z dwóch maszyn
	{
		for (int i = 0; i < n; ++i)
		{
			if (tabP[i][0] <= tabP[i][1])
			{
				kolejnoscDlaMaszynyM1[indexM1++] = i;    // zapisanie indeksów zadań gdzie czas p m1 < m2			
			}
			else
			{
				kolejnoscDlaMaszynyM2[indexM2++] = i;  // // zapisanie indeksów zadań gdzie czas p m1 > m2
			}
		}

		while (zmiana)               // Sortowanie rosnące
		{
			zmiana = 0;
			for (int i = 1; i < indexM1; ++i)
			{
				if (tabP[kolejnoscDlaMaszynyM1[i]][0] < tabP[kolejnoscDlaMaszynyM1[i - 1]][0])
				{
					zmiana = 1;
					zm = kolejnoscDlaMaszynyM1[i - 1];
					kolejnoscDlaMaszynyM1[i - 1] = kolejnoscDlaMaszynyM1[i];
					kolejnoscDlaMaszynyM1[i] = zm;
				}
			}
		}
		zmiana = 1;
		while (zmiana)			// Sortowanie malejące
		{
			zmiana = 0;
			for (int i = 1; i < indexM2; ++i)
			{
				if (tabP[kolejnoscDlaMaszynyM2[i]][1] > tabP[kolejnoscDlaMaszynyM2[i - 1]][1])
				{
					zmiana = 1;
					zm = kolejnoscDlaMaszynyM2[i - 1];
					kolejnoscDlaMaszynyM2[i - 1] = kolejnoscDlaMaszynyM2[i];
					kolejnoscDlaMaszynyM2[i] = zm;
				}
			}
		}
		for (int i = 0; i < indexM1; ++i)
		{
			tabK[i] = kolejnoscDlaMaszynyM1[i];
		}
		for (int i = 0; i < indexM2; ++i)
		{
			tabK[i + indexM1] = kolejnoscDlaMaszynyM2[i];
		}

	}
	return Policztab();
}

istream & operator>> (istream &wej, Fsp &zm)
{
	wej >> zm.n >> zm.m;
	char a;
	for (int i = 0; i < zm.n; ++i)
	{
		for (int j = 0; j < zm.m; ++j)
		{
			wej >> a >> zm.tabP[i][j];
		}
	}
	return wej;
}
ostream & operator<< (ostream &wyj, Fsp &zm)
{
	wyj << "--Dane Wejsciowe--" << endl;
	wyj << " n: " << zm.n << "\tm: " << zm.m << endl;
	for (int i = 0; i < zm.n; ++i)
	{
		wyj << "  ";
		for (int j = 0; j < zm.m; ++j)
		{
			wyj << zm.tabP[i][j] << "\t";
		}
		wyj << endl;
	}
	wyj << " --Macierz Momentow Rozpoczecia--" << endl;
	for (int i = 0; i < zm.n; ++i)
	{
		wyj << "  ";
		for (int j = 0; j < zm.m; ++j)
		{
			wyj << zm.tabS[i][j] << "\t";
		}
		wyj << endl;
	}
	wyj << " --Macierz Momentow Zakonczenia--" << endl;
	for (int i = 0; i < zm.n; ++i)
	{
		wyj << "  ";
		for (int j = 0; j < zm.m; ++j)
		{
			wyj << zm.tabC[i][j] << "\t";
		}
		wyj << endl;
	}
	wyj << " --Permutacja optymalna--" << endl;
	for (int i = 0; i < zm.n; ++i)
	{
		wyj << "  " << zm.tabK[i];
	}
	wyj << endl;
	return wyj;
}
bool WczytajZPliku(string nazwaPliku, Fsp &F)
{
	fstream plik;
	plik.open(nazwaPliku, std::ios::in | std::ios::out);
	if (plik.good() == true)
	{
		plik >> F;
		plik.close();
		return true;
	}
	return false;
}



int main()
{
	Fsp D, F;
	string nazwaPliku = "data001.txt";
	string tabNazwaPliku[] = { "data001.txt","data002.txt","data003.txt","data004.txt","data005.txt","data006.txt" };
	for (int i = 0; i < 6; ++i)
	{
		WczytajZPliku(tabNazwaPliku[i], F);
		clock_t start = clock();
		F.AlgorytmJohanson();
		cout << "-------------------" << endl;
		//cout << F;
		//F.AlgorytmJohanson();
		double czas = (double)(clock() - start);
		cout << "Czas trwania algorytmu: " << czas << endl;
		cout << F;
	}

}

