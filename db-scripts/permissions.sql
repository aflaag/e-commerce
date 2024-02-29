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

GRANT ALL ON SCHEMA public TO :admin ;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin ;

-- grant usage and select permission to autoincrements
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :admin ;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :customer ;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :courier ;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :supplier ;

GRANT SELECT ON Purchase TO :courier ;
GRANT INSERT, UPDATE ON AssignedDelivery TO :courier ;
GRANT UPDATE ON RefundRequest TO :courier ;

GRANT INSERT, UPDATE, SELECT, DELETE ON Product TO :supplier ;
GRANT INSERT, SELECT ON Product TO :supplier ;
GRANT INSERT ON Supplier TO :supplier ;

GRANT INSERT ON Rating TO :customer ;
GRANT SELECT, INSERT ON RefundRequest TO :customer ;
GRANT SELECT ON Product TO :customer ;
GRANT SELECT, INSERT ON Address TO :customer ;
GRANT SELECT, INSERT ON AddCust TO :customer ;
GRANT SELECT, INSERT, UPDATE, DELETE ON Purchase TO :customer ;
GRANT SELECT, INSERT ON Card TO :customer ;
GRANT SELECT ON AssignedDelivery TO :customer ;
GRANT SELECT, INSERT ON Customer TO :customer ;
GRANT SELECT, INSERT ON OrderedProducts TO :customer ;
GRANT SELECT, INSERT ON RefundedProduct TO :customer ;
GRANT SELECT ON Restock TO :customer ;
