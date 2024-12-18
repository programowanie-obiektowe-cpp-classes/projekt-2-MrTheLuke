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

    // Regex do wykrywania poprawnych instrukcji w formacie `mul(X,Y)`
    std::regex  wzorzec(R"(mul\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\))");
    std::smatch dopasowanie; // Obiekt do przechowywania wyników dopasowania

    int  suma     = 0; // Zmienna przechowuj¹ca wynik sumowania poprawnych mno¿eñ
    auto poczatek = zawartosc.cbegin();

    // Przetwarzaj dane wejœciowe i znajdŸ wszystkie poprawne instrukcje
    while (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzec))
    {
        int x = std::stoi(dopasowanie[1].str()); // Pierwszy argument mno¿enia
        int y = std::stoi(dopasowanie[2].str()); // Drugi argument mno¿enia

        suma += x * y; // Dodaj wynik mno¿enia do sumy
        std::cout << "Znaleziono instrukcjê: " << dopasowanie[0] << " | Wynik: " << x * y << "\n";

        poczatek = dopasowanie.suffix().first; // PrzejdŸ do dalszej czêœci danych
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