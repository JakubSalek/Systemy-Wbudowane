# Embedded Systems ENG

This is my code for DSM-51 embedded system. It is a project I had to do to pass my clasess. I got 107 out of 110 possible points for it. Meaning I got the best grade.

# Systemy wpudowane PL

Jest to kod do mojego programu na system dydaktyczny DSM-51. Praca została mi zadana jako projekt na zaliczenie, z której finalnie otrzymałem 107 na 110 punktów i najlepszą z dostępnych ocen.

PROJEKT KOŃCOWY
W ramach projektu końcowego student powinien wykazać się umiejętnością obsługi elementów składowych systemu
mikroprocesorowego DSM-51. W ramach oceny z projektu punkty przyznawane są w kategoriach:
1. Układ czasowo-licznikowy T0.
1.1.(!) (30 pkt.)
Gdy zapewnia sygnał okresowy o częstotliwości 50Hz na wyprowadzeniach (wszystkich ośmiu) portu B układu 8255 systemu
DSM51. Stan wysoki utrzymuje się przez czas określony procentowo parametrem wypełnienia PWM regulowanym w zakresie 3 –
12% (stan niski utrzymuje się przez resztę czasu).
1.2.(*) (3 pkt.)
Dioda TEST „wtóruje” stanowi sygnału – jest zapalona, gdy sygnał utrzymuje się w stanie wysokim, jest gaszona, gdy sygnał PWM
przechodzi w stan niski.
2. Klawiatura multipleksowana.
Gdy pozwala zmieniać wartość parametru wypełnienia PWM w zakresie od 030 (030 = 3%) do 120 (120 = 12%). Strzałki zmieniają
wartość parametru wypełnienia PWM:
2.1.(!) (3 pkt.)
Góra/dół z krokiem co 1.
2.2.(*) (5 pkt.)
Prawo/lewo z krokiem co 10,
3. Wyświetlacz multipleksowany.
 3.1.(!) (10 pkt.)
Gdy wyświetla aktualną wartość parametru wypełnienia PWM.
4. Wyświetlacz LCD i klawiatura matrycowa.
Gdy dostępne jest menu graficzne oraz można się po nim poruszać z użyciem strzałek klawiatury matrycowej. Aby w ogóle otrzymać
punkty w ramach podzadania 4 możliwe musi być poruszanie się po całym menu - niedziałające opcje nadal należy wyświetlić.
Wejście do podmenu odbywa się klawiszem ENTER, powrót do opcji wyżej odbywa się klawiszem ESC. Zmiany dokonane we
wszystkich opcjach zgromadzonych w ramach bieżącego podmenu zatwierdzane są klawiszem ENTER i anulowane klawiszem ESC
(z jednoczesnym powrotem do opcji wyżej). Zmiana poszczególnych opcji odbywa się strzałkami prawo/lewo. Wskazana aktualnie
pozycja sygnalizowana jest przez poprzedzający ją symbol „>”.
- 1 MAIN SCREEN
JAN KOWALSKI
PWM VALUE: 030
- 2 SETTINGS
- 2.1 PWM
- STATE OFF
- VALUE 030
- 2.2 LED
- F1 OFF
- F2 OFF
- F3 OFF
- F4 OFF
- OK OFF
- ER OFF
- 2.3 OTHER
- BUZZ OFF
- 2.4 LOAD DEFAULTS
4.1.(*) (3 pkt.)
Po wejściu do podmenu MAIN SCREEN widoczne jest imię i nazwisko autora oraz bieżąca wartość PWM.
4.2.(!) (15 pkt.)
Po wejściu do podmenu SETTINGS>PWM możliwe jest dokonywanie zmian ustawień dotyczących generowania sygnału PWM.
Wskazanie STATE z poziomu podmenu PWM pozwala na edycję stanu ON/OFF (strzałkami prawo/lewo).
Wskazanie VALUE pozwala na edycję bieżącej wartości parametru PWM z krokiem co 10 (strzałkami prawo/lewo).
4.3.(*) (5 pkt.)
Po wejściu do podmenu SETTINGS>LED możliwe jest zmiana stanu diod sygnalizacyjnych (strzałkami prawo/lewo).
4.4.(*) (3 pkt.)
Wskazanie LOAD DEFAULTS i zatwierdzenie klawiszem ENTER przywraca domyślne parametry PWM, diod LED oraz brzęczyka.
4.5.(*) (3 pkt.)
Po wejściu do podmenu SETTINGS>OTHER pozwala na edycję pozostałych elementów DSM51. Pozycja BUZZ pozwala na edycję
stanu brzęczyka (strzałkami prawo/lewo).
5. Port transmisji szeregowej.
5.1{*) (20 pkt.)
Gdy pozwala zmienić ustawienie wypełnienia PWM przez wysłanie trzech cyfr z komputera PC (z zakresu 030-120).
5.2.(*) (10 pkt.)
Gdy co sekundę odsyła aktualną wartość (trzy cyfry) wypełnienia PWM do komputera PC.