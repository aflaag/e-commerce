\c ecommerce

INSERT INTO Address (zip_code, street, street_number, city)
VALUES  ('01028', 'boh', '1a', 1);

Insert INTO AddCust (customer, zip_code, street, street_number)
VALUES ('user1@gmail.com', '01028', 'boh', '1a');

INSERT INTO Purchase (purchase_instant, fare, customer, card, zip_code, street, street_number)
VALUES ('2024-02-22 10:30:00', 0.0, 'user1@gmail.com', '1234567890123456', '01028', 'boh', '1a');

INSERT INTO OrderedProducts (product, purchase, quantity)
VALUES ('P1', 1, 10);

INSERT INTO AssignedDelivery (delivery_code, delivery_assignment_instant, delivery_state, courier, purchase,delivery_start, delivery_end)
VALUES ('bo', '2024-02-22 10:30:00', 'Delivered', 'courier1', 1, '2024-02-22 10:30:00', '2024-02-22 10:30:00');

INSERT INTO RefundRequest (request_instant, refund_state, assigned_delivery)
VALUES ('2024-02-22 10:30:00', 'Requested', 'bo');

INSERT INTO RefundedProduct (refund_request, product, quantity)
VALUES (1, 'P1', 2);

INSERT INTO RefundRequest (request_instant, refund_state, assigned_delivery)
VALUES ('2024-02-22 10:30:00', 'Requested', 'bo');