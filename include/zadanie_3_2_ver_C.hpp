#include <fstream>  // std::ifstream
#include <iostream> // std::cout, std::cerr
#include <string>   // std::string

// matmê wzi¹³em z https://github.com/PaigePalisade/AdventOfCode2024/blob/main/Solutions/day03part1.c
// Funkcja analizuj¹ca zawartoœæ pliku i sumuj¹ca poprawne mno¿enia
int obliczSumeMnozen(const std::string& sciezkaPliku)
{
    std::ifstream plik(sciezkaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie mo¿na otworzyæ pliku: " << sciezkaPliku << "\n";
        return 0;
    }

    char zawartosc[20000];
    plik.read(zawartosc, sizeof(zawartosc));
    int dlugosc = plik.gcount();
    plik.close();

    bool mulAktywne = true; // Zmienna okreœlaj¹ca stan mno¿enia
    int  suma       = 0;    // Wynik sumowania

    for (int i = 0; i < dlugosc - 4; i++)
    {
        if (strncmp(zawartosc + i, "mul(", 4) == 0) // Znaleziono "mul("
        {
            i += 4;
            int a = 0, b = 0;

            // Wczytaj pierwszy argument
            while (zawartosc[i] >= '0' && zawartosc[i] <= '9')
            {
                a = a * 10 + (zawartosc[i] - '0');
                i++;
            }

            if (zawartosc[i] == ',') // Oczekiwany przecinek
            {
                i++;
                // Wczytaj drugi argument
                while (zawartosc[i] >= '0' && zawartosc[i] <= '9')
                {
                    b = b * 10 + (zawartosc[i] - '0');
                    i++;
                }
            }

            if (zawartosc[i] == ')' && mulAktywne) // Poprawna instrukcja "mul(X,Y)"
            {
                suma += a * b;
            }
        }
        else if (strncmp(zawartosc + i, "do()", 4) == 0) // Znaleziono "do()"
        {
            mulAktywne = true;
            i += 3; // Przesuñ indeks
        }
        else if (strncmp(zawartosc + i, "don't()", 7) == 0) // Znaleziono "don't()"
        {
            mulAktywne = false;
            i += 6; // Przesuñ indeks
        }
    }

    return suma; // Zwróæ wynik
}

// Funkcja g³ówna do analizy pliku
void wykonajAnalizeMnozen(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza danych z przyk³adu
    std::cout << "=== Analiza danych z pliku z przyk³adem ===\n";
    int wynikPrzykladu = obliczSumeMnozen(sciezkaPrzykladu);
    std::cout << "Suma mno¿eñ w przyk³adzie: " << wynikPrzykladu << "\n";

    // Wymaganie potwierdzenia przed analiz¹ pe³nych danych
    std::cout << "\nAby kontynuowaæ i przeanalizowaæ dane z pliku wejœciowego, naciœnij Enter...\n";
    std::cin.get();

    // Analiza danych z pliku wejœciowego
    std::cout << "=== Analiza danych z pliku wejœciowego ===\n";
    int wynikPliku = obliczSumeMnozen(sciezkaPliku);
    std::cout << "Suma mno¿eñ w pliku wejœciowym: " << wynikPliku << "\n";
}
