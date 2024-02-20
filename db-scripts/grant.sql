
\c :dbname postgres

-- user already exists
GRANT ALL PRIVILEGES ON DATABASE :dbname to :admin ;


-- ALTER TABLE TimeVar OWNER TO :admin ;
-- ALTER TABLE LogTable OWNER TO :admin ;

-- grant all privileges on all tables in schema public to :admin ;
-- grant all privileges on all sequences in schema public to :admin ;

GRANT ALL ON SCHEMA public TO :admin ;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin ;
