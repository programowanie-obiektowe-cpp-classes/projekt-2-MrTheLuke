#include <algorithm> // std::ranges::find, std::ranges::all_of
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::cerr
#include <ranges>    // std::views::split, std::views::transform
#include <sstream>   // std::istringstream
#include <string>    // std::string
#include <vector>    // std::vector

// Funkcja wczytuj�ca regu�y i listy stron z pliku
void wczytajDane(const std::string&                    sciezkaPliku,
                 std::vector< std::pair< int, int > >& reguly,
                 std::vector< std::vector< int > >&    aktualizacje)
{
    std::ifstream plik(sciezkaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie uda�o si� otworzy� pliku: " << sciezkaPliku << "\n";
        return;
    }

    std::string linia;
    bool        sekcjaAktualizacje = false;

    while (std::getline(plik, linia))
    {
        if (linia.empty())
        {
            sekcjaAktualizacje = true;
            continue;
        }

        if (!sekcjaAktualizacje)
        {
            // Przetwarzanie regu� w formacie X|Y
            auto rozdzielone = linia | std::views::split('|') | std::views::transform([](auto&& czesc) {
                                   return std::stoi(std::string{czesc.begin(), czesc.end()});
                               });

            auto iter = rozdzielone.begin();
            int  x    = *iter++;
            int  y    = *iter;

            reguly.emplace_back(x, y); // Dodanie regu�y do kontenera
        }
        else
        {
            // Przetwarzanie aktualizacji stron
            std::vector< int > aktualizacja;
            auto               liczby = linia | std::views::split(',') | std::views::transform([](auto&& czesc) {
                              return std::stoi(std::string{czesc.begin(), czesc.end()});
                          });

            for (int liczba : liczby)
            {
                aktualizacja.push_back(liczba);
            }

            aktualizacje.push_back(aktualizacja);
        }
    }

    plik.close(); // Zamkni�cie pliku po zako�czeniu przetwarzania
}

// Funkcja sprawdzaj�ca poprawno�� jednej aktualizacji
bool czyPoprawnaAktualizacja(const std::vector< int >& aktualizacja, const std::vector< std::pair< int, int > >& reguly)
{
    for (const auto& [a, b] : reguly)
    {
        auto itA = std::ranges::find(aktualizacja, a);
        auto itB = std::ranges::find(aktualizacja, b);

        if (itA != aktualizacja.end() && itB != aktualizacja.end())
        {
            if (itA > itB)
            {
                return false; // Je�li strona `a` wyst�puje po stronie `b`, regu�a jest z�amana
            }
        }
    }
    return true; // Wszystkie regu�y zosta�y spe�nione
}

// Funkcja obliczaj�ca sum� �rodkowych stron z poprawnych aktualizacji
int obliczSumePoprawnych(const std::vector< std::vector< int > >&    aktualizacje,
                         const std::vector< std::pair< int, int > >& reguly)
{
    int suma = 0;

    for (const auto& aktualizacja : aktualizacje)
    {
        if (czyPoprawnaAktualizacja(aktualizacja, reguly))
        {
            int srodek = aktualizacja[aktualizacja.size() / 2]; // Znajd� �rodkowy element
            suma += srodek;                                     // Dodaj go do sumy
            std::cout << "Poprawna aktualizacja: ";
            for (int strona : aktualizacja)
            {
                std::cout << strona << " ";
            }
            std::cout << "| �rodkowy element: " << srodek << "\n";
        }
        else
        {
            std::cout << "Niepoprawna aktualizacja: ";
            for (int strona : aktualizacja)
            {
                std::cout << strona << " ";
            }
            std::cout << "\n";
        }
    }

    return suma;
}

// G��wna funkcja analizuj�ca kolejk� drukowania
void wykonajAnalizeKolejki(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza przyk�adu
    std::cout << "=== Analiza przyk�adu ===\n";
    std::vector< std::pair< int, int > > regulyPrzyklad;
    std::vector< std::vector< int > >    aktualizacjePrzyklad;

    wczytajDane(sciezkaPrzykladu, regulyPrzyklad, aktualizacjePrzyklad);
    int wynikPrzyklad = obliczSumePoprawnych(aktualizacjePrzyklad, regulyPrzyklad);

    std::cout << "Suma dla przyk�adu: " << wynikPrzyklad << "\n";
    std::cout << "Aby kontynuowa� analiz� pliku wej�ciowego, naci�nij Enter...\n";
    std::cin.get();

    // Analiza pliku wej�ciowego
    std::cout << "=== Analiza pliku wej�ciowego ===\n";
    std::vector< std::pair< int, int > > regulyPlik;
    std::vector< std::vector< int > >    aktualizacjePlik;

    wczytajDane(sciezkaPliku, regulyPlik, aktualizacjePlik);
    int wynikPlik = obliczSumePoprawnych(aktualizacjePlik, regulyPlik);

    std::cout << "Suma dla pliku wej�ciowego: " << wynikPlik << "\n";
}