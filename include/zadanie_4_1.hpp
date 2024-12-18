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
--- Dzieñ 4: Wyszukiwanie na Ceres ---

Zadanie polega na znalezieniu wszystkich wyst¹pieñ s³owa "XMAS" w podanej tablicy znaków.
S³owo mo¿e byæ zapisane poziomo, pionowo, ukoœnie, wspak lub nawet nak³adaæ siê na inne s³owa.
Twoim zadaniem jest przetworzenie tablicy znaków i policzenie wszystkich wyst¹pieñ s³owa we wszystkich mo¿liwych
kierunkach.

Przyk³ad:
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

W tym przyk³adzie s³owo "XMAS" wystêpuje 18 razy.
*/







#include <algorithm> // std::max
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::cerr
#include <string>    // std::string
#include <vector>    // std::vector

// Funkcja sprawdzaj¹ca wyst¹pienie s³owa w tablicy znaków
int znajdzWystapienia(const std::vector< std::string >& tablica, const std::string& slowo)
{
    const int dlugosc         = slowo.size(); // D³ugoœæ szukanego s³owa
    int       liczbaWystapien = 0;  // Licznik znalezionych wyst¹pieñ


    // Wymiary tablicy
    const int wiersze = tablica.size();
    const int kolumny = tablica[0].size();

    std::cout << "Rozpoczynam przeszukiwanie tablicy o wymiarach " << wiersze << "x" << kolumny << "\n";

    // Funkcja sprawdzaj¹ca wyst¹pienie s³owa w danym kierunku
    /*
    Lambda sprawdzKierunek sprawdza, czy dane s³owo mo¿na znaleŸæ w tablicy znaków, 
    zaczynaj¹c od podanego punktu (x, y) i poruszaj¹c siê w kierunku okreœlonym przez (dx, dy).
    */
    auto sprawdzKierunek = [&](int x, int y, int dx, int dy) {
        //std::cout << "  Sprawdzam kierunek z punktu (" << x << ", " << y << ") dla wektora (" << dx << ", " << dy << ")\n";
        for (int i = 0; i < dlugosc; ++i)
        {
            int nx = x + i * dx;
            int ny = y + i * dy;

            // Sprawdzanie granic tablicy oraz zgodnoœci znaków z poszukiwanym s³owem
            if (nx < 0 || ny < 0 || nx >= wiersze || ny >= kolumny || tablica[nx][ny] != slowo[i])
            {
                return false;
            }
        }
        return true; // Jeœli wszystkie znaki pasuj¹
    };

    // Przeszukiwanie tablicy
    for (int i = 0; i < wiersze; ++i)
    {
        for (int j = 0; j < kolumny; ++j)
        {
            // Sprawdzanie wszystkich kierunków
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
                std::cout << "  -> Znaleziono pionowo w dó³!\n";
            }
            if (sprawdzKierunek(i, j, -1, 0))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono pionowo w górê!\n";
            }
            if (sprawdzKierunek(i, j, 1, 1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w prawo w dó³)!\n";
            }
            if (sprawdzKierunek(i, j, -1, -1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w lewo w górê)!\n";
            }
            if (sprawdzKierunek(i, j, 1, -1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w lewo w dó³)!\n";
            }
            if (sprawdzKierunek(i, j, -1, 1))
            {
                liczbaWystapien++;
                std::cout << "  -> Znaleziono po skosie (w prawo w górê)!\n";
            }
        }
    }

    return liczbaWystapien;
}

// Funkcja wczytuj¹ca dane z pliku
std::vector< std::string > wczytajTablice(const std::string& nazwaPliku)
{
    std::ifstream plik(nazwaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie mo¿na otworzyæ pliku: " << nazwaPliku << std::endl;
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

// Funkcja wykonuj¹ca analizê tablicy i wyœwietlaj¹ca wynik
void wykonajAnalizeWordSearch(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza przyk³adu z zadania
    std::cout << "=== Analiza przyk³adu ===\n";
    std::vector< std::string > tablicaPrzykladu = wczytajTablice(sciezkaPrzykladu);
    if (!tablicaPrzykladu.empty())
    {
        const std::string slowo           = "XMAS";
        int               liczbaWystapien = znajdzWystapienia(tablicaPrzykladu, slowo);
        std::cout << "Liczba wyst¹pieñ s³owa '" << slowo << "' w przyk³adzie: " << liczbaWystapien << "\n";
    }

    // Wymaganie potwierdzenia przed analiz¹ puzzle input
    std::cout << "\nAby kontynuowaæ i przeanalizowaæ dane z puzzle input, naciœnij Enter...\n";
    std::cin.get();

    // Analiza danych z puzzle input
    std::cout << "\n=== Analiza danych z puzzle input ===\n";
    std::vector< std::string > tablicaPliku = wczytajTablice(sciezkaPliku);
    if (!tablicaPliku.empty())
    {
        const std::string slowo           = "XMAS";
        int               liczbaWystapien = znajdzWystapienia(tablicaPliku, slowo); //za³¹czenie wyszukiwania
        std::cout << "Liczba wyst¹pieñ s³owa '" << slowo << "' w puzzle input: " << liczbaWystapien << "\n";
    }
}