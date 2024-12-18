#include <algorithm> // std::ranges::find, std::ranges::all_of, std::ranges::sort
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

// Funkcja porz�dkuj�ca aktualizacj� na podstawie regu�
std::vector< int > uporzadkujAktualizacje(const std::vector< int >&                   aktualizacja,
                                          const std::vector< std::pair< int, int > >& reguly)
{
    std::vector< int > posortowana = aktualizacja;

    bool zmieniono;
    do
    {
        zmieniono = false;

        for (const auto& [a, b] : reguly)
        {
            auto itA = std::ranges::find(posortowana, a);
            auto itB = std::ranges::find(posortowana, b);

            // Je�li oba elementy s� w aktualizacji i s� w z�ej kolejno�ci, zamie� je miejscami
            if (itA != posortowana.end() && itB != posortowana.end() && itA > itB)
            {
                std::iter_swap(itA, itB);
                zmieniono = true;
            }
        }
    } while (zmieniono); // Powtarzaj a� ca�a aktualizacja b�dzie zgodna z regu�ami

    return posortowana;
}

// Funkcja obliczaj�ca sum� �rodkowych stron z uporz�dkowanych aktualizacji
int obliczSumeNiepoprawnych(const std::vector< std::vector< int > >&    aktualizacje,
                            const std::vector< std::pair< int, int > >& reguly)
{
    int suma = 0;

    for (const auto& aktualizacja : aktualizacje)
    {
        // Sprawd�, czy aktualizacja jest poprawna
        bool poprawna = true;
        for (const auto& [a, b] : reguly)
        {
            auto itA = std::ranges::find(aktualizacja, a);
            auto itB = std::ranges::find(aktualizacja, b);

            if (itA != aktualizacja.end() && itB != aktualizacja.end() && itA > itB)
            {
                poprawna = false;
                break;
            }
        }

        if (!poprawna)
        {
            // Porz�dkuj aktualizacj�, je�li by�a niepoprawna
            std::vector< int > posortowana = uporzadkujAktualizacje(aktualizacja, reguly);

            // Dodaj �rodkowy element do sumy
            int srodek = posortowana[posortowana.size() / 2];
            suma += srodek;

            // Wy�wietl poprawion� aktualizacj�
            std::cout << "Niepoprawna aktualizacja: ";
            for (int strona : aktualizacja)
            {
                std::cout << strona << " ";
            }
            std::cout << "\nPoprawiona aktualizacja: ";
            for (int strona : posortowana)
            {
                std::cout << strona << " ";
            }
            std::cout << "| �rodkowy element: " << srodek << "\n";
        }
    }

    return suma;
}

// G��wna funkcja analizuj�ca kolejk� drukowania dla zadania 5.2
void wykonajAnalizeKolejki(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza przyk�adu
    std::cout << "=== Analiza przyk�adu ===\n";
    std::vector< std::pair< int, int > > regulyPrzyklad;
    std::vector< std::vector< int > >    aktualizacjePrzyklad;

    wczytajDane(sciezkaPrzykladu, regulyPrzyklad, aktualizacjePrzyklad);
    int wynikPrzyklad = obliczSumeNiepoprawnych(aktualizacjePrzyklad, regulyPrzyklad);

    std::cout << "Suma dla niepoprawnych aktualizacji w przyk�adzie: " << wynikPrzyklad << "\n";
    std::cout << "Aby kontynuowa� analiz� pliku wej�ciowego, naci�nij Enter...\n";
    std::cin.get();

    // Analiza pliku wej�ciowego
    std::cout << "=== Analiza pliku wej�ciowego ===\n";
    std::vector< std::pair< int, int > > regulyPlik;
    std::vector< std::vector< int > >    aktualizacjePlik;

    wczytajDane(sciezkaPliku, regulyPlik, aktualizacjePlik);
    int wynikPlik = obliczSumeNiepoprawnych(aktualizacjePlik, regulyPlik);

    std::cout << "Suma dla niepoprawnych aktualizacji w pliku wej�ciowym: " << wynikPlik << "\n";
}