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

WITH p2 AS (
INSERT INTO Purchase (purchase_instant, fare, customer, card, zip_code, street, street_number)
VALUES ('2024-02-22 10:30:00', 0.0, 'user1@gmail.com', '1234567890123456', '01028', 'boh', '1a')
)
INSERT INTO OrderedProducts (product, purchase, quantity)
VALUES ('P1', 2, 10);
INSERT INTO OrderedProducts (product, purchase, quantity)
VALUES ('P2', 2, 20);

INSERT INTO AssignedDelivery 
VALUES('bho2','2024-02-22 10:30:00','2024-02-22 10:30:00', '2024-02-22 10:30:00','Delivered',null, 'courier1',2);

INSERT INTO Restock (quantity, supplier, product)
VALUES(100, 'Supplier1', 'P1');

INSERT INTO RefundRequest (request_instant, refund_state, assigned_delivery)
VALUES ('2024-02-22 10:30:00', 'Requested', 'bho2');

INSERT INTO RefundedProduct (refund_request, product, quantity)
VALUES (1, 'P1', 10);

INSERT INTO RefundedProduct (refund_request, product, quantity)
VALUES (1, 'P2', 10);

SELECT rp.product AS product, rp.quantity AS quantity, rr.refund_state AS state,rr.request_instant AS request_instant, rr.refund_assignment_instant AS assigned, rr.refund_start AS refund_start, rr.refund_end AS refund_end, rr.courier AS courier, ad.purchase AS purchase 
FROM RefundRequest AS rr, RefundedProduct AS rp, AssignedDelivery AS ad 
WHERE rr.id = '1' AND rr.id = rp.refund_request AND ad.delivery_code = rr.assigned_delivery;