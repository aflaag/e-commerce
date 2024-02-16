# e-commerce

## TODO list

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

- funzionalità
    - customers
        - registrazione ed autenticazione customer
        - ricerca prodotti
        - acquisto prodotti
        - annullamento ordini
        - restituzione prodotti
        - feedback prodotti
        - feedback spedizioni
    - fornitori
        - registrazione ed autenticazione fornitore
        - gestione prodotti
            - registrazione prodotti
            - aggiornamento prodotti
            - rimozione prodotti
        - affiliazione trasportatori
        - statistiche prodotti
            - classifica prodotti maggiormente venduti negli ultimi X giorni
    - trasportatori
        - trasporto prodotti
