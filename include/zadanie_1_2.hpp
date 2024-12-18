/*
This time, you'll need to figure out exactly how often each number from the left list appears in the right list.
Calculate a total similarity score by adding up each number in the left list after multiplying it by the number of times
that number appears in the right list.

Here are the same example lists again:

3   4
4   3
2   5
1   3
3   9
3   3
For these example lists, here is the process of finding the similarity score:

The first number in the left list is 3. It appears in the right list three times, so the similarity score increases by 3
* 3 = 9. The second number in the left list is 4. It appears in the right list once, so the similarity score increases
by 4 * 1 = 4. The third number in the left list is 2. It does not appear in the right list, so the similarity score does
not increase (2 * 0 = 0). The fourth number, 1, also does not appear in the right list. The fifth number, 3, appears in
the right list three times; the similarity score increases by 9. The last number, 3, appears in the right list three
times; the similarity score again increases by 9. So, for these example lists, the similarity score at the end of this
process is 31 (9 + 4 + 0 + 0 + 9 + 9).

Once again consider your left and right lists. What is their similarity score?




*/





#include <algorithm> // std::count - do liczenia wyst�pie� elementu w li�cie
#include <fstream>   // std::ifstream - do obs�ugi plik�w wej�ciowych
#include <iostream>  // std::cout, std::cerr - do obs�ugi wej�cia/wyj�cia
#include <string>    // std::string - do obs�ugi nazw plik�w
#include <vector>    // std::vector - do przechowywania dynamicznych list

/*

Przyklad:
Dla list:
3   4
4   3
2   5
1   3
3   9
3   3

Dopasowanie liczb wed�ug rosn�cego porz�dku i obliczanie r�nic:
- 1 (z lewej listy) i 3 (z prawej listy) -> r�nica: 2
- 2 (z lewej listy) i 3 (z prawej listy) -> r�nica: 1
- 3 (z lewej listy) i 3 (z prawej listy) -> r�nica: 0
- 3 (z lewej listy) i 4 (z prawej listy) -> r�nica: 1
- 3 (z lewej listy) i 5 (z prawej listy) -> r�nica: 2
- 4 (z lewej listy) i 9 (z prawej listy) -> r�nica: 5
Ca�kowita r�nica: 2 + 1 + 0 + 1 + 2 + 5 = 11.
*/

// Funkcja licz�ca wynik podobie�stwa mi�dzy dwiema listami
int obliczPodobienstwo(const std::vector< int >& listaLewa, const std::vector< int >& listaPrawa)
{
    int sumaPodobienstwa = 0;


    for (size_t i = 0; i < listaLewa.size(); ++i)
    {
        int liczba = listaLewa[i]; // Aktualna liczba z lewej listy

        // Liczenie, ile razy liczba z lewej listy pojawia si� na prawej li�cie
        int wystapienia = std::count(listaPrawa.begin(), listaPrawa.end(), liczba);

        // Aktualizacja wyniku podobie�stwa o (liczba * liczba wyst�pie�)
        sumaPodobienstwa += liczba * wystapienia;

        // Drukowanie szczeg��w oblicze� dla debugowania
        std::cout << "Liczba: " << liczba << ", Wyst�pienia: " << wystapienia << ", Aktualna suma: " << sumaPodobienstwa
                  << std::endl;
    }

    return sumaPodobienstwa; // Zwracanie ko�cowego wyniku
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
    std::vector< int > listaLewa; // Tworzenie dynamicznych kontener�w
    std::vector< int > listaPrawa;
    if (!wczytajDaneZPliku(sciezkaPliku, listaLewa, listaPrawa))
    {
        return;
    }
    if (listaLewa.size() != listaPrawa.size()) // sprawdzenie czy dobrze wpisano kolumny z pliku do wektor�w
    {
        std::cerr << "Listy maj� r�n� d�ugo��!" << std::endl;
        return;
    }

    int wynik = obliczPodobienstwo(listaLewa, listaPrawa); // wynik ko�cowy
    std::cout << "Wynik podobie�stwa: " << wynik << std::endl;
}