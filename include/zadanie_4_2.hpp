/*
Looking for the instructions, you flip over the word search to find that this isn't actually an XMAS puzzle; it's an
X-MAS puzzle in which you're supposed to find two MAS in the shape of an X. One way to achieve that is like this:

M.S
.A.
M.S
Irrelevant characters have again been replaced with . in the above diagram. Within the X, each MAS can be written
forwards or backwards.

Here's the same example from before, but this time all of the X-MASes have been kept instead:

.M.S......
..A..MSMS.
.M.S.MAA..
..A.ASMSM.
.M.S.M....
..........
S.S.S.S.S.
.A.A.A.A..
M.M.M.M.M.
..........
In this example, an X-MAS appears 9 times.

Flip the word search from the instructions back over to the word search side and try again. How many times does an X-MAS
appear?
*/



/*
1. Zliczy� wszystkie wyst�pienia wzorca "X-MAS" w tablicy znak�w.
2. Wy�wietli� szczeg�y ka�dego znalezionego wzorca oraz ich liczb�.
*/





#include <algorithm> // std::max
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::cerr
#include <string>    // std::string
#include <vector>    // std::vector

// Funkcja sprawdzaj�ca wyst�pienie X-MAS w tablicy znak�w
// Wyszukuje wzorzec "X-MAS" w tablicy, gdzie dwie sekwencje "MAS" tworz� liter� X z liter� "A" w �rodku
int znajdzWystapieniaXMAS(const std::vector< std::string >& tablica)
{
    const int wiersze         = tablica.size();    // Liczba wierszy w tablicy
    const int kolumny         = tablica[0].size(); // Liczba kolumn w tablicy
    int       liczbaWystapien = 0;                 // Licznik znalezionych wyst�pie�

    std::cout << "Rozpoczynam przeszukiwanie tablicy o wymiarach " << wiersze << "x" << kolumny << "\n";

    // Lambda sprawdzaj�ca X-MAS w kszta�cie X z danego punktu (x, y)
    // Sprawdza, czy �rodkowy punkt to "A" oraz czy na przek�tnych znajduj� si� odpowiednie litery "M" i "S"
    auto sprawdzKszta�tXMAS = [&](int x, int y) {
        // Sprawdzanie, czy punkt jest w granicach tablicy
        if (x <= 0 || x >= wiersze - 1 || y <= 0 || y >= kolumny - 1)
        {
            return false;
        }

        // �rodkowy punkt musi by� 'A'
        if (tablica[x][y] != 'A')
        {
            return false;
        }

        // Sprawdzamy przek�tn� g��wn� (M -> A -> S lub S -> A -> M)
        bool diagonal1 = (tablica[x - 1][y - 1] == 'M' && tablica[x + 1][y + 1] == 'S') ||
                         (tablica[x - 1][y - 1] == 'S' && tablica[x + 1][y + 1] == 'M');

        // Sprawdzamy przek�tn� poboczn� (M -> A -> S lub S -> A -> M)
        bool diagonal2 = (tablica[x + 1][y - 1] == 'M' && tablica[x - 1][y + 1] == 'S') ||
                         (tablica[x + 1][y - 1] == 'S' && tablica[x - 1][y + 1] == 'M');

        return diagonal1 && diagonal2; // Warunek spe�niony, je�li obie przek�tne tworz� poprawny wzorzec
    };

    // Przeszukiwanie tablicy znak�w
    for (int i = 0; i < wiersze; ++i)
    {
        for (int j = 0; j < kolumny; ++j)
        {
            // Sprawdzamy, czy punkt (i, j) jest �rodkiem wzorca "X-MAS"
            if (sprawdzKszta�tXMAS(i, j))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono X-MAS w punkcie (" << i << ", " << j << ")\n";
            }
        }
    }

    return liczbaWystapien; // Zwracamy liczb� znalezionych wzorc�w
}

// Funkcja wczytuj�ca dane z pliku (kopia z zadania 4.1)
// Odczytuje tablic� znak�w z pliku i zapisuje j� w wektorze �a�cuch�w
std::vector< std::string > wczytajTablice(const std::string& nazwaPliku)
{
    std::ifstream plik(nazwaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie mo�na otworzy� pliku: " << nazwaPliku << std::endl;
        return {};
    }

    std::vector< std::string > tablica; // Wektor przechowuj�cy wiersze tablicy
    std::string                linia;
    while (std::getline(plik, linia))
    {
        tablica.push_back(linia); // Dodajemy ka�dy wiersz do wektora
    }

    plik.close();
    std::cout << "Tablica wczytana z pliku: " << nazwaPliku << "\n";
    for (const auto& wiersz : tablica)
    {
        std::cout << "  " << wiersz << "\n";
    }
    return tablica; // Zwracamy wczytan� tablic�
}

// Funkcja wykonuj�ca analiz� tablicy dla wzorca X-MAS
// Wywo�uje funkcje wczytania tablicy i wyszukiwania wzorca, raportuje wyniki
void wykonajAnalizeXMAS(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza przyk�adu
    std::cout << "=== Analiza przyk�adu ===\n";
    std::vector< std::string > tablicaPrzykladu = wczytajTablice(sciezkaPrzykladu);
    if (!tablicaPrzykladu.empty())
    {
        int liczbaWystapienPrzyklad = znajdzWystapieniaXMAS(tablicaPrzykladu);
        std::cout << "Liczba wyst�pie� X-MAS w przyk�adzie: " << liczbaWystapienPrzyklad << "\n";
    }

    // Wymaganie potwierdzenia przed analiz� pliku wej�ciowego
    std::cout << "\nAby kontynuowa� i przeanalizowa� dane z pliku wej�ciowego, naci�nij Enter...\n";
    std::cin.get();

    // Analiza danych z pliku wej�ciowego
    std::cout << "\n=== Analiza danych z pliku wej�ciowego ===\n";
    std::vector< std::string > tablicaPliku = wczytajTablice(sciezkaPliku);
    if (!tablicaPliku.empty())
    {
        int liczbaWystapienPlik = znajdzWystapieniaXMAS(tablicaPliku);
        std::cout << "Liczba wyst�pie� X-MAS w pliku wej�ciowym: " << liczbaWystapienPlik << "\n";
    }
}