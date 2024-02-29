# e-commerce

## TODO list

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
