/*
This word search allows words to be horizontal, vertical, diagonal, written backwards, or even overlapping other words.
It's a little unusual, though, as you don't merely need to find one instance of XMAS - you need to find all of them.
Here are a few ways XMAS might appear, where irrelevant characters have been replaced with .:


..X...
.SAMX.
.A..A.
XMAS.S
.X....
The actual word search will be full of letters instead. For example:

MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX
In this word search, XMAS occurs a total of 18 times; here's the same word search again, but where letters not involved
in any XMAS have been replaced with .:

....XXMAS.
.SAMXMS...
...S..A...
..A.A.MS.X
XMASAMX.MM
X.....XA.A
S.S.S.S.SS
.A.A.A.A.A
..M.M.M.MM
.X.X.XMASX
Take a look at the little Elf's word search. How many times does XMAS appear?
*/




/*
--- Dzie� 4: Wyszukiwanie na Ceres ---

Zadanie polega na znalezieniu wszystkich wyst�pie� s�owa "XMAS" w podanej tablicy znak�w.
S�owo mo�e by� zapisane poziomo, pionowo, uko�nie, wspak lub nawet nak�ada� si� na inne s�owa.
Twoim zadaniem jest przetworzenie tablicy znak�w i policzenie wszystkich wyst�pie� s�owa we wszystkich mo�liwych
kierunkach.

Przyk�ad:
Tablica:
....XXMAS.
.SAMXMS...
...S..A...
..A.A.MS.X
XMASAMX.MM
X.....XA.A
S.S.S.S.SS
.A.A.A.A.A
..M.M.M.MM
.X.X.XMASX

W tym przyk�adzie s�owo "XMAS" wyst�puje 18 razy.
*/







#include <algorithm> // std::max
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::cerr
#include <string>    // std::string
#include <vector>    // std::vector

// Funkcja sprawdzaj�ca wyst�pienie s�owa w tablicy znak�w
int znajdzWystapienia(const std::vector< std::string >& tablica, const std::string& slowo)
{
    const int dlugosc         = slowo.size(); // D�ugo�� szukanego s�owa
    int       liczbaWystapien = 0;  // Licznik znalezionych wyst�pie�


    // Wymiary tablicy
    const int wiersze = tablica.size();
    const int kolumny = tablica[0].size();

    std::cout << "Rozpoczynam przeszukiwanie tablicy o wymiarach " << wiersze << "x" << kolumny << "\n";

    // Funkcja sprawdzaj�ca wyst�pienie s�owa w danym kierunku
    /*
    Lambda sprawdzKierunek sprawdza, czy dane s�owo mo�na znale�� w tablicy znak�w, 
    zaczynaj�c od podanego punktu (x, y) i poruszaj�c si� w kierunku okre�lonym przez (dx, dy).
    */
    auto sprawdzKierunek = [&](int x, int y, int dx, int dy) {
        //std::cout << "  Sprawdzam kierunek z punktu (" << x << ", " << y << ") dla wektora (" << dx << ", " << dy << ")\n";
        for (int i = 0; i < dlugosc; ++i)
        {
            int nx = x + i * dx;
            int ny = y + i * dy;

            // Sprawdzanie granic tablicy oraz zgodno�ci znak�w z poszukiwanym s�owem
            if (nx < 0 || ny < 0 || nx >= wiersze || ny >= kolumny || tablica[nx][ny] != slowo[i])
            {
                return false;
            }
        }
        return true; // Je�li wszystkie znaki pasuj�
    };

    // Przeszukiwanie tablicy
    for (int i = 0; i < wiersze; ++i)
    {
        for (int j = 0; j < kolumny; ++j)
        {
            // Sprawdzanie wszystkich kierunk�w
            if (sprawdzKierunek(i, j, 0, 1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono poziomo w prawo!\n";
            }
            if (sprawdzKierunek(i, j, 0, -1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono poziomo w lewo!\n";
            }
            if (sprawdzKierunek(i, j, 1, 0))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono pionowo w d�!\n";
            }
            if (sprawdzKierunek(i, j, -1, 0))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono pionowo w g�r�!\n";
            }
            if (sprawdzKierunek(i, j, 1, 1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w prawo w d�)!\n";
            }
            if (sprawdzKierunek(i, j, -1, -1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w lewo w g�r�)!\n";
            }
            if (sprawdzKierunek(i, j, 1, -1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w lewo w d�)!\n";
            }
            if (sprawdzKierunek(i, j, -1, 1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w prawo w g�r�)!\n";
            }
        }
    }

    return liczbaWystapien;
}

// Funkcja wczytuj�ca dane z pliku
std::vector< std::string > wczytajTablice(const std::string& nazwaPliku)
{
    std::ifstream plik(nazwaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie mo�na otworzy� pliku: " << nazwaPliku << std::endl;
        return {};
    }

    std::vector< std::string > tablica;
    std::string                linia;
    while (std::getline(plik, linia))
    {
        tablica.push_back(linia);
    }

    plik.close();
    std::cout << "Tablica wczytana z pliku: " << nazwaPliku << "\n";
    for (const auto& wiersz : tablica)
    {
        std::cout << "  " << wiersz << "\n";
    }
    return tablica;
}

// Funkcja wykonuj�ca analiz� tablicy i wy�wietlaj�ca wynik
void wykonajAnalizeWordSearch(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza przyk�adu z zadania
    std::cout << "=== Analiza przyk�adu ===\n";
    std::vector< std::string > tablicaPrzykladu = wczytajTablice(sciezkaPrzykladu);
    if (!tablicaPrzykladu.empty())
    {
        const std::string slowo           = "XMAS";
        int               liczbaWystapien = znajdzWystapienia(tablicaPrzykladu, slowo);
        std::cout << "Liczba wyst�pie� s�owa '" << slowo << "' w przyk�adzie: " << liczbaWystapien << "\n";
    }

    // Wymaganie potwierdzenia przed analiz� puzzle input
    std::cout << "\nAby kontynuowa� i przeanalizowa� dane z puzzle input, naci�nij Enter...\n";
    std::cin.get();

    // Analiza danych z puzzle input
    std::cout << "\n=== Analiza danych z puzzle input ===\n";
    std::vector< std::string > tablicaPliku = wczytajTablice(sciezkaPliku);
    if (!tablicaPliku.empty())
    {
        const std::string slowo           = "XMAS";
        int               liczbaWystapien = znajdzWystapienia(tablicaPliku, slowo); //za��czenie wyszukiwania
        std::cout << "Liczba wyst�pie� s�owa '" << slowo << "' w puzzle input: " << liczbaWystapien << "\n";
    }
}