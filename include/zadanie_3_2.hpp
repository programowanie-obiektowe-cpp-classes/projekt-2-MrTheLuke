#include <fstream>  // std::ifstream
#include <iostream> // std::cout, std::cerr
#include <regex>    // std::regex, std::smatch
#include <string>   // std::string

// Funkcja analizuj¹ca zawartoœæ pliku i sumuj¹ca poprawne mno¿enia
int obliczSumeMnozen(const std::string& sciezkaPliku)
{
    std::ifstream plik(sciezkaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie mo¿na otworzyæ pliku: " << sciezkaPliku << "\n";
        return 0;
    }

    std::string zawartosc; // Zmienna przechowuj¹ca dane z pliku
    std::string linia;

    while (std::getline(plik, linia)) // Wczytaj plik linia po linii
    {
        zawartosc += linia;
    }

    plik.close(); // Zamknij plik po wczytaniu danych

    // Regex do wykrywania instrukcji mul(X,Y), do() oraz don't()
    std::regex  wzorzecMul(R"(mul\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\))");
    std::regex  wzorzecDo(R"(do\(\))");
    std::regex  wzorzecDont(R"(don't\(\))");
    std::smatch dopasowanie;

    bool mulAktywne = true; // Zmienna okreœlaj¹ca, czy mno¿enie jest aktywne
    int  suma       = 0;    // Zmienna przechowuj¹ca wynik sumowania poprawnych mno¿eñ
    auto poczatek   = zawartosc.cbegin();

    // Przetwarzaj dane wejœciowe
    while (poczatek != zawartosc.cend())
    {
        // SprawdŸ, czy znaleziono instrukcjê `do()` lub `don't()`
        if (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzecDo))
        {
            mulAktywne = true;
            std::cout << "Znaleziono instrukcjê: do() | Mno¿enie w³¹czone\n";
            poczatek = dopasowanie.suffix().first;
            continue;
        }
        else if (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzecDont))
        {
            mulAktywne = false;
            std::cout << "Znaleziono instrukcjê: don't() | Mno¿enie wy³¹czone\n";
            poczatek = dopasowanie.suffix().first;
            continue;
        }
        // SprawdŸ, czy znaleziono poprawn¹ instrukcjê `mul(X,Y)`
        else if (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzecMul))
        {
            if (mulAktywne) // Uwzglêdniaj tylko, gdy mno¿enie jest aktywne
            {
                int x = std::stoi(dopasowanie[1].str());
                int y = std::stoi(dopasowanie[2].str());
                suma += x * y;
                std::cout << "Znaleziono instrukcjê: " << dopasowanie[0] << " | Wynik: " << x * y << "\n";
            }
            else
            {
                std::cout << "Pominiêto instrukcjê: " << dopasowanie[0] << " (mno¿enie wy³¹czone)\n";
            }
            poczatek = dopasowanie.suffix().first;
            continue;
        }
        // Przesuñ siê o jeden znak, jeœli nie znaleziono dopasowania
        ++poczatek;
    }

    return suma; // Zwróæ koñcowy wynik sumowania
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
