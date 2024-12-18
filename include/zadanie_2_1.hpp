/*
The unusual data (your puzzle input) consists of many reports, one report per line. Each report is a list of numbers
called levels that are separated by spaces. For example:

7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9
This example data contains six reports each containing five levels.

The engineers are trying to figure out which reports are safe. The Red-Nosed reactor safety systems can only tolerate
levels that are either gradually increasing or gradually decreasing. So, a report only counts as safe if both of the
following are true:

The levels are either all increasing or all decreasing.
Any two adjacent levels differ by at least one and at most three.
In the example above, the reports can be found safe or unsafe by checking those rules:

7 6 4 2 1: Safe because the levels are all decreasing by 1 or 2.
1 2 7 8 9: Unsafe because 2 7 is an increase of 5.
9 7 6 2 1: Unsafe because 6 2 is a decrease of 4.
1 3 2 4 5: Unsafe because 1 3 is increasing but 3 2 is decreasing.
8 6 4 4 1: Unsafe because 4 4 is neither an increase or a decrease.
1 3 6 7 9: Safe because the levels are all increasing by 1, 2, or 3.
So, in this example, 2 reports are safe.

Analyze the unusual data from the engineers. How many reports are safe?
*/


/*
Co trzeba zrobi�?

Odczyta� raporty z pliku wej�ciowego.
Sprawdzi�, czy ka�dy raport jest bezpieczny, weryfikuj�c dwa warunki bezpiecze�stwa.
Policzy� liczb� raport�w spe�niaj�cych kryteria bezpiecze�stwa.
Wy�wietli� wynik na ekranie.


*/

#include <fstream>  // std::ifstream - do obs�ugi plik�w wej�ciowych
#include <iostream> // std::cout, std::cerr - do obs�ugi wej�cia/wyj�cia
#include <ranges>   // std::ranges - operacje na sekwencjach danych
#include <string>   // std::string - do przechowywania nazw plik�w
#include <vector>   // std::vector - dynamiczne kontenery na poziomy raport�w

/*
Przyk�ad raport�w:
7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9

Analiza:
Raporty bezpieczne spe�niaj� dwa warunki:
1. Wszystkie poziomy s� albo rosn�ce, albo malej�ce.
2. Ka�da r�nica mi�dzy s�siaduj�cymi poziomami mie�ci si� w przedziale [1, 3].

Przyk�ad analizy:
- 7 6 4 2 1: Bezpieczny - wszystkie poziomy malej� o 1 lub 2.
- 1 2 7 8 9: Niebezpieczny - r�nica mi�dzy 2 a 7 wynosi 5.
- 9 7 6 2 1: Niebezpieczny - r�nica mi�dzy 6 a 2 wynosi 4.
- 1 3 2 4 5: Niebezpieczny - zmiana kierunku (rosn�ce -> malej�ce).
- 8 6 4 4 1: Niebezpieczny - r�nica 4 - 4 wynosi 0.
- 1 3 6 7 9: Bezpieczny - wszystkie poziomy rosn� o 1, 2 lub 3.
*/

// Funkcja sprawdzaj�ca, czy raport jest bezpieczny
// Sprawdza kierunek (rosn�cy/malej�cy) oraz r�nice mi�dzy poziomami
bool czyRaportBezpieczny(const std::vector< int >& poziomy)
{
    // Je�li raport zawiera mniej ni� dwa poziomy, jest niebezpieczny
    if (poziomy.size() < 2)
    {
        std::cout << "Raport ma mniej ni� dwa poziomy, wi�c jest niebezpieczny.\n";
        return false;
    }

    // Ustalenie kierunku (rosn�cy lub malej�cy)
    bool rosnacy  = poziomy[1] > poziomy[0];
    bool malejacy = poziomy[1] < poziomy[0];

    // Je�li poziomy nie s� wyra�nie rosn�ce ani malej�ce, raport jest niebezpieczny
    if (!rosnacy && !malejacy)
    {
        std::cout << "Poziomy nie s� ani wyra�nie rosn�ce, ani malej�ce.\n";
        return false;
    }

    // Sprawdzanie r�nic mi�dzy kolejnymi poziomami
    for (size_t i = 1; i < poziomy.size(); ++i)
    {
        int roznica = poziomy[i] - poziomy[i - 1];
        // Sprawdzenie warunk�w dla rosn�cego kierunku
        if (rosnacy && (roznica <= 0 || roznica > 3))
        {
            std::cout << "Nieprawid�owa r�nica przy wzro�cie: " << roznica << " (poziom " << poziomy[i - 1] << " -> "
                      << poziomy[i] << ").\n";
            return false;
        }
        // Sprawdzenie warunk�w dla malej�cego kierunku
        if (malejacy && (roznica >= 0 || roznica < -3))
        {
            std::cout << "Nieprawid�owa r�nica przy spadku: " << roznica << " (poziom " << poziomy[i - 1] << " -> "
                      << poziomy[i] << ").\n";
            return false;
        }
    }

    std::cout << "Raport jest bezpieczny.\n";
    return true;
}

// Funkcja wczytuj�ca dane z pliku i analizuj�ca raporty
// Dzieli linie na liczby i sprawdza, czy raporty s� bezpieczne
int analizujRaporty(const std::string& nazwaPliku)
{
    std::ifstream plik(nazwaPliku); // Otwieranie pliku
    if (!plik.is_open())
    {
        std::cerr << "Nie mo�na otworzy� pliku: " << nazwaPliku << std::endl;
        return -1;
    }

    int         liczbaBezpiecznych = 0; // Licznik bezpiecznych raport�w
    std::string linia;                  // Zmienna do przechowywania linii z pliku

    // Iteracja po ka�dej linii pliku (ka�dy raport po koleui)
    while (std::getline(plik, linia))
    {
        std::cout << "Analiza raportu: " << linia << "\n";

        // Podzia� linii na liczby przy u�yciu std::ranges
        std::vector< int > poziomy; // wektor, kt�ry b�dzie przechowywa� liczby odczytane z danej linii.
        // std::views::split, aby podzieli� ci�g znak�w (lini�) na fragmenty oddzielone spacjami.
        for (auto&& czesc : linia | std::views::split(' ')) 
        {
            // "czesc" to podzakres (std::ranges::subrange) zawieraj�cy iteratory do aktualnego fragmentu linii.

            // Konwersja podzakresu (subrange) na std::string:
            // std::string(czesc.begin(), czesc.end()) - pocz�tek i koniec bie��cego fragmentu podzielonej linii
            // Tworzymy z tego std::string, aby m�c przekaza� go do std::stoi, kt�re wymaga std::string jako wej�cia.
            std::string token(czesc.begin(), czesc.end()); // Konwersja na std::string
            // Konwersja ci�gu znak�w (token) na liczb� ca�kowit� za pomoc� std::stoi:
            // - std::stoi przekszta�ca ci�g znak�w w liczb� ca�kowit� (int).
            // - Przekszta�cona liczba jest nast�pnie dodawana do wektora poziomy.
            poziomy.push_back(std::stoi(token));
            // Po zako�czeniu p�tli wektor "poziomy" zawiera wszystkie liczby ca�kowite odczytane i przetworzone z danej linii
        }

        // Drukowanie poziom�w raportu
        std::cout << "Poziomy w raporcie: ";
        for (size_t i = 0; i < poziomy.size(); ++i)
        {
            std::cout << poziomy[i] << (i < poziomy.size() - 1 ? ", " : "\n");
        }

        // Sprawdzenie, czy raport jest bezpieczny
        if (czyRaportBezpieczny(poziomy))
        {
            ++liczbaBezpiecznych;
            std::cout << "Raport oznaczony jako bezpieczny.\n";
        }
        else
        {
            std::cout << "Raport oznaczony jako niebezpieczny.\n";
        }
    }

    plik.close();
    return liczbaBezpiecznych;
}

// Funkcja g��wna do uruchomienia analizy raport�w
void wykonajAnalizeRaportow(const std::string& sciezkaPliku)
{
    int wynik = analizujRaporty(sciezkaPliku);
    if (wynik >= 0)
    {
        std::cout << "Liczba bezpiecznych raport�w: " << wynik << std::endl; //liczba bezp raportow
    }
}