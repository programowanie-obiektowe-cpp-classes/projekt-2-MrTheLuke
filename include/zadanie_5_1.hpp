#include <algorithm> // std::ranges::find, std::ranges::all_of
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::cerr
#include <ranges>    // std::views::split, std::views::transform
#include <sstream>   // std::istringstream
#include <string>    // std::string
#include <vector>    // std::vector

// Funkcja wczytuj¹ca regu³y i listy stron z pliku
void wczytajDane(const std::string&                    sciezkaPliku,
                 std::vector< std::pair< int, int > >& reguly,
                 std::vector< std::vector< int > >&    aktualizacje)
{
    std::ifstream plik(sciezkaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie uda³o siê otworzyæ pliku: " << sciezkaPliku << "\n";
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
            // Przetwarzanie regu³ w formacie X|Y
            auto rozdzielone = linia | std::views::split('|') | std::views::transform([](auto&& czesc) {
                                   return std::stoi(std::string{czesc.begin(), czesc.end()});
                               });

            auto iter = rozdzielone.begin();
            int  x    = *iter++;
            int  y    = *iter;

            reguly.emplace_back(x, y); // Dodanie regu³y do kontenera
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

    plik.close(); // Zamkniêcie pliku po zakoñczeniu przetwarzania
}

// Funkcja sprawdzaj¹ca poprawnoœæ jednej aktualizacji
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
                return false; // Jeœli strona `a` wystêpuje po stronie `b`, regu³a jest z³amana
            }
        }
    }
    return true; // Wszystkie regu³y zosta³y spe³nione
}

// Funkcja obliczaj¹ca sumê œrodkowych stron z poprawnych aktualizacji
int obliczSumePoprawnych(const std::vector< std::vector< int > >&    aktualizacje,
                         const std::vector< std::pair< int, int > >& reguly)
{
    int suma = 0;

    for (const auto& aktualizacja : aktualizacje)
    {
        if (czyPoprawnaAktualizacja(aktualizacja, reguly))
        {
            int srodek = aktualizacja[aktualizacja.size() / 2]; // ZnajdŸ œrodkowy element
            suma += srodek;                                     // Dodaj go do sumy
            std::cout << "Poprawna aktualizacja: ";
            for (int strona : aktualizacja)
            {
                std::cout << strona << " ";
            }
            std::cout << "| Œrodkowy element: " << srodek << "\n";
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

// G³ówna funkcja analizuj¹ca kolejkê drukowania
void wykonajAnalizeKolejki(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza przyk³adu
    std::cout << "=== Analiza przyk³adu ===\n";
    std::vector< std::pair< int, int > > regulyPrzyklad;
    std::vector< std::vector< int > >    aktualizacjePrzyklad;

    wczytajDane(sciezkaPrzykladu, regulyPrzyklad, aktualizacjePrzyklad);
    int wynikPrzyklad = obliczSumePoprawnych(aktualizacjePrzyklad, regulyPrzyklad);

    std::cout << "Suma dla przyk³adu: " << wynikPrzyklad << "\n";
    std::cout << "Aby kontynuowaæ analizê pliku wejœciowego, naciœnij Enter...\n";
    std::cin.get();

    // Analiza pliku wejœciowego
    std::cout << "=== Analiza pliku wejœciowego ===\n";
    std::vector< std::pair< int, int > > regulyPlik;
    std::vector< std::vector< int > >    aktualizacjePlik;

    wczytajDane(sciezkaPliku, regulyPlik, aktualizacjePlik);
    int wynikPlik = obliczSumePoprawnych(aktualizacjePlik, regulyPlik);

    std::cout << "Suma dla pliku wejœciowego: " << wynikPlik << "\n";
}