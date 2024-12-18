/*
Now, the same rules apply as before, except if removing a single level from an unsafe report would make it safe, the
report instead counts as safe.

More of the above example's reports are now safe:

7 6 4 2 1: Safe without removing any level.
1 2 7 8 9: Unsafe regardless of which level is removed.
9 7 6 2 1: Unsafe regardless of which level is removed.
1 3 2 4 5: Safe by removing the second level, 3.
8 6 4 4 1: Safe by removing the third level, 4.
1 3 6 7 9: Safe without removing any level.
Thanks to the Problem Dampener, 4 reports are actually safe!

Update your analysis by handling situations where the Problem Dampener can remove a single level from unsafe reports.
How many reports are now safe?
*/


#include <fstream>  // std::ifstream - do obs³ugi plików wejœciowych
#include <iostream> // std::cout, std::cerr - do obs³ugi wejœcia/wyjœcia
#include <ranges>   // std::ranges - operacje na sekwencjach danych
#include <string>   // std::string - do przechowywania nazw plików
#include <vector>   // std::vector - dynamiczne kontenery na poziomy raportów

/*
Przyk³ad raportów:
7 6 4 2 1 - bezpieczny bez usuwania poziomu
1 2 7 8 9 - niebezpieczny niewa¿ne co usuniemy
9 7 6 2 1 - niebezpieczny niewa¿ne co usuniemy
1 3 2 4 5 - bezpieczny gdy usuniemy poziom drugi poziom (3)
8 6 4 4 1 - bezpieczny gdy usuniemy poziom trzeci poziom (4)
1 3 6 7 9 - bezpieczny bez usuwania poziomu

Analiza:
Raporty bezpieczne spe³niaj¹ dwa warunki:
1. Wszystkie poziomy s¹ albo rosn¹ce, albo malej¹ce.
2. Ka¿da ró¿nica miêdzy s¹siaduj¹cymi poziomami mieœci siê w przedziale [1, 3].

poprzez Problem Dampenera:
- Dopuszczamy usuniêcie jednego poziomu, aby raport spe³nia³ zasady bezpieczeñstwa.
*/

// Funkcja sprawdzaj¹ca, czy raport jest bezpieczny (wazrunek wzrostu lub spadku w przedziale ró¿nic [1,3])
bool czyRaportBezpieczny(const std::vector< int >& poziomy)
{
    // Jeœli raport zawiera mniej ni¿ dwa poziomy, jest niebezpieczny
    if (poziomy.size() < 2)
    {
        std::cout << "Raport ma mniej ni¿ dwa poziomy, wiêc jest niebezpieczny.\n";
        return false;
    }

    // Ustalenie kierunku (rosn¹cy lub malej¹cy)
    bool rosnacy  = poziomy[1] > poziomy[0];
    bool malejacy = poziomy[1] < poziomy[0];

    // Jeœli poziomy nie s¹ wyraŸnie rosn¹ce ani malej¹ce, raport jest niebezpieczny
    if (!rosnacy && !malejacy)
    {
        std::cout << "Poziomy nie s¹ ani wyraŸnie rosn¹ce, ani malej¹ce.\n";
        return false;
    }

    // Sprawdzanie ró¿nic miêdzy kolejnymi poziomami
    for (size_t i = 1; i < poziomy.size(); ++i)
    {
        int roznica = poziomy[i] - poziomy[i - 1];
        // Sprawdzenie warunków dla rosn¹cego kierunku
        if (rosnacy && (roznica <= 0 || roznica > 3))
        {
            std::cout << "Nieprawid³owa ró¿nica przy wzroœcie: " << roznica << " (poziom " << poziomy[i - 1] << " -> "
                      << poziomy[i] << ").\n";
            return false;
        }
        // Sprawdzenie warunków dla malej¹cego kierunku
        if (malejacy && (roznica >= 0 || roznica < -3))
        {
            std::cout << "Nieprawid³owa ró¿nica przy spadku: " << roznica << " (poziom " << poziomy[i - 1] << " -> "
                      << poziomy[i] << ").\n";
            return false;
        }
    }

    std::cout << "Raport jest bezpieczny.\n";
    return true;
}

// Funkcja sprawdzaj¹ca raport z uwzglêdnieniem Problemu Dampenera
// Jeœli usuniêcie jednego poziomu pozwala na spe³nienie zasad, raport jest uznawany za bezpieczny
bool czyRaportBezpiecznyZDampenerem(const std::vector< int >& poziomy)
{
    std::cout << "\nSprawdzanie raportu z Dampenerem...\n";
    if (czyRaportBezpieczny(poziomy))
    {
        std::cout << "Raport jest bezpieczny bez u¿ycia Dampenera.\n";
        return true;
    }

    // Iteracja po ka¿dym poziomie raportu
    for (size_t i = 0; i < poziomy.size(); ++i)
    {
        std::cout << "Usuwanie poziomu: " << poziomy[i] << "\n";

        std::vector< int > kopiaPoziomow = poziomy; // Tworzenie kopii oryginalnego raportu

        // Usuwanie poziomu na pozycji i
        kopiaPoziomow.erase(kopiaPoziomow.begin() + i);

        // Sprawdzanie, czy po usuniêciu poziomu raport jest bezpieczny
        if (czyRaportBezpieczny(kopiaPoziomow))
        {
            std::cout << "Raport staje siê bezpieczny po usuniêciu poziomu: " << poziomy[i] << "\n";
            return true;
        }
    }

    std::cout << "Raport nie jest bezpieczny nawet z u¿yciem Dampenera.\n";
    return false;
}

// Funkcja analizuj¹ca raporty z pliku
// Iteruje po ka¿dej linii w pliku, przetwarza dane i sprawdza bezpieczeñstwo raportów
int analizujRaportyZDampenerem(const std::string& nazwaPliku)
{
    std::ifstream plik(nazwaPliku);
    if (!plik.is_open())
    {
        std::cerr << "Nie mo¿na otworzyæ pliku: " << nazwaPliku << std::endl;
        return -1;
    }

    int         liczbaBezpiecznych = 0; // Licznik bezpiecznych raportów
    std::string linia;

    while (std::getline(plik, linia))
    {
        std::cout << "\nAnaliza raportu: " << linia << "\n";

        std::vector< int > poziomy;

        // Podzia³ linii na liczby i konwersja na int (std::ranges z 2.1)
        for (auto&& czesc : linia | std::views::split(' '))
        {
            std::string token(czesc.begin(), czesc.end());
            poziomy.push_back(std::stoi(token));
        }

        std::cout << "Poziomy w raporcie: ";
        for (size_t i = 0; i < poziomy.size(); ++i)
        {
            std::cout << poziomy[i] << (i < poziomy.size() - 1 ? ", " : "\n");
        }

        // Sprawdzenie bezpieczeñstwa z Dampenerem
        if (czyRaportBezpiecznyZDampenerem(poziomy))
        {
            ++liczbaBezpiecznych;
            std::cout << "Raport oznaczony jako bezpieczny.\n";
        }
        else
        {
            std::cout << "Raport oznaczony jako niebezpieczny.\n";
        }
    }

    plik.close();
    return liczbaBezpiecznych;
}

// Funkcja g³ówna do wywo³ania analizy raportów
void wykonajAnalizeRaportowZDampenerem(const std::string& sciezkaPliku)
{
    int wynik = analizujRaportyZDampenerem(sciezkaPliku);
    if (wynik >= 0)
    {
        std::cout << "\nLiczba bezpiecznych raportów (z Dampenerem): " << wynik << std::endl;
    }
}