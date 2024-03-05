\c :log_dbname 

CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringL AS VARCHAR(250);

CREATE TABLE IF NOT EXISTS Client (
        file_descriptor integer NOT NULL,
        connection_instant timestamp NOT NULL,
        disconnection_instant timestamp,

        PRIMARY KEY (file_descriptor, connection_instant),
        CONSTRAINT disconnection_instant_after_connection_instant CHECK (disconnection_instant IS NULL OR disconnection_instant >= connection_instant)
);

CREATE TABLE IF NOT EXISTS Communication (
        request StringL NOT NULL,
        request_instant timestamp NOT NULL,
        response StringS,
        response_instant timestamp,
        client_file_descriptor integer NOT NULL,
        client_connection_instant timestamp NOT NULL,

        PRIMARY KEY (client_file_descriptor, client_connection_instant, request_instant),
        CONSTRAINT fk_communication_client FOREIGN KEY (client_file_descriptor, client_connection_instant) REFERENCES Client(file_descriptor, connection_instant),
        CONSTRAINT response_instant_after_request_instant CHECK (response_instant IS NULL OR response_instant >= request_instant),
        CONSTRAINT request_instant_after_connection_instant CHECK (request_instant >= client_connection_instant)
);