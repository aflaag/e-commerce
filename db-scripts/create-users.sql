-- create the database :dbname
DROP DATABASE IF EXISTS :dbname ;
CREATE DATABASE :dbname ;

-- create the user :dbadmin
\c :dbname postgres

REASSIGN OWNED BY :dbadmin TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :dbadmin ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :dbadmin ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :dbadmin ;

DROP OWNED BY :dbadmin;
DROP USER IF EXISTS :dbadmin ;

CREATE USER :dbadmin WITH ENCRYPTED PASSWORD 'dbadmin' ;

-- grant privileges to :dbadmin
-- \c :dbname postgres

-- GRANT ALL PRIVILEGES ON DATABASE :dbname to :dbadmin ;
-- GRANT ALL ON SCHEMA public TO :dbadmin ;

-- create the user :customer
\c :dbname postgres

REASSIGN OWNED BY :customer TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :customer ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :customer ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :customer ;

DROP OWNED BY :customer;
DROP USER IF EXISTS :customer ;

CREATE USER :dbadmin WITH ENCRYPTED PASSWORD 'customer' ;

-- create the user :supplier
\c :dbname postgres

REASSIGN OWNED BY :supplier TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :supplier ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :supplier ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :supplier ;

DROP OWNED BY :supplier ;
DROP USER IF EXISTS :supplier ;

CREATE USER :dbadmin WITH ENCRYPTED PASSWORD 'supplier' ;

-- create the user :courier
\c :dbname postgres

REASSIGN OWNED BY :courier TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :courier ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :courier ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :courier ;

DROP OWNED BY :courier ;
DROP USER IF EXISTS :courier ;

CREATE USER :dbadmin WITH ENCRYPTED PASSWORD 'supplier' ;