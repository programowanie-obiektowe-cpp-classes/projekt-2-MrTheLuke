#include <algorithm> //std::sort - do sortowania element�w list
#include <cmath> //std::abs - obliczanie warto�ci bezwzgl�dnej r�nic mi�dzy elementami list.
#include <fstream> //obs�uga plik�w, wczytywanie danych z pliku za pomoc� std::ifstream
#include <iostream> //obs�uga wej�cia i wyj�cia, std::cout i std::cerr
#include <vector> //korzystanie z dynamicznych tablic (kontener std::vector)


/*
Zadanie 1_1:

For example:

3   4
4   3
2   5
1   3
3   9
3   3
Maybe the lists are only off by a small amount! To find out, pair up the numbers and measure how far apart they are.
Pair up the smallest number in the left list with the smallest number in the right list, then the second-smallest left
number with the second-smallest right number, and so on.

Within each pair, figure out how far apart the two numbers are; you'll need to add up all of those distances. For
example, if you pair up a 3 from the left list with a 7 from the right list, the distance apart is 4; if you pair up a 9
with a 3, the distance apart is 6.

In the example list above, the pairs and distances would be as follows:

The smallest number in the left list is 1, and the smallest number in the right list is 3. The distance between them
is 2. The second-smallest number in the left list is 2, and the second-smallest number in the right list is another 3.
The distance between them is 1. The third-smallest number in both lists is 3, so the distance between them is 0. The
next numbers to pair up are 3 and 4, a distance of 1. The fifth-smallest numbers in each list are 3 and 5, a distance
of 2. Finally, the largest number in the left list is 4, while the largest number in the right list is 9; these are a
distance 5 apart. To find the total distance between the left list and the right list, add up the distances between all
of the pairs you found. In the example above, this is 2 + 1 + 0 + 1 + 2 + 5, a total distance of 11!

Your actual left and right lists contain many location IDs. What is the total distance between your lists?




program obliczaj�cy ca�kowit� r�nic� pomi�dzy dwoma listami liczb
po dopasowaniu ich w pary wed�ug rosn�cego porz�dku.
*/

// Funkcja obliczaj�ca ca�kowit� r�nic� mi�dzy dwiema listami
int obliczRoznice(const std::vector< int >& listaLewa, const std::vector< int >& listaPrawa)
{
    std::vector< int > lewa(listaLewa); //tworzenie wektor�w do list z liczbami
    std::vector< int > prawa(listaPrawa);
    std::sort(lewa.begin(), lewa.end()); //sortowanie element�w wektora
    std::sort(prawa.begin(), prawa.end());
    int sumaRoznic = 0;
    for (size_t i = 0; i < lewa.size(); ++i)
    {
        sumaRoznic += std::abs(lewa[i] - prawa[i]); // naliczanie r�nicy mi�dzy elementami co wiersz
    }
    return sumaRoznic;
}

// Funkcja wczytuj�ca dane z pliku
bool wczytajDaneZPliku(const std::string& nazwaPliku, std::vector< int >& listaLewa, std::vector< int >& listaPrawa)
{
    std::ifstream plik(nazwaPliku); // Tworzenie obiektu do odczytu pliku
    if (!plik.is_open())
    {
        std::cerr << "Nie mo�na otworzy� pliku: " << nazwaPliku << std::endl;
        return false;
    }
    int lewa, prawa;
    while (plik >> lewa >> prawa) // Wczytywanie danych
    {
        listaLewa.push_back(lewa); // Dodawanie elementu do wektora
        listaPrawa.push_back(prawa);
    }
    plik.close();
    return true;
}

void wykonajObliczenia(const std::string& sciezkaPliku)
{
    std::vector< int > listaLewa;  // Tworzenie dynamicznych kontener�w
    std::vector< int > listaPrawa;
    if (!wczytajDaneZPliku(sciezkaPliku, listaLewa, listaPrawa))
    {
        return;
    }
    if (listaLewa.size() != listaPrawa.size()) //sprawdzenie czy dobrze wpisano kolumny z pliku do wektor�w
    {
        std::cerr << "Listy maj� r�n� d�ugo��!" << std::endl;
        return;
    }
    int wynik = obliczRoznice(listaLewa, listaPrawa); 
    std::cout << "Ca�kowita r�nica: " << wynik << std::endl; //drukowanie wyniku
}