# Descrizione del File pam7q.c

**Nome del File**: pam7q.c
**Progetto**: PSE_2021_2022_SERVER_NTP_LAN
**Cartella**: /FW/TEST_RTC_GPS/src/

## Descrizione
Il file `pam7q.c` contiene le definizioni delle funzioni utilizzate per l'interazione con l'integrato PAM7Q, che è un componente hardware utilizzato per acquisire dati GPS.

## Funzioni Pubbliche
Il file include le seguenti funzioni pubbliche:

1. `uint32_t getTime(uint8_t* gps_time_buf, uint8_t offset)`: Questa funzione riceve un buffer contenente dati temporali dal modulo GPS e un offset. Calcola e restituisce il tempo in formato uint32_t.

2. `uint32_t getData(uint8_t* gps_datum_buf, uint8_t offset)`: Questa funzione riceve un buffer contenente dati relativi alla data dal modulo GPS e un offset. Calcola e restituisce la data in formato uint32_t.

3. `void gpio_callback(uint gpio, uint32_t events)`: Questa funzione rappresenta una callback per gli interrupt GPIO. Al momento, questa funzione è vuota e potrebbe essere implementata per gestire interrupt specifici.

## Note
- È necessario importare la funzione `void on_uart_rx()` da `main.c` per abilitare la comunicazione UART con il modulo GPS.
- È necessario implementare un interrupt su PPS (Pulse Per Second) per ottenere un'accurata sincronizzazione temporale con il modulo GPS.

## Autori
- PAGANIN ANDREA
- PASQUINI GIUSEPPE
- TRENTI ELIA
- TITTON GIULIA
- AMORTH MATTEO

## Cambiamenti
- F26/05, Versione A.00.01, 26 Maggio 2022, M. A.: Aggiunta intestazione.

---

Il file `pam7q.c` contiene le definizioni delle funzioni necessarie per l'interfacciamento con l'integrato PAM7Q, utilizzato per acquisire dati GPS. Le funzioni pubbliche includono `getTime` e `getData`, che calcolano rispettivamente il tempo e la data dai dati forniti dal modulo GPS. La funzione `gpio_callback` rappresenta una callback per gli interrupt GPIO, anche se attualmente è vuota.

# Descrizione del File mcp7940.c

**Nome del File**: mcp7940.c
**Progetto**: PSE_2021_2022_SERVER_NTP_LAN

## Descrizione
Il file `mcp7940.c` contiene le definizioni delle funzioni utilizzate per l'interazione con l'integrato MCP7940, che è utilizzato per gestire l'orologio in tempo reale (RTC) e altre funzionalità temporali nel progetto.

## Funzioni Pubbliche
Il file include le seguenti funzioni pubbliche:

1. `uint8_t mcp7940_get_all_data(uint8_t* data, uint8_t base_address, uint8_t n_bytes)`: Questa funzione legge i dati dall'integrato MCP7940, a partire dall'indirizzo di base specificato e per il numero di byte specificato. Restituisce 1 in caso di successo e 0 in caso di errore.

2. `uint8_t mcp7940_write_single_register(uint8_t reg_address, uint8_t valueBCD)`: Questa funzione scrive un singolo registro dell'integrato MCP7940 con l'indirizzo e il valore specificati. Restituisce 1 in caso di successo e 0 in caso di errore.

3. `uint8_t mcp7940_write_multiple_registers(uint8_t base_reg_address, uint8_t* buf, uint8_t nregs)`: Questa funzione scrive più registri dell'integrato MCP7940 a partire dall'indirizzo di base specificato. Restituisce 1 in caso di successo e 0 in caso di errore.

4. `void mcp7940_start_count(uint8_t secondsBCD)`: Questa funzione avvia il conteggio del tempo sull'integrato MCP7940 con il valore specificato per i secondi in formato BCD.

5. `uint8_t mcp7940_add_hour_format(bool bool12Hours, uint8_t valueBCD)`: Questa funzione aggiunge un formato orario all'ora specificata in formato BCD. Restituisce il valore aggiornato.

6. `uint8_t num_to_BCD(uint8_t value, uint8_t unitBits)`: Questa funzione converte un numero decimale in formato BCD con il numero di bit specificato.

7. `uint8_t BCD_to_num(uint8_t valueBCD, uint8_t unitBits, uint8_t decBits)`: Questa funzione converte un numero in formato BCD in un numero decimale con il numero di bit specificato.

8. `uint8_t mcp7940_set_allarm(uint8_t allarmNumber, uint8_t allarmMask, uint8_t reg_address, uint8_t valueBCD, bool interruptHighLow)`: Questa funzione imposta un allarme sull'integrato MCP7940, specificando il numero dell'allarme, la maschera dell'allarme, l'indirizzo del registro, il valore in formato BCD e se l'allarme deve generare un interrupt alto o basso. Restituisce 1 in caso di successo e 0 in caso di errore.

9. `void mcp7940_set_all_data(uint8_t secs, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year)`: Questa funzione imposta tutti i dati dell'orario sull'integrato MCP7940 con i valori specificati.

10. `void mcp7940_set_time(uint8_t secs, uint8_t min, uint8_t hour)`: Questa funzione imposta l'orario sull'integrato MCP7940 con i valori specificati per i secondi, i minuti e le ore.

11. `const char* getDayName(uint8_t day)`: Questa funzione restituisce il nome del giorno corrispondente al valore del giorno specificato.

12. `uint32_t mcp7940_get_time(uint8_t* buf)`: Questa funzione converte i dati dell'orario letti dall'integrato MCP7940 in un valore uint32_t.

13. `uint32_t mcp7940_get_data(uint8_t* buf, uint8_t offset)`: Questa funzione converte i dati della data letti dall'integrato MCP7940 in un valore uint32_t, con un offset specificato.

Queste funzioni sono utilizzate per gestire l'orario in tempo reale e altri aspetti temporali del progetto.

---

Il file `mcp7940.c` contiene le definizioni delle funzioni necessarie per l'interazione con l'integrato MCP7940, che gestisce l'orologio in tempo reale (RTC) e altre funzionalità temporali. Le funzioni pubbliche includono la lettura e la scrittura di dati, la gestione degli allarmi e la conversione tra formati BCD e decimale.

Questo file è essenziale per il corretto funzionamento del sistema di gestione del tempo nel progetto.


