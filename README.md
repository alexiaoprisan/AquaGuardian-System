# AquaGuardian System

AquaGuardian System este un sistem inteligent creat pentru monitorizarea si automatizarea unui acvariu. Ideea proiectului a pornit din dorinta de a realiza un sistem capabil sa mentina conditii stabile si sigure pentru pesti prin monitorizarea permanenta a parametrilor importanti si automatizarea unor procese esentiale.

## Scopul proiectului

Sistemul monitorizeaza constant parametri importanti ai acvariului, precum temperatura apei, nivelul apei si lumina ambientala, pentru a mentine un mediu sigur si stabil pentru pesti. Informatiile sunt afisate in timp real pe un LCD conectat prin I2C, astfel incat utilizatorul poate vedea rapid ce se intampla in acvariu.

Una dintre functiile principale ale proiectului este hranirea automata la ore fixe. Cu ajutorul modulului RTC DS3231, sistemul pastreaza timpul real si declanseaza automat hranirea la ore prestabilite. In plus, sistemul poate controla si iluminarea acvariului in functie de lumina detectata de senzorul LDR.

Proiectul combina mai multe concepte importante din domeniul sistemelor embedded: senzori, ADC, PWM, I2C si comunicatie seriala.

---

# Descriere Generala

Sistemul integreaza mai multe module hardware si software care comunica intre ele pentru a asigura functionarea automata a acvariului si monitorizarea parametrilor importanti.

* Microcontrollerul ATmega328P coordoneaza toate procesele.
* RTC DS3231 pastreaza timpul real.
* NTC waterproof monitorizeaza temperatura apei.
* HC-SR04 monitorizeaza nivelul apei.
* LDR controleaza automat iluminarea.
* Servomotorul SG90 realizeaza hranirea automata.
* LCD-ul 16x2 I2C afiseaza informatiile sistemului.

Comunicatia dintre module este realizata folosind:

* I2C pentru RTC si LCD
* PWM pentru controlul servomotorului
* ADC pentru senzori analogici

---

# Hardware Design

## Componente utilizate

| Componenta         | Model          | Interfata          | Functie           |
| ------------------ | -------------- | ------------------ | ----------------- |
| Microcontroller    | ATmega328P     | -                  | Control sistem    |
| RTC                | DS3231         | I2C                | Timp real         |
| Senzor temperatura | NTC waterproof | ADC                | Temperatura apa   |
| Senzor nivel apa   | HC-SR04        | GPIO               | Nivel apa         |
| LCD                | LCD 16x2 I2C   | I2C                | Afisare           |
| Servomotor         | SG90           | PWM                | Hranire           |
| Releu              | Releu 5V       | GPIO               | Control banda LED |
| Senzor lumina      | LDR            | ADC                | Detectare lumina  |
| Banda LED          | 12V            | Alimentare externa | Iluminare         |
| Butoane            | Push Button    | GPIO               | Control manual    |

---

# Conexiuni Hardware

| Componenta      | Pin                    |
| --------------- | ---------------------- |
| RTC DS3231      | PC4 (SDA), PC5 (SCL)   |
| LCD 16x2 I2C    | PC4 (SDA), PC5 (SCL)   |
| HC-SR04         | PD4 (TRIG), PD5 (ECHO) |
| NTC waterproof  | PC1 (A1)               |
| LDR             | PC0 (A0)               |
| Releu           | PD7 (D7)               |
| Servomotor SG90 | PB1 (D9)               |
| Buton hranire   | PD3 (D3)               |
| Buton iluminare | PD6 (D6)               |

---

# Software Design

Partea software a proiectului a fost dezvoltata in limbajul C/C++ folosind framework-ul Arduino pentru microcontrollerul ATmega328P.

## Functionalitati implementate

### Serial Monitor

Comunicatia seriala este utilizata pentru monitorizarea parametrilor sistemului in timp real.

Sunt afisate:

* temperatura apei
* nivelul apei
* valorile LDR
* mesaje de stare
* alerte

### PWM

Servomotorul SG90 este controlat prin PWM pentru hranirea automata.

### ADC

ADC-ul este utilizat pentru:

* citirea senzorului NTC
* citirea senzorului LDR

### I2C

Protocolul I2C este utilizat pentru:

* RTC DS3231
* LCD 16x2 I2C

---

# Implementare

Programul ruleaza intr-o bucla infinita in care sunt cititi senzorii, sunt verificate conditiile de functionare si sunt controlate perifericele.

## GPIO si control hardware

Pinii microcontrollerului au fost configurati folosind functiile:

* pinMode
* digitalWrite
* digitalRead

## Monitorizare si automatizare

Sistemul monitorizeaza:

* temperatura apei
* nivelul apei
* lumina ambientala

Iluminarea este controlata automat folosind LDR-ul si releul.

Hranirea automata este realizata folosind RTC-ul DS3231 si servomotorul SG90.

## Biblioteci utilizate

```cpp
Wire.h
RTClib.h
LiquidCrystal_I2C.h
Servo.h
math.h
```

---

# Rezultate Obținute

Sistemul AquaGuardian a fost implementat si testat cu succes.

Proiectul permite:

* monitorizarea temperaturii apei
* monitorizarea nivelului apei
* control automat al iluminarii
* hranire automata folosind RTC
* afisarea parametrilor pe LCD
* monitorizare prin Serial Monitor

Toate modulele hardware au fost testate individual si integrate cu succes in sistemul final.

---

# Concluzii

Proiectul AquaGuardian a demonstrat integrarea mai multor periferice si protocoale embedded intr-un sistem functional de automatizare.

Au fost utilizate concepte importante precum ADC, PWM, I2C, GPIO si comunicatie seriala pentru realizarea unui sistem capabil sa monitorizeze si sa controleze automat parametrii importanti ai unui acvariu.

---

# Jurnal

## Saptamana 1

Am ales ideea proiectului si am stabilit functionalitatile principale ale sistemului.

## Saptamana 2

Am realizat planul proiectului si am ales modulele hardware necesare.

## Saptamana 3

Am realizat schema bloc si am inceput redactarea documentatiei.

## Saptamana 4

Am realizat conexiunile hardware dintre componente si am testat fiecare modul individual.

## Saptamana 5

Am implementat partea software a proiectului si am integrat toate functionalitatile sistemului.

---

# Bibliografie / Resurse

* Datasheet ATmega328P
* Datasheet DS3231
* Datasheet HC-SR04
* Datasheet SG90
* [https://www.arduino.cc/reference/en/](https://www.arduino.cc/reference/en/)
* [https://randomnerdtutorials.com/](https://randomnerdtutorials.com/)
* [https://lastminuteengineers.com/](https://lastminuteengineers.com/)

---

# Autor

Alexia Oprisan
