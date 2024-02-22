\c ecommerce

INSERT INTO Address (zip_code, street, street_number, city)
VALUES  ('01028', 'boh', '1a', 1);

Insert INTO AddCust (customer, zip_code, street, street_number)
VALUES ('user1@gmail.com', '01028', 'boh', '1a');

WITH p2 AS (
INSERT INTO Purchase (purchase_instant, fare, customer, card, zip_code, street, street_number)
VALUES ('2024-02-22 10:30:00', 0.0, 'user1@gmail.com', '1234567890123456', '01028', 'boh', '1a')
)
INSERT INTO OrderedProducts (product, purchase, quantity)
VALUES ('P1', 1, 10);