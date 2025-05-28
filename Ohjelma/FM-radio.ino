#include <LiquidCrystal.h>
#include <Si4703_Breakout.h>
#include <Wire.h>

//Aapo Manni 2025
//Arduino-pohjainen koodi Si4703-kehitysalustan ohjaamiseen.

int resetPinni = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPinni, SDIO, SCLK);
int kanava = 937; //aloituskanava
int volume;

//Kytkimiä vastaavien pinnien määritys
const int seek_up_pinni = 8;
const int seek_down_pinni = 9;
const int fine_tune_pinni = 10;


//Pinnien määritys LCD:tä varten.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup()
{
  //Si4703 alustus
  radio.powerOn();
  
  //Aloituskanavan asettaminen
  radio.setChannel(937);
  
  //LCD:n alustus
  lcd.begin(16,2);  
  
}


//Palauttaa kokonaisluvun välillä 0-15 riippuen siitä mille
//tasolle äänenvoimakkuus halutaan asettaa
int lue_volume(){
  
  //Lukee potentiometrin arvon.
  int vol = analogRead(A1);

  //Skaalaa luvun välille 0-15.
  volume = (vol-22)/61.5;

  //Määrittää suurimmaksi arvoksi 15.
  if (volume >= 16){
    volume = 15;
  }
  
  //Asettaa radion äänenvoimakkuuden
  radio.setVolume(volume);
  }


//Pääfunktio
void loop()
{
  //Lukee potentiometrin arvon ja asettaa äänenvoimakkuuden
  lue_volume(); 
  
  //Odottaa, että SEEK UP-nappia painetaan
  if (digitalRead(8)){
    kanava = radio.seekUp();    
  }
  
  //Odottaa, että SEEK DOWN-nappia painetaan
  if (digitalRead(9)){
    kanava = radio.seekDown();    
  }

  //Odottaa, että FINE TUNE-nappia painetaan
  if (digitalRead(10)){
    kanava = kanava+1;
    radio.setChannel(kanava);    
  }  

  //Rajaa taajuudet välille 87,5-108 MHz
  if (kanava == 0){
    kanava = 875;
  }

  //Päivittää LCD-näytön arvon
  naytto(); 

  //200 ms viive toiminnan varmistamiseksi
  delay(200);    
}

//Tämä funktio päivittää LCD-näytön tietoja.
void naytto()
{
  //Muuntaa kanavan arvon desimaaliksi. Esim. 937 -> 93.7
  float desimaali = kanava / 10.0; 
  
  //Asettaa soitettavan taajuuden LCD:n ensimmäiselle riville.
  lcd.setCursor(0, 0); 
  lcd.print("FREQ: "); 
  lcd.print(desimaali, 1);
  lcd.print(" MHz "); 
  
  //Asettaa äänenvoimakkuuden toiselle LCD:n riville.
  lcd.setCursor(0, 1);
  lcd.print("VOLUME: ");  


  //Jos äänenvoimakkuus saavuttaa maksimin se tulostetaan MAX LCD:lle.
  if (volume == 15){
    lcd.print("MAX");
    return;
  }

  //Jos äänenvoimakkuus saavuttaa minimin se tulostetaan MIN LCD:lle.
  if (volume == 0){
    lcd.print("MIN");
    return;
  }

  // Muissa tilanteissa tulostetaan äänenvoimakkuus numeroarvona.
  lcd.print(volume);
  lcd.print("   ");
     
}
