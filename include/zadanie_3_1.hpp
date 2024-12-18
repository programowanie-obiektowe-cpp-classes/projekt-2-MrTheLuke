#include <fstream>  // std::ifstream
#include <iostream> // std::cout, std::cerr
#include <regex>    // std::regex, std::smatch
#include <string>   // std::string

// Funkcja analizuj�ca zawarto�� pliku i sumuj�ca poprawne mno�enia
int obliczSumeMnozen(const std::string& sciezkaPliku)
{
    std::ifstream plik(sciezkaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie mo�na otworzy� pliku: " << sciezkaPliku << "\n";
        return 0;
    }

    std::string zawartosc; // Zmienna przechowuj�ca dane z pliku
    std::string linia;

    while (std::getline(plik, linia)) // Wczytaj plik linia po linii
    {
        zawartosc += linia;
    }

    plik.close(); // Zamknij plik po wczytaniu danych

    // Regex do wykrywania poprawnych instrukcji w formacie `mul(X,Y)`
    std::regex  wzorzec(R"(mul\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\))");
    std::smatch dopasowanie; // Obiekt do przechowywania wynik�w dopasowania

    int  suma     = 0; // Zmienna przechowuj�ca wynik sumowania poprawnych mno�e�
    auto poczatek = zawartosc.cbegin();

    // Przetwarzaj dane wej�ciowe i znajd� wszystkie poprawne instrukcje
    while (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzec))
    {
        int x = std::stoi(dopasowanie[1].str()); // Pierwszy argument mno�enia
        int y = std::stoi(dopasowanie[2].str()); // Drugi argument mno�enia

        suma += x * y; // Dodaj wynik mno�enia do sumy
        std::cout << "Znaleziono instrukcj�: " << dopasowanie[0] << " | Wynik: " << x * y << "\n";

        poczatek = dopasowanie.suffix().first; // Przejd� do dalszej cz�ci danych
    }

    return suma; // Zwr�� ko�cowy wynik sumowania
}

// Funkcja g��wna do analizy pliku
void wykonajAnalizeMnozen(const std::string& sciezkaPrzykladu, const std::string& sciezkaPliku)
{
    // Analiza danych z przyk�adu
    std::cout << "=== Analiza danych z pliku z przyk�adem ===\n";
    int wynikPrzykladu = obliczSumeMnozen(sciezkaPrzykladu);
    std::cout << "Suma mno�e� w przyk�adzie: " << wynikPrzykladu << "\n";

    // Wymaganie potwierdzenia przed analiz� pe�nych danych
    std::cout << "\nAby kontynuowa� i przeanalizowa� dane z pliku wej�ciowego, naci�nij Enter...\n";
    std::cin.get();

    // Analiza danych z pliku wej�ciowego
    std::cout << "=== Analiza danych z pliku wej�ciowego ===\n";
    int wynikPliku = obliczSumeMnozen(sciezkaPliku);
    std::cout << "Suma mno�e� w pliku wej�ciowym: " << wynikPliku << "\n";
}