# e-commerce

## TODO list

- [ ] refactoring
    - [x] usare costanti per dichiarazione dimensione buffer
    - [x] INVALID#REQUEST e REQUEST_SUCCESS va messo l'underscore
    - [ ] controllare print fatti e togliere quelli inutili
    - [ ] filtrare librerie necessarie

- relazione
    - sistemare descrzione problema
    - figura che illustra sistema 
    - figura che illustra l'ambiente (è quello che abbiamo messo nella system architecture piccolo???)
    - controllare requisiti di sistema scritti
    - controllare diagramma architettura sistema (è quello che abbiamo messo nella system architecture grande)
    - activity diagram per mostrare come le varie componenti intervengono per soddisfare i requisiti funzionali (almeno uno)
    - state diagram per una delle componenti del sistema (almeno uno)
    - message sequence chart diagram per la comunicazione tra le componenti del sistema (almeno uno)
    - descrizione generale implementazione per ogni componente (manager, server, handler)
    - schema dei db (er log, er principale)
    - descrizione connessioni Redis
    - descrizioni risultati

- [x] fare server per ogni handler
- [x] test generator
- [x] monitor (funzionali e non funzionali)
    - [x] non funzionali:
        - [x] il tempo medio di sessione
        - [x] il tempo medio di risposta ad una richiesta
    - funzionali
        - [x] loggare i risultati delle richiete nel db perchè i monitor sono i trigger ed i vincoli messi nella creazione del db
- [x] creare db log
- [x] loggare i dati
- [ ] vedere come, caso di sigint, chiudere le connessioni
- [ ] risolvere i seguenti tests
    - [ ] 2 view-order customer view-order purchase_id -73 REQUEST_SUCCESS (select su prodotto)
    - [ ] 2 search-products customer search-products product_name "cose a caso" REQUEST_SUCCESS (non dovrebbe essere un errore, maybe brutforce sui test, ma attentezione su il fatto che debba essere vero o meno in base anche ad altri parametri)
    - [ ] 2 update-refund-request courier update-refund-request refund -34 refund_start 2024-01-29##15:30:37 (select)
    - [ ] 2 delete-purchase customer delete-purchase purchase -11 REQUEST_SUCCESS (select)

### Altro

### Relazione

- generale
    - diagramma del sistema
    - diagramma dell'ambiente operativo del sistema

- requisiti utenti
    - requisiti numerati
    - diagrammi UML per almeno 2 use case

- requisiti di sistema
    - diagramma dell'architettura
    - almeno un activity diargram UML
    - almeno uno state diagram UML per almeno una componente
    - almeno un message sequence chart UML

- implementazione
    - pseudo-codice per tutte le componenti
    - schema dei DB
    - descrizione delle connessioni redis

- risultati
    - descrizione dei risultati

### Progetto

- elementi
    - test generator (environment)
        - customers
        - fornitori
        - trasportatori
    - system under design
        - redis
        - DB postgresql per i log
    - monitor
        - almeno 3 per i requisiti funzionali
        - almeno 2 per i requisiti non-funzionali

- componenti
    - uno o più server per i customers
    - uno o più server per i fornitori
    - uno o più server per i produttori
    - DB dei dati

- [x] funzionalità
    - [x] customers
        - [x] registrazione customer
        - [x] ricerca prodotti
        - [x] acquisto prodotti
        - [x] annullamento ordini
        - [x] restituzione prodotti
        - [x] feedback prodotti
        - [x] registrazione carte
        - [x] registrazione indirizzi
        - [x] vedere i propri ordini
    - [x] fornitori
        - [x] registrazione fornitore
        - [x] gestione prodotti
        - [x] classifica prodotti maggiormente venduti in range temporale
    - [x] trasportatori
        - [x] registrazione trasportatore
        - [x] gestione spedizioni
