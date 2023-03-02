#include<8051.h>

// ----------------------------------DEKLARACJIA----ZMIENNYCH----I----FUNKCJI----------------------------------------------------


// Kody liczb 0-9 na wyswietlaczu 7seg i LCD + transmisja szeregowa
__code unsigned char numbers[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
                                   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};
__code unsigned char LCDnumbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

// Tablice z napisami dla kazdego menu
__code unsigned char mainMenu[] = {'M', 'A', 'I', 'N', ' ', 'M', 'E', 'N', 'U', '\0'};
__code unsigned char settings[] = {'S', 'E', 'T', 'T', 'I', 'N', 'G', 'S', '\0'};
__code unsigned char myName[] = {'J', 'A', 'K', 'U', 'B', ' ', 'S', 'A', 'L', 'E', 'K', '\0'};
__code unsigned char PWMValueText[] = {'P', 'W', 'M', ' ', 'V', 'A', 'L', 'U', 'E', ':', ' ', ' ', '\0'};
__code unsigned char PWMText[] = {'P', 'W', 'M', '\0'};
__code unsigned char LEDText[] = {'L', 'E', 'D', '\0'};
__code unsigned char other[] = {'O', 'T', 'H', 'E', 'R', '\0'};
__code unsigned char loadDefaultsText[] = {'L', 'O', 'A', 'D', ' ', 'D', 'E', 'F', 'A', 'U', 'L', 'T', 'S', '\0'};
__code unsigned char stateText[] = {'S', 'T', 'A', 'T', 'E', ' ', ' ', '\0'};
__code unsigned char valueText[] = {'V', 'A', 'L', 'U', 'E', ' ', ' ', '\0'};
__code unsigned char F1Text[] = {'F', '1', ' ', ' ', ' ', ' ', '\0'};
__code unsigned char F2Text[] = {'F', '2', ' ', ' ', ' ', ' ', '\0'};
__code unsigned char F3Text[] = {'F', '3', ' ', ' ', ' ', ' ', '\0'};
__code unsigned char F4Text[] = {'F', '4', ' ', ' ', ' ', ' ', '\0'};
__code unsigned char OKText[] = {'O', 'K', ' ', ' ', ' ', ' ', '\0'};
__code unsigned char ERText[] = {'E', 'R', ' ', ' ', ' ', ' ', '\0'};
__code unsigned char BUZZText[] = {'B', 'U', 'Z', 'Z', ' ', ' ', '\0'};
__code unsigned char onText[] = {'O', 'N', '\0'};
__code unsigned char offText[] = {'O', 'F', 'F', '\0'};
// Tablice z wartosciami dla t0
__code unsigned char TH0Low[] = {0xBA, 0xBA, 0xBB, 0xBC, 0xBD, 0xBD, 0xBE, 0xBF, 0xBF, 0xC0};
__code unsigned char TL0Low[] = {0x28, 0xE1, 0x99, 0x51, 0x0A, 0xC2, 0x7A, 0x33, 0xEB, 0xA3};
__code unsigned char TH0High[] = {0xFD, 0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF8, 0xF7};
__code unsigned char TL0High[] = {0xD7, 0x1E, 0x66, 0xAE, 0xF5, 0x3D, 0x85, 0xCC, 0x14, 0x5C};
__code unsigned char smallPrecent[] = {0, 19, 37, 56, 74, 93, 111, 129, 148, 166};

__bit __at (0x96) SEG_DISPLAY;               // Wlaczenie 7SEG (1/0)
__bit __at (0xB5) BUTTON_PRESSED;            // Czy 7SEG guzik wcisniety (0/1)
__bit __at (0x95) BUZZER;                    // Brzeczyk (1/0)
__bit __at (0x97) TEST;                      // Dioda TEST (1/0)

__bit LCDbusyFlag;                 // LCD zajety?
__bit finishedLCDInit;             // Skonczono inicjalizacje LCD?
__bit changeMenu;                  // Menu sie zmienilo?
__bit isSecondLine;                // Druga linia na LCD?
__bit cursorLine;                  // W ktorej lini jest kursor (0 - w pierwszej / 1 - w drugiej)
__bit wasCursorDrawn;              // Kursor narysowany?
__bit LCDcleared;                  // LCD wyczyszczone?
__bit showingValue;                // Pokazywanie wartosci?
__bit onOff;                       // OFF/ON (0/1)
__bit isHigh;                      // Aktualny stan PWM to wysoki?
__bit receiveFlag;                 // Odebrano bit?
__bit transmitFlag;                // Mozna przesylac bajt?
__bit oneSec;                      // Minela sekunda?
__bit doubleClicked;

// Wartosci w menu, tymczasowe i aktualne
__bit currentState;
__bit tempState;
__bit tempBUZZ;
unsigned char tempPWMValue;
unsigned char PWMValue;
unsigned char tempDiodeStatus;
unsigned char currentDiodeStatus;

// CSDS, wybor jednego (lub wiecej) z 6 wyswietlaczy lub diody
__xdata unsigned char* whichLED = (__xdata unsigned char*) 0xFF30;
// CSDB, wybor wlaczonych segmentow, lub wlaczonych diod z 6
__xdata unsigned char* whichSEG = (__xdata unsigned char*) 0xFF38;
// LCD (W - write, R - read, C - command, D - data)
__xdata unsigned char* LCDWC = (__xdata unsigned char*) 0xFF80;
__xdata unsigned char* LCDWD = (__xdata unsigned char*) 0xFF81;
__xdata unsigned char* LCDRC = (__xdata unsigned char*) 0xFF82;
// CSKB1 (8, 9, l, p, g, d, esc, enter)
__xdata unsigned char* CSKB = (__xdata unsigned char*) 0xFF22;
// port B
__xdata unsigned char* CS55B = (__xdata unsigned char*) 0xFF29;
__xdata unsigned char* CS55D = (__xdata unsigned char*) 0xFF2B;

// Zmienne do wyswietlania menu
unsigned char LCDIterator;
unsigned char currentMenu;
// Klawiatura LCD
unsigned char LCDKeyboard;
unsigned char lastClickedLCDButton;
// Zmienne pomocnicze do petli przy 7seg i jego klawiaturze
unsigned char counter;
unsigned char LEDchosen;
unsigned char lastSegmentButton;
unsigned char currentSegment;
unsigned char lastHighOnTimer;
unsigned char delayForDouble;
// Pomocnicza zmienna w timerze
unsigned char timer0Data;
unsigned char oneSecCounter;
// Transmisja szeregowa
unsigned char receivedData[3];
unsigned char transmitCounter;
unsigned char receiveCounter;

// Funkcje do wyswietlacza segmentowego i jego klawiatury
void showFirstNumber(void);
void showSecondNumber(void);
void showThirdNumber(void);
void showSegDiodes(void);
void showPWMOnSeg(void);
void readSegButtons(void);
void segButtonWasClicked(void);

// Funkcje do LCD
void initLCD(void);
void showCurrentMenu(void);
void finishedFirstLine(void);
void finishedSecondLine(void);
void startShowingValue(void);
void showOnOff(void);
void loadDefaults(void);
void readLCDButtons(void);

// T0
void setTimer0Value(void);

// Transmisja szeregowa
void sendData(void);
void receiveData(void);

// ---------------------------------KONIEC-----DEKLARACJI----ZMIENNYCH----I----FUNKCJI-------------------------------------------

void main(void){
// Init
     // Ustawiamy wartosci poczatkowe
     *CS55D = 0x80;
     lastSegmentButton = 0;
     lastClickedLCDButton = 255;
     LCDIterator = 0;
     changeMenu = 0;
     currentMenu = 0;
     isSecondLine = 0;
     cursorLine = 0;
     wasCursorDrawn = 0;
     LCDcleared = 0;
     LCDKeyboard = 0;
     showingValue = 0;
     TEST = 1;
     isHigh = 0;
     oneSecCounter = 0;
     currentSegment = 1;
     receiveFlag = 0;
     transmitFlag = 1;
     transmitCounter = 0;
     receiveCounter = 0;
     lastHighOnTimer = 0;
	finishedLCDInit = 0;
	doubleClicked = 0;

     // Wczytanie wartosci poczatkowych w menu
     loadDefaults();

     // Mode 1 transmisji szeregowej - asynchroniczny na zegarze t1
     // 9600 baud rate (wartość 253 w t1), 1 bit stopu, brak kontroli parzystosci
     SCON = 0b01010000;
     PCON &= 0b01111111;      // Ustawienie SMOD na 0
     // Licznik t0 w trybie 1 (0 - 65535) i t1 w trybie 2 (0-255, automatyczne przepisanie TH1 do TL1)
     TMOD = 0b00100001;
     EA = 1;        // Obsluga wszystkich przerwań
     ES = 1;        // Przerwania szeregowe
     ET0 = 1;       // Przerwania timera0
     TR0 = 1;       // Wlaczenie t0
     TR1 = 1;       // Wlaczenie t1
     TH1 = 253;     // Ustawienie wartosci 253 w t1

     // Glowna petla programowa
     while(1){
          if(!finishedLCDInit){    // Inicjalizuj LCD
               initLCD();
          }else{
               if(changeMenu){     // Jesli LCD zainicjalizowane i zmiana menu to je wyswietl
                    showCurrentMenu();
               }
          }
          if(oneSecCounter == 100){     // Jesli minela sekunda to zacznij wysylac dane
               oneSecCounter = 0;
               transmitCounter = 0;
               oneSec = 1;
          }
          if(oneSec){
               sendData();
          }
          if(receiveFlag){         // Jesli odebrano dane to je zapisz
               receiveData();
          }
          readLCDButtons();             // Odczytaj przyciski na klawiaturze matrycowej
          if(TH0 != lastHighOnTimer && TL0 > 150){
          	if(doubleClicked){
          		delayForDouble++;
          		if(delayForDouble == 100){
          			delayForDouble = 0;
          			doubleClicked = 0;
				}
			}else{
   		  		readSegButtons();        // Odczytaj przyciski na klawiaturze segmentowej
			}
               showPWMOnSeg();          // Wyswietl PWM na 7SEG
          }
     };
}

void showFirstNumber(void){        // Pokaz liczbe jednosci na 7SEG
     SEG_DISPLAY = 1;
     *whichLED = 1;
     *whichSEG = numbers[PWMValue % 10];
     SEG_DISPLAY = 0;
     lastHighOnTimer = TH0;
}

void showSecondNumber(void){       // Pokaz liczbe dziesiatek na 7SEG
     SEG_DISPLAY = 1;
     *whichLED = 2;
     *whichSEG = numbers[((PWMValue % 100) / 10)];
     SEG_DISPLAY = 0;
     lastHighOnTimer = TH0;
}

void showThirdNumber(void){        // Pokaz liczbe setek na 7SEG
     SEG_DISPLAY = 1;
     *whichLED = 4;
     *whichSEG = numbers[PWMValue / 100];
     SEG_DISPLAY = 0;
     lastHighOnTimer = TH0;
}

void showSegDiodes(void){          // Zapal diody na 7SEG
     SEG_DISPLAY = 1;
     *whichLED = 64;
     *whichSEG = currentDiodeStatus;
     SEG_DISPLAY = 0;
     lastHighOnTimer = TH0;
}

void showPWMOnSeg(void){
     if(currentSegment == 1){
          showFirstNumber();
          currentSegment = 2;
     }else if(currentSegment == 2){
          showSecondNumber();
          currentSegment = 4;
     }else if(currentSegment == 4){
          showThirdNumber();
          currentSegment = 64;
     }else if(currentSegment == 64){
          showSegDiodes();
          currentSegment = 1;
     }
}

void readSegButtons(void){         // Odczytaj klawisze na segmentowej klawiaturze
     SEG_DISPLAY = 1;
     *whichLED = lastSegmentButton;  // Sprawdz czy poprzedni zostal puszcczony
     if(!BUTTON_PRESSED){
          lastSegmentButton = 0;
          LEDchosen = 1;
          for(counter = 0; counter < 6; counter++){    // Sprawdzaj kolejne przyciski
               *whichLED = LEDchosen;
               if(BUTTON_PRESSED){                     // Jesli wcisniety to obsluz i zapamietaj
                    lastSegmentButton = LEDchosen;
                    segButtonWasClicked();
               }
               LEDchosen += LEDchosen;
          }
     }
   	doubleClicked = 1;     
}

void segButtonWasClicked(void){
     if(LEDchosen == 4){     // PRAWO (PWM += 10)
          if(PWMValue <= 110){
               PWMValue += 10;
               if(currentMenu == 1){
                    changeMenu = 1;
               }
          }
     }
     else if(LEDchosen == 8){     // GORA (PWM += 1)
          if(PWMValue < 120){
               PWMValue++;
               if(currentMenu == 1){
                    changeMenu = 1;
               }
          }
     }else if(LEDchosen == 16){     // DOL (PWM -= 1)
          if(PWMValue > 30){
               PWMValue--;
               if(currentMenu == 1){
                    changeMenu = 1;
               }
          }
     }else if(LEDchosen == 32){     // LEWO (PWM -= 10)
          if(PWMValue >= 40){
               PWMValue -= 10;
               if(currentMenu == 1){
                    changeMenu = 1;
               }
          }
     }
}

void initLCD(void){
     LCDbusyFlag = *LCDRC & 128;   // Sprawdz czy busy
     if(!LCDbusyFlag){
          if(LCDIterator == 0){
               // 8 bitowa magistrala danych, dwie linie i czcionka 5x8
               *LCDWC = 0b00111000;
               LCDIterator++;
          }else if(LCDIterator == 1){
               // Wlaczenie LCD, kursora i jego migania
               *LCDWC = 0b00001100;
               LCDIterator++;
          }else if(LCDIterator == 2){
               // Kierunek ruchu kursora w prawo (I/D = I) i poruszanie sie kursora zamiast wyswietlacza
               *LCDWC = 0b00000110;
               finishedLCDInit = 1;
               changeMenu = 1;
               LCDIterator = 0;
          }
     }
}

void showCurrentMenu(void){
     LCDbusyFlag = *LCDRC & 128;
     if(!LCDbusyFlag){
          if(!LCDcleared){                        // Wyczysc wyswietlacz
               *LCDWC = 0b00000001;
               LCDcleared = 1;
          }else if(!wasCursorDrawn){                   // Napisz kursor lub spacje
               if(isSecondLine == cursorLine){
                    *LCDWD = '>';
               }else{
                    *LCDWD = ' ';
               }
               wasCursorDrawn = 1;
          }else{
               switch(currentMenu)
               {
               case 0:                                           // Menu 0 = MAIN MENU / SETTINGS
                    if(!isSecondLine){
                         if(mainMenu[LCDIterator] != '\0'){
                              *LCDWD = mainMenu[LCDIterator];
                              LCDIterator++;
                         }else{
                              finishedFirstLine();
                         }
                    }else{
                         if(settings[LCDIterator] != '\0'){
                              *LCDWD = settings[LCDIterator];
                              LCDIterator++;
                         }else{
                              finishedSecondLine();
                         }
                    }
                    break;
               case 1:                                           // Menu 1 = IMIE NAZWISKO / PWM VALUE
                    if(!isSecondLine){                                // Pierwsza linia
                         if(myName[LCDIterator] != '\0'){
                              *LCDWD = myName[LCDIterator];
                              LCDIterator++;
                         }else{
                              finishedFirstLine();
                         }
                    }else{                                           // Druga linia + jej wartosc
                         if(!showingValue){
                              if(PWMValueText[LCDIterator] != '\0'){
                                   *LCDWD = PWMValueText[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{                                            // Pokaz wartosc
                              if(LCDIterator < 4){
                                   switch(LCDIterator){
                                        case 0:
                                             *LCDWD = LCDnumbers[(PWMValue / 100)];
                                             break;
                                        case 1:
                                             *LCDWD = LCDnumbers[((PWMValue % 100) / 10)];
                                             break;
                                        case 2:
                                             *LCDWD = LCDnumbers[(PWMValue % 10)];
                                             break;
                                   }
                                   LCDIterator++;
                              }else{
                                   showingValue = 0;
                                   finishedSecondLine();
                              }
                         }
                    }
                    break;
               case 2:                                           // Menu 2 = PWM / LED
                    if(!isSecondLine){
                         if(PWMText[LCDIterator] != '\0'){
                              *LCDWD = PWMText[LCDIterator];
                              LCDIterator++;
                         }else{
                              finishedFirstLine();
                         }
                    }else{
                         if(LEDText[LCDIterator] != '\0'){
                              *LCDWD = LEDText[LCDIterator];
                              LCDIterator++;
                         }else{
                              finishedSecondLine();
                         }
                    }
                    break;
               case 3:                                           // Menu 3 = OTHER / LOAD DEFAULTS
                    if(!isSecondLine){
                         if(other[LCDIterator] != '\0'){
                              *LCDWD = other[LCDIterator];
                              LCDIterator++;
                         }else{
                              finishedFirstLine();
                         }
                    }else{
                         if(loadDefaultsText[LCDIterator] != '\0'){
                              *LCDWD = loadDefaultsText[LCDIterator];
                              LCDIterator++;
                         }else{
                              finishedSecondLine();
                         }
                    }
                    break;
               case 4:                                                 // Menu 4 = STATE / VALUE
                    if(!isSecondLine){
                         if(!showingValue){
                              if(stateText[LCDIterator] != '\0'){
                                   *LCDWD = stateText[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempState;
                              showOnOff();
                         }
                    }else{
                         if(!showingValue){
                              if(valueText[LCDIterator] != '\0'){
                                   *LCDWD = valueText[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              if(LCDIterator < 4){
                                   switch(LCDIterator){
                                        case 0:
                                             *LCDWD = LCDnumbers[(tempPWMValue / 100)];
                                             break;
                                        case 1:
                                             *LCDWD = LCDnumbers[((tempPWMValue % 100) / 10)];
                                             break;
                                        case 2:
                                             *LCDWD = LCDnumbers[(tempPWMValue % 10)];
                                             break;
                                   }
                                   LCDIterator++;
                              }else{
                                   showingValue = 0;
                                   finishedSecondLine();
                              }
                         }
                    }
                    break;
               case 5:                                                // Menu 5 = F1 / F2
                    if(!isSecondLine){
                         if(!showingValue){
                              if(F1Text[LCDIterator] != '\0'){
                                   *LCDWD = F1Text[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempDiodeStatus & 1;
                              showOnOff();
                         }
                    }else{
                         if(!showingValue){
                              if(F2Text[LCDIterator] != '\0'){
                                   *LCDWD = F2Text[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempDiodeStatus & 2;
                              showOnOff();
                         }
                    }
                    break;
               case 6:                                                // Menu 6 = F3 / F4
                    if(!isSecondLine){
                         if(!showingValue){
                              if(F3Text[LCDIterator] != '\0'){
                                   *LCDWD = F3Text[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempDiodeStatus & 4;
                              showOnOff();
                         }
                    }else{
                         if(!showingValue){
                              if(F4Text[LCDIterator] != '\0'){
                                   *LCDWD = F4Text[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempDiodeStatus & 8;
                              showOnOff();
                         }
                    }
                    break;
               case 7:                                                // Menu 7 = OK / ER
                    if(!isSecondLine){
                         if(!showingValue){
                              if(OKText[LCDIterator] != '\0'){
                                   *LCDWD = OKText[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempDiodeStatus & 16;
                              showOnOff();
                         }
                    }else{
                         if(!showingValue){
                              if(ERText[LCDIterator] != '\0'){
                                   *LCDWD = ERText[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempDiodeStatus & 32;
                              showOnOff();
                         }
                    }
                    break;
               case 8:                                                // Menu 8 = BUZZ
                    if(!isSecondLine){
                         if(!showingValue){
                              if(BUZZText[LCDIterator] != '\0'){
                                   *LCDWD = BUZZText[LCDIterator];
                                   LCDIterator++;
                              }else{
                                   startShowingValue();
                              }
                         }else{
                              onOff = tempBUZZ;
                              showOnOff();
                         }
                    }else{
                         finishedSecondLine();
                    }
                    break;
               }
          }
          
     }
}

void finishedFirstLine(void){
     *LCDWC = 0b11000000;          // Nowa linia \n
     LCDIterator = 0;
     isSecondLine = 1;
     wasCursorDrawn = 0;
     showingValue = 0;
}

void finishedSecondLine(void){
     LCDIterator = 0;
     changeMenu = 0;
     wasCursorDrawn = 0;
     isSecondLine = 0;
     LCDcleared = 0;
     showingValue = 0;
}

void startShowingValue(void){
     showingValue = 1;
     LCDIterator = 0;
}

void showOnOff(void){
     if(onOff){
          if(onText[LCDIterator] != '\0'){
               *LCDWD = onText[LCDIterator];
               LCDIterator++;
          }else{
               if(isSecondLine){
                    finishedSecondLine();
               }else{
                    finishedFirstLine();
               }
          }
     }else{
          if(offText[LCDIterator] != '\0'){
               *LCDWD = offText[LCDIterator];
               LCDIterator++;
          }else{
               if(isSecondLine){
                    finishedSecondLine();
               }else{
                    finishedFirstLine();
               }
          }
     }
}

void loadDefaults(void){
     PWMValue = 30;
     tempPWMValue = PWMValue;
     currentState = 1;
     tempState = currentState;
     BUZZER = 1;
     tempBUZZ = !BUZZER;
     currentDiodeStatus = 0;
     tempDiodeStatus = currentDiodeStatus;
}

void readLCDButtons(void){
     LCDKeyboard = *CSKB;
     if(LCDKeyboard != lastClickedLCDButton){
          if(!(LCDKeyboard & 4)){       // LEWO (On = Off ; PWM -= 10)
               switch(currentMenu){
                    case 4:
                         if(!cursorLine){
                              if(tempState){
                                   tempState = 0;
                                   changeMenu = 1;
                              }
                         }else{
                              if(tempPWMValue >= 40){
                                   tempPWMValue -= 10;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 5:
                         if(!cursorLine){
                              if((tempDiodeStatus & 1)){
                                   tempDiodeStatus -= 1;
                                   changeMenu = 1;
                              }
                         }else{
                              if((tempDiodeStatus & 2)){
                                   tempDiodeStatus -= 2;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 6:
                         if(!cursorLine){
                              if((tempDiodeStatus & 4)){
                                   tempDiodeStatus -= 4;
                                   changeMenu = 1;
                              }
                         }else{
                              if((tempDiodeStatus & 8)){
                                   tempDiodeStatus -= 8;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 7:
                         if(!cursorLine){
                              if((tempDiodeStatus & 16)){
                                   tempDiodeStatus -= 16;
                                   changeMenu = 1;
                              }
                         }else{
                              if((tempDiodeStatus & 32)){
                                   tempDiodeStatus -= 32;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 8:
                         if(tempBUZZ){
                              tempBUZZ = 0;
                              changeMenu = 1;
                         }
                         break;
               }
          }else if(!(LCDKeyboard & 8)){      // PRAWO  (Off = On ; PWM += 10)
               switch(currentMenu){
                    case 4:
                         if(!cursorLine){
                              if(!tempState){
                                   tempState = 1;
                                   changeMenu = 1;
                              }
                         }else{
                              if(tempPWMValue <= 110){
                                   tempPWMValue += 10;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 5:
                         if(!cursorLine){
                              if(!(tempDiodeStatus & 1)){
                                   tempDiodeStatus += 1;
                                   changeMenu = 1;
                              }
                         }else{
                              if(!(tempDiodeStatus & 2)){
                                   tempDiodeStatus += 2;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 6:
                         if(!cursorLine){
                              if(!(tempDiodeStatus & 4)){
                                   tempDiodeStatus += 4;
                                   changeMenu = 1;
                              }
                         }else{
                              if(!(tempDiodeStatus & 8)){
                                   tempDiodeStatus += 8;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 7:
                         if(!cursorLine){
                              if(!(tempDiodeStatus & 16)){
                                   tempDiodeStatus += 16;
                                   changeMenu = 1;
                              }
                         }else{
                              if(!(tempDiodeStatus & 32)){
                                   tempDiodeStatus += 32;
                                   changeMenu = 1;
                              }
                         }
                         break;
                    case 8:
                         if(!tempBUZZ){
                              tempBUZZ = 1;
                              changeMenu = 1;
                         }
                         break;
               }
          }else if(!(LCDKeyboard & 16)){          // GORA   (Kursor w gore (mozliwa zmiana wyswietlanego podmenu))
               if(!cursorLine){
                    if(currentMenu == 3){
                         currentMenu = 2;
                         changeMenu = 1;
                         cursorLine = 1;
                    }else if(currentMenu == 7){
                         currentMenu = 6;
                         changeMenu = 1;
                         cursorLine = 1;
                    }else if(currentMenu == 6){
                         currentMenu = 5;
                         changeMenu = 1;
                         cursorLine = 1;
                    }
               }else{
                    cursorLine = 0;
                    changeMenu = 1;
               }
          }else if(!(LCDKeyboard & 32)){          // DOL    (Kursor w dol (mozliwa zmiana wyswietlanego podmenu))
               if(cursorLine){
                    if(currentMenu == 2){
                         currentMenu = 3;
                         cursorLine = 0;
                         changeMenu = 1;
                    }else if(currentMenu == 5){
                         currentMenu = 6;
                         cursorLine = 0; 
                         changeMenu = 1;
                    }else if(currentMenu == 6){
                         currentMenu = 7;
                         cursorLine = 0;
                         changeMenu = 1;
                    }
               }else if(currentMenu != 8){
                    cursorLine = 1;
                    changeMenu = 1;
               }
          }else if(!(LCDKeyboard & 64)){          // ESC    (Wyjscie z podmenu + odrzucenie zmian)
               switch(currentMenu){
                    case 1:
                         currentMenu = 0;
                         cursorLine = 0;
                         changeMenu = 1;
                         break;
                    case 2:
                         currentMenu = 0;
                         cursorLine = 1;
                         changeMenu = 1;
                         break;
                    case 3:
                         currentMenu = 0;
                         cursorLine = 1;
                         changeMenu = 1;
                         break;
                    case 4:
                         currentMenu = 2;
                         cursorLine = 0;
                         changeMenu = 1;
                         tempState = currentState;
                         tempPWMValue = PWMValue;
                         break;
                    case 5:
                    case 6:
                    case 7:
                         currentMenu = 2;
                         cursorLine = 1;
                         changeMenu = 1;
                         tempDiodeStatus = currentDiodeStatus;
                         break;
                    case 8:
                         currentMenu = 3;
                         cursorLine = 0;
                         changeMenu = 1;
                         tempBUZZ = !BUZZER;
                         break;
               }
          }else if(!(LCDKeyboard & 128)){         // ENTER  (Wejscie do podmenu / Zatwierdzenie zmian i powrot)
               switch(currentMenu){
                    case 0:
                         if(!cursorLine){
                              tempPWMValue = PWMValue;
                              currentMenu = 1;
                              cursorLine = 0;
                              changeMenu = 1;
                         }else{
                              currentMenu = 2;
                              cursorLine = 0;
                              changeMenu = 1;
                         }
                         break;
                    case 1:
                         currentMenu = 0;
                         cursorLine = 0;
                         changeMenu = 1;
                         break;
                    case 2:
                         if(!cursorLine){
                              tempPWMValue = PWMValue;
                              currentMenu = 4;
                              cursorLine = 0;
                              changeMenu = 1;
                         }else{
                              currentMenu = 5;
                              cursorLine = 0;
                              changeMenu = 1;
                         }
                         break;
                    case 3:
                         if(!cursorLine){
                              currentMenu = 8;
                              cursorLine = 0;
                              changeMenu = 1;
                         }else{
                              loadDefaults();
                              currentMenu = 0;
                              cursorLine = 0;
                              changeMenu = 1;
                         }
                         break;
                    case 4:
                         currentState = tempState;
                         PWMValue = tempPWMValue;
                         currentMenu = 2;
                         cursorLine = 0;
                         changeMenu = 1;
                         break;
                    case 5:
                    case 6:
                    case 7:
                         currentDiodeStatus = tempDiodeStatus;
                         currentMenu = 2;
                         cursorLine = 1;
                         changeMenu = 1;
                         break;
                    case 8:
                         BUZZER = !tempBUZZ;
                         currentMenu = 3;
                         cursorLine = 0;
                         changeMenu = 1;
                         break;
               }
          }
     }
     lastClickedLCDButton = LCDKeyboard;
}

void sendData(void){
     if(transmitFlag){                            // jesli odbiorca gotowy to wyslij mu kolejna dana
          if(transmitCounter == 0){
               SBUF = LCDnumbers[PWMValue/100];
          }else if(transmitCounter == 1){
               SBUF = LCDnumbers[(PWMValue%100)/10];
          }else if(transmitCounter == 2){
               SBUF = LCDnumbers[PWMValue%10];
          }else if(transmitCounter == 3){       // zakoncz spacja i skoncz wysylanie
               SBUF = ' ';
               oneSec = 0;
          }
          transmitCounter++;
          transmitFlag = 0;
     }
}

void receiveData(void){
     receivedData[receiveCounter] = SBUF;         // zapisz odebrana dana
     receiveCounter++;
     if(receiveCounter == 3){
     	if((receivedData[0] == 48 && receivedData[1] > 50)
		 					|| (receivedData[0] == 49 && receivedData[1] == 50 && receivedData[2] == 48)
							|| (receivedData[0] == 49 && receivedData[1] < 50)){
    		PWMValue = (receivedData[0] - 48)*100 + (receivedData[1] - 48)*10 + (receivedData[2] - 48);
		 }
	   	 receiveCounter = 0;
	 }
     receiveFlag = 0;
}

void setTimer0Value(void){
     if(isHigh){         // dla wysokiego odejmujemy maly procent aby trwalo dluzej
          *CS55B = 255;
          TEST = 0;
          if(TL0High[PWMValue/10 - 3] < smallPrecent[PWMValue%10]){        // sprawdz czy odejmowanie nie wyjdzie poza 0-255
               TL0 = (255 - (smallPrecent[PWMValue%10] - TL0High[PWMValue/10 - 3]));  // jesli wyjdzie poza to oblicz roznice bez wychodzenia
               TH0 = (TH0High[PWMValue/10 - 3] - 1);                    // i zmniejsz th0 o 1
          }else{
               TL0 = TL0High[PWMValue/10 - 3] - smallPrecent[PWMValue%10];
               TH0 = TH0High[PWMValue/10 - 3];
          }
     }else{         // dla niskiego dodajemy maly procent
          *CS55B = 0;
          TEST = 1;
          timer0Data = 255 - TL0Low[PWMValue/10 - 3];
          if(timer0Data < smallPrecent[PWMValue%10]){       // upewniamy sie czy nie wyjdzie poza 0-255
               TL0 = smallPrecent[PWMValue%10] - timer0Data;     // jesli wyjdzie to ustawiamy wartosc przekroczona
               TH0 = TH0Low[PWMValue/10 - 3] + 1;                // i zwiekszamy th0 o 1
          }else{
               TL0 = TL0Low[PWMValue/10 - 3] + smallPrecent[PWMValue%10];
               TH0 = TH0Low[PWMValue/10 - 3];
          }
     }
}


// obsluga przerwan timera0
void t0_interrupt(void) __interrupt(1){
     if(currentState){   // jesli wystawiamy pwm na port B
          setTimer0Value();   // ustawiamy odpowiednia wartosc zaleznie od stanu
          isHigh = !isHigh;
     }else{              // jesli nie wystawiamy wartosci na portB to dbamy o 50Hz co dwa przepelnienia w liczniku
          TH0 = 0xDC;
          TL0 = 0;
          TEST = 1;
          *CS55B = 0;
     }
     oneSecCounter++;    // licznik dziala w 50Hz czyli co 100 przepelnien bedzie sekunda
}

// obsluga przerwania szeregowego
void serial_interrupt(void) __interrupt(4){
     if(TI){   // jesli odbiorca dostal bajt to gotowy na kolejny
          TI = 0;
          transmitFlag = 1;
     }else{    // jesli odebralismy bajt to go zapiszmy
          RI = 0;
          receiveFlag = 1;
     }
}
