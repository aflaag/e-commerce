\c :dbname

-- we can't insert addresses because they have an unknow id (SERIAL)
-- amd all the things related to it like punrchase and so on

INSERT INTO Country (name) VALUES
    ('United Kingdom'),
    ('Germany'),
    ('France'),
    ('Italy'),
    ('Spain') ;

INSERT INTO City (name, country) VALUES
    ('London', 'United Kingdom'),
    ('Manchester', 'United Kingdom'),
    ('Berlin', 'Germany'),
    ('Munich', 'Germany'),
    ('Paris', 'France'),
    ('Marseille', 'France'),
    ('Rome', 'Italy'),
    ('Milan', 'Italy'),
    ('Madrid', 'Spain'),
    ('Barcelona', 'Spain') ;

INSERT INTO Customer (email, name, surname, phone_number) VALUES
    ('user1@gmail.com', 'user1', 'boh', '+15551234'),
    ('user2@gmail.com', 'user2', 'boh', '+442012345678'),
    ('user3@gmail.com', 'user3', 'boh', '+14165557890') ;

INSERT INTO Supplier (business_name)
VALUES
    ('Supplier1'),
    ('Supplier2'),
    ('Supplier3'),
    ('Supplier4') ;

INSERT INTO Product (code, name, description, price)
VALUES
    ('P1', 'Laptop', 'High-performance laptop', 999.99),
    ('P2', 'Smartphone', 'Latest smartphone model', 499.99),
    ('P3', 'Tablet', 'Slim and lightweight tablet', 299.99),
    ('P4', 'Smartwatch', 'Fitness tracking smartwatch', 149.99) ;

INSERT INTO Restock (quantity, supplier, product)
VALUES
    (100, 'Supplier1', 'P1'),
    (100, 'Supplier1', 'P1'),
    (50, 'Supplier2', 'P2'),
    (75, 'Supplier3', 'P3'),
    (40, 'Supplier4', 'P4'),
    (40, 'Supplier4', 'P4') ;

INSERT INTO Card (number, customer)
VALUES
    ('1234567890123456', 'user1@gmail.com'),
    ('9876543210987654', 'user2@gmail.com'),
    ('1111222233334444', 'user3@gmail.com');

INSERT INTO Courier (business_name)
VALUES
    ('courier1'),
    ('courier2'),
    ('courier3');

