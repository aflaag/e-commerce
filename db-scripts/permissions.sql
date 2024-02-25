-- try to understant if we need USAGE ON LANGUAGE
\c :dbname postgres

-- user already exists
GRANT ALL PRIVILEGES ON DATABASE :dbname to :admin ;
GRANT CONNECT ON DATABASE :dbname TO :courier ;
GRANT CONNECT ON DATABASE :dbname TO :supplier ;
GRANT CONNECT ON DATABASE :dbname TO :customer ;


ALTER TABLE Country OWNER TO :admin ;
ALTER TABLE City OWNER TO :admin ;
ALTER TABLE Address OWNER TO :admin ;
ALTER TABLE Customer OWNER TO :admin ;
ALTER TABLE AddCust OWNER TO :admin ;
ALTER TABLE Card OWNER TO :admin ;
ALTER TABLE Supplier OWNER TO :admin ;
ALTER TABLE Product OWNER TO :admin ;
ALTER TABLE Restock OWNER TO :admin ;
ALTER TABLE Rating OWNER TO :admin ;
ALTER TABLE Purchase OWNER TO :admin ;
ALTER TABLE OrderedProducts OWNER TO :admin ;
ALTER TABLE Courier OWNER TO :admin ;
ALTER TABLE AssignedDelivery OWNER TO :admin ;
ALTER TABLE RefundRequest OWNER TO :admin ;
ALTER TABLE RefundedProduct OWNER TO :admin ;


-- grant all privileges on all tables in schema public to :username ;
-- grant all privileges on all sequences in schema public to :username ;

GRANT ALL ON SCHEMA public TO :admin ;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin ;

GRANT SELECT ON Purchase TO :courier ;
GRANT INSERT, UPDATE ON AssignedDelivery TO :courier ;
GRANT UPDATE ON RefundRequest TO :courier ;

GRANT INSERT, UPDATE, SELECT, DELETE ON Product TO :supplier ;
GRANT INSERT, SELECT ON Product TO :supplier ;
GRANT INSERT ON Supplier TO :supplier ;

GRANT INSERT ON Rating TO :customer ;
GRANT SELECT, INSERT ON RefundRequest TO :customer ;
GRANT SELECT ON Product TO :customer ;
GRANT INSERT ON Purchase TO :customer ;
GRANT INSERT ON Address TO :customer ;
GRANT SELECT, INSERT ON AddCust TO :customer ;
GRANT SELECT, UPDATE ON Purchase TO :customer ;
GRANT SELECT, INSERT ON Card TO :customer ;
GRANT SELECT ON AssignedDelivery TO :customer ;
GRANT INSERT ON Customer TO :customer ;