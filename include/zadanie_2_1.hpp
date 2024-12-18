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
Co trzeba zrobiæ?

Odczytaæ raporty z pliku wejœciowego.
Sprawdziæ, czy ka¿dy raport jest bezpieczny, weryfikuj¹c dwa warunki bezpieczeñstwa.
Policzyæ liczbê raportów spe³niaj¹cych kryteria bezpieczeñstwa.
Wyœwietliæ wynik na ekranie.


*/

#include <fstream>  // std::ifstream - do obs³ugi plików wejœciowych
#include <iostream> // std::cout, std::cerr - do obs³ugi wejœcia/wyjœcia
#include <ranges>   // std::ranges - operacje na sekwencjach danych
#include <string>   // std::string - do przechowywania nazw plików
#include <vector>   // std::vector - dynamiczne kontenery na poziomy raportów

/*
Przyk³ad raportów:
7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9

Analiza:
Raporty bezpieczne spe³niaj¹ dwa warunki:
1. Wszystkie poziomy s¹ albo rosn¹ce, albo malej¹ce.
2. Ka¿da ró¿nica miêdzy s¹siaduj¹cymi poziomami mieœci siê w przedziale [1, 3].

Przyk³ad analizy:
- 7 6 4 2 1: Bezpieczny - wszystkie poziomy malej¹ o 1 lub 2.
- 1 2 7 8 9: Niebezpieczny - ró¿nica miêdzy 2 a 7 wynosi 5.
- 9 7 6 2 1: Niebezpieczny - ró¿nica miêdzy 6 a 2 wynosi 4.
- 1 3 2 4 5: Niebezpieczny - zmiana kierunku (rosn¹ce -> malej¹ce).
- 8 6 4 4 1: Niebezpieczny - ró¿nica 4 - 4 wynosi 0.
- 1 3 6 7 9: Bezpieczny - wszystkie poziomy rosn¹ o 1, 2 lub 3.
*/

// Funkcja sprawdzaj¹ca, czy raport jest bezpieczny
// Sprawdza kierunek (rosn¹cy/malej¹cy) oraz ró¿nice miêdzy poziomami
bool czyRaportBezpieczny(const std::vector< int >& poziomy)
{
    // Jeœli raport zawiera mniej ni¿ dwa poziomy, jest niebezpieczny
    if (poziomy.size() < 2)
    {
        std::cout << "Raport ma mniej ni¿ dwa poziomy, wiêc jest niebezpieczny.\n";
        return false;
    }

    // Ustalenie kierunku (rosn¹cy lub malej¹cy)
    bool rosnacy  = poziomy[1] > poziomy[0];
    bool malejacy = poziomy[1] < poziomy[0];

    // Jeœli poziomy nie s¹ wyraŸnie rosn¹ce ani malej¹ce, raport jest niebezpieczny
    if (!rosnacy && !malejacy)
    {
        std::cout << "Poziomy nie s¹ ani wyraŸnie rosn¹ce, ani malej¹ce.\n";
        return false;
    }

    // Sprawdzanie ró¿nic miêdzy kolejnymi poziomami
    for (size_t i = 1; i < poziomy.size(); ++i)
    {
        int roznica = poziomy[i] - poziomy[i - 1];
        // Sprawdzenie warunków dla rosn¹cego kierunku
        if (rosnacy && (roznica <= 0 || roznica > 3))
        {
            std::cout << "Nieprawid³owa ró¿nica przy wzroœcie: " << roznica << " (poziom " << poziomy[i - 1] << " -> "
                      << poziomy[i] << ").\n";
            return false;
        }
        // Sprawdzenie warunków dla malej¹cego kierunku
        if (malejacy && (roznica >= 0 || roznica < -3))
        {
            std::cout << "Nieprawid³owa ró¿nica przy spadku: " << roznica << " (poziom " << poziomy[i - 1] << " -> "
                      << poziomy[i] << ").\n";
            return false;
        }
    }

    std::cout << "Raport jest bezpieczny.\n";
    return true;
}

// Funkcja wczytuj¹ca dane z pliku i analizuj¹ca raporty
// Dzieli linie na liczby i sprawdza, czy raporty s¹ bezpieczne
int analizujRaporty(const std::string& nazwaPliku)
{
    std::ifstream plik(nazwaPliku); // Otwieranie pliku
    if (!plik.is_open())
    {
        std::cerr << "Nie mo¿na otworzyæ pliku: " << nazwaPliku << std::endl;
        return -1;
    }

    int         liczbaBezpiecznych = 0; // Licznik bezpiecznych raportów
    std::string linia;                  // Zmienna do przechowywania linii z pliku

    // Iteracja po ka¿dej linii pliku (ka¿dy raport po koleui)
    while (std::getline(plik, linia))
    {
        std::cout << "Analiza raportu: " << linia << "\n";

        // Podzia³ linii na liczby przy u¿yciu std::ranges
        std::vector< int > poziomy; // wektor, który bêdzie przechowywaæ liczby odczytane z danej linii.
        // std::views::split, aby podzieliæ ci¹g znaków (liniê) na fragmenty oddzielone spacjami.
        for (auto&& czesc : linia | std::views::split(' ')) 
        {
            // "czesc" to podzakres (std::ranges::subrange) zawieraj¹cy iteratory do aktualnego fragmentu linii.

            // Konwersja podzakresu (subrange) na std::string:
            // std::string(czesc.begin(), czesc.end()) - pocz¹tek i koniec bie¿¹cego fragmentu podzielonej linii
            // Tworzymy z tego std::string, aby móc przekazaæ go do std::stoi, które wymaga std::string jako wejœcia.
            std::string token(czesc.begin(), czesc.end()); // Konwersja na std::string
            // Konwersja ci¹gu znaków (token) na liczbê ca³kowit¹ za pomoc¹ std::stoi:
            // - std::stoi przekszta³ca ci¹g znaków w liczbê ca³kowit¹ (int).
            // - Przekszta³cona liczba jest nastêpnie dodawana do wektora poziomy.
            poziomy.push_back(std::stoi(token));
            // Po zakoñczeniu pêtli wektor "poziomy" zawiera wszystkie liczby ca³kowite odczytane i przetworzone z danej linii
        }

        // Drukowanie poziomów raportu
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

// Funkcja g³ówna do uruchomienia analizy raportów
void wykonajAnalizeRaportow(const std::string& sciezkaPliku)
{
    int wynik = analizujRaporty(sciezkaPliku);
    if (wynik >= 0)
    {
        std::cout << "Liczba bezpiecznych raportów: " << wynik << std::endl; //liczba bezp raportow
    }
}