-- This creates the database :dbname
drop database if exists :dbname ;
CREATE DATABASE :dbname ;

-- This creates the user :dbadmin

\c :dbname postgres

-- erase role if already created
REASSIGN OWNED BY :dbadmin TO postgres ;
REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :dbadmin ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :dbadmin ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :dbadmin ;
DROP OWNED BY :dbadmin;
DROP USER IF EXISTS :dbadmin ;
CREATE USER :dbadmin WITH ENCRYPTED PASSWORD '1337' ;
-- user created

\c :dbname postgres

-- grant privileges to user
GRANT ALL PRIVILEGES ON DATABASE :dbname to :dbadmin ;
GRANT ALL ON SCHEMA public TO :dbadmin ;
