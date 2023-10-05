# Descrizione del File main.c

**Nome del File**: main.c
**Progetto**: PSE_2021_2022_SERVER_NTP_LAN
**Cartella**: /FW/TEST_RTC_GPS

## Descrizione
Il programma `main.c` acquisisce l'orario da un modulo GPS e dal RTC (Real-Time Clock) e ne confronta i valori. Se i valori sono diversi, reimposta l'orario del RTC con i valori acquisiti dal GPS. Il programma è diviso in varie sezioni:

1. **Setup dei LED GPIO**: Viene configurato il GPIO per fornire un feedback visivo tramite LED.
2. **Setup di interrupt e interfaccia UART GPS**: Vengono configurati gli interrupt e l'interfaccia UART per comunicare con il modulo GPS.
3. **Lampeggio dei LED**: Viene implementato un lampeggio dei LED per scopi di debug.
4. **Setup I2C e orario iniziale RTC**: Si inizializza la comunicazione I2C e si imposta l'orario iniziale del RTC.
5. **Impostazione dell'allarme**: L'allarme non è impostato nella versione attuale.
6. **Loop continuo composto da**:
   - Lampeggio dei LED.
   - Acquisizione dei dati dal modulo GPS.
   - Acquisizione dei dati dal RTC.
   - Confronto dei dati acquisiti dal modulo GPS e dal RTC.
7. **Controllo dei messaggi in ricezione dal modulo GPS**: Viene gestita la ricezione dei messaggi dal modulo GPS.

## Note
L'allarme non è attualmente impostato nel programma, ma può essere utilizzato per il confronto dei dati acquisiti.

## Autori
- PAGANIN ANDREA
- PASQUINI GIUSEPPE
- TRENTI ELIA
- TITTON GIULIA
- AMORTH MATTEO

## Cambiamenti
- F26/05, Versione A.00.01, 26 Maggio 2022, M. A.: Aggiunte descrizioni delle funzioni e intestazione.
- F19/06, Versione A.00.02, 19 Giugno 2023, M. A.: Aggiunto reset LAN per problema di connessione.


