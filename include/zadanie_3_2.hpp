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

    // Regex do wykrywania instrukcji mul(X,Y), do() oraz don't()
    std::regex  wzorzecMul(R"(mul\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\))");
    std::regex  wzorzecDo(R"(do\(\))");
    std::regex  wzorzecDont(R"(don't\(\))");
    std::smatch dopasowanie;

    bool mulAktywne = true; // Zmienna okre�laj�ca, czy mno�enie jest aktywne
    int  suma       = 0;    // Zmienna przechowuj�ca wynik sumowania poprawnych mno�e�
    auto poczatek   = zawartosc.cbegin();

    // Przetwarzaj dane wej�ciowe
    while (poczatek != zawartosc.cend())
    {
        // Sprawd�, czy znaleziono instrukcj� `do()` lub `don't()`
        if (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzecDo))
        {
            mulAktywne = true;
            std::cout << "Znaleziono instrukcj�: do() | Mno�enie w��czone\n";
            poczatek = dopasowanie.suffix().first;
            continue;
        }
        else if (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzecDont))
        {
            mulAktywne = false;
            std::cout << "Znaleziono instrukcj�: don't() | Mno�enie wy��czone\n";
            poczatek = dopasowanie.suffix().first;
            continue;
        }
        // Sprawd�, czy znaleziono poprawn� instrukcj� `mul(X,Y)`
        else if (std::regex_search(poczatek, zawartosc.cend(), dopasowanie, wzorzecMul))
        {
            if (mulAktywne) // Uwzgl�dniaj tylko, gdy mno�enie jest aktywne
            {
                int x = std::stoi(dopasowanie[1].str());
                int y = std::stoi(dopasowanie[2].str());
                suma += x * y;
                std::cout << "Znaleziono instrukcj�: " << dopasowanie[0] << " | Wynik: " << x * y << "\n";
            }
            else
            {
                std::cout << "Pomini�to instrukcj�: " << dopasowanie[0] << " (mno�enie wy��czone)\n";
            }
            poczatek = dopasowanie.suffix().first;
            continue;
        }
        // Przesu� si� o jeden znak, je�li nie znaleziono dopasowania
        ++poczatek;
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
