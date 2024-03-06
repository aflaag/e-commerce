\c :dbname 

CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringM AS VARCHAR(100);
CREATE DOMAIN StringL AS VARCHAR(200);

CREATE DOMAIN IntGZ AS integer CHECK (VALUE > 0);
CREATE DOMAIN IntGEZ AS integer CHECK (VALUE >= 0);

CREATE DOMAIN RealGZ AS real CHECK (VALUE > 0);
CREATE DOMAIN RealGEZ AS real CHECK (VALUE >= 0);

CREATE DOMAIN Email AS VARCHAR(255) CHECK (VALUE ~* E'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$');
CREATE DOMAIN PhoneNumber AS VARCHAR(20) CHECK (VALUE ~* E'^\\+?[0-9-]+[0-9]+$');
CREATE DOMAIN CardNumber AS VARCHAR(20) CHECK (VALUE ~* E'^[0-9]{13,19}$');
CREATE DOMAIN ProductCode AS StringS CHECK (VALUE ~* E'^[A-Z0-9]{1,20}$');

CREATE DOMAIN Stars AS integer CHECK (VALUE >= 1 AND VALUE <= 5);

CREATE TYPE DeliveryState AS ENUM ('Assigned', 'OnGoing', 'Lost', 'Delivered');
CREATE TYPE RefundState AS ENUM ('Requested', 'Assigned', 'OnGoing', 'Refunded');

CREATE TABLE IF NOT EXISTS Country (
        name StringS NOT NULL PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS City (
        id SERIAL PRIMARY KEY,
        name StringS NOT NULL,
        country StringS NOT NULL,

        CONSTRAINT fk_city_country FOREIGN KEY (country) REFERENCES Country(name)
);

CREATE TABLE IF NOT EXISTS Address (
        zip_code StringS NOT NULL,
        street StringM NOT NULL,
        street_number StringS NOT NULL,
        city integer NOT NULL,

        PRIMARY KEY (zip_code, street, street_number),
        CONSTRAINT fk_address_city FOREIGN KEY (city) REFERENCES City(id)
);

CREATE TABLE iF NOT EXISTS Customer (
        email Email NOT NULL PRIMARY KEY,
        name StringS NOT NULL,
        surname StringS NOT NULL,
        phone_number PhoneNumber NOT NULL
);

CREATE TABLE IF NOT EXISTS AddCust (
        customer Email NOT NULL,
        zip_code StringS NOT NULL,
        street StringM NOT NULL,
        street_number StringS NOT NULL,

        PRIMARY KEY (customer, zip_code, street, street_number),
        CONSTRAINT fk_addcust_customer FOREIGN KEY (customer) REFERENCES Customer(email),
        CONSTRAINT fk_addcust_address FOREIGN KEY (zip_code, street, street_number) REFERENCES Address(zip_code, street, street_number)
);

CREATE TABLE IF NOT EXISTS Card (
        number CardNumber NOT NULL PRIMARY KEY,
        customer Email NOT NULL,

        CONSTRAINT fk_card_customer FOREIGN KEY (customer) REFERENCES Customer(email)
);

CREATE TABLE IF NOT EXISTS Supplier (
        business_name StringS NOT NULL PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS Product (
        code ProductCode NOT NULL PRIMARY KEY,
        name StringS NOT NULL,
        description StringL NOT NULL,
        price RealGEZ NOT NULL
);

CREATE TABLE IF NOT EXISTS Restock (
        id SERIAL PRIMARY KEY,
        quantity IntGZ NOT NULL,
        supplier StringS NOT NULL,
        product StringS NOT NULL,

        CONSTRAINT fk_restock_supplier FOREIGN KEY (supplier) REFERENCES Supplier(business_name),
        CONSTRAINT fk_restock_product FOREIGN KEY (product) REFERENCES Product(code)
);

CREATE TABLE IF NOT EXISTS Rating (
        product StringS NOT NULL,
        customer Email NOT NULL,
        stars Stars NOT NULL,

        PRIMARY KEY (product, customer),
        CONSTRAINT fk_rating_product FOREIGN KEY (product) REFERENCES Product(code),
        CONSTRAINT fk_rating_customer FOREIGN KEY (customer) REFERENCES Customer(email)
);

CREATE TABLE IF NOT EXISTS Purchase (
        id SERIAL PRIMARY KEY,
        purchase_instant timestamp NOT NULL,
        cancel_instant timestamp,
        fare RealGEZ NOT NULL,
        customer Email NOT NULL,
        card CardNumber NOT NULL,
        zip_code StringS NOT NULL,
        street StringM NOT NULL,
        street_number StringS NOT NULL,

        CONSTRAINT fk_purchase_customer FOREIGN KEY (customer) REFERENCES Customer(email),
        CONSTRAINT fk_purchase_card FOREIGN KEY (card) REFERENCES Card(number),
        CONSTRAINT fk_purchase_address FOREIGN KEY (zip_code, street, street_number) REFERENCES Address(zip_code, street, street_number),
        CONSTRAINT cancel_instant_after_purchase CHECK (cancel_instant IS NULL OR cancel_instant >= purchase_instant)
);

CREATE TABLE IF NOT EXISTS OrderedProducts (
        product StringS NOT NULL,
        purchase integer NOT NULL,
        quantity IntGZ NOT NULL,

        PRIMARY KEY (product, purchase),
        CONSTRAINT fk_orderedproducts_product FOREIGN KEY (product) REFERENCES Product(code),
        CONSTRAINT fk_orderedproduct_purchase FOREIGN KEY (purchase) REFERENCES Purchase(id)
);

CREATE TABLE IF NOT EXISTS Courier (
        business_name StringS NOT NULL PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS AssignedDelivery (
        delivery_code StringS NOT NULL PRIMARY KEY,
        delivery_assignment_instant timestamp NOT NULL,
        delivery_start timestamp,
        delivery_end timestamp,
        delivery_state DeliveryState NOT NULL,
        lost_instant timestamp,
        courier StringS NOT NULL,
        purchase integer NOT NULL UNIQUE,

        CONSTRAINT fk_assigneddelivery_courier FOREIGN KEY (courier) REFERENCES Courier(business_name),
        CONSTRAINT fk_assigneddelivery_purchase FOREIGN KEY (purchase) REFERENCES Purchase(id),

        CONSTRAINT delivery_start_after_assignment CHECK (delivery_start IS NULL OR delivery_start >= delivery_assignment_instant),
        CONSTRAINT lost_after_delivery_start CHECK (lost_instant IS NULL OR lost_instant >= delivery_start),
        CONSTRAINT delivery_end_after_delivery_start CHECK (delivery_end IS NULL OR delivery_end >= delivery_start),
        CONSTRAINT ongoing_lost_delivery_disjunction CHECK (delivery_end IS NULL OR lost_instant IS NULL),
        
        CONSTRAINT assigned_delivery_status1 CHECK (delivery_start IS NOT NULL OR delivery_end IS NOT NULL OR lost_instant IS NOT NULL OR delivery_state = 'Assigned'),
        CONSTRAINT assigned_delivery_status2 CHECK (delivery_state != 'Assigned' OR (delivery_start IS NULL AND delivery_end IS NULL AND lost_instant IS NULL)),
        
        CONSTRAINT ongoing_delivery_status1 CHECK (delivery_start IS NULL OR delivery_end IS NOT NULL OR lost_instant IS NOT NULL OR delivery_state = 'OnGoing'),
        CONSTRAINT ongoing_delivery_status2 CHECK (delivery_state != 'OnGoing' OR (delivery_start IS NOT NULL AND delivery_end IS NULL AND lost_instant IS NULL)),
        
        CONSTRAINT delivered_delivery_status1 CHECK (delivery_start IS NULL OR delivery_end IS NULL OR lost_instant IS NOT NULL OR delivery_state = 'Delivered'),
        CONSTRAINT delivered_delivery_status2 CHECK (delivery_state != 'Delivered' OR (delivery_start IS NOT NULL AND delivery_end IS NOT NULL AND lost_instant IS NULL)),

        CONSTRAINT lost_delivery_status1 CHECK (delivery_start IS NULL OR delivery_end IS NOT NULL OR lost_instant IS NULL OR delivery_state = 'Lost'),
        CONSTRAINT lost_delivery_status2 CHECK (delivery_state != 'Lost' OR (delivery_start IS NOT NULL AND delivery_end IS NULL AND lost_instant IS NOT NULL))
);

CREATE TABLE IF NOT EXISTS RefundRequest (
        id SERIAL PRIMARY KEY,
        request_instant timestamp NOT NULL,
        refund_assignment_instant timestamp,
        refund_start timestamp,
        refund_end timestamp,
        refund_state RefundState NOT NULL,
        assigned_delivery StringS NOT NULL,
        courier StringS,

        CONSTRAINT fk_refundrequest_assigneddelivery FOREIGN KEY (assigned_delivery) REFERENCES AssignedDelivery(delivery_code),
        CONSTRAINT fk_refundrequest_courier FOREIGN KEY (courier) REFERENCES Courier(business_name),
        CONSTRAINT assigned_refund_instant CHECK ((courier IS NULL AND refund_assignment_instant IS NULL) OR (courier IS NOT NULL AND refund_assignment_instant IS NOT NULL)), 
        CONSTRAINT refund_assignment_after_request_instant CHECK (refund_assignment_instant IS NULL OR refund_assignment_instant >= request_instant),
        CONSTRAINT refund_start_after_refund_assignment CHECK (refund_start IS NULL OR refund_start >= refund_assignment_instant),
        CONSTRAINT refund_end_after_refund_start CHECK (refund_end IS NULL OR refund_end >= refund_start),

        CONSTRAINT requested_refund_status1 CHECK (refund_assignment_instant IS NOT NULL OR refund_start IS NOT NULL OR refund_end IS NOT NULL OR refund_state = 'Requested'),
        CONSTRAINT requested_refund_status2 CHECK (refund_state != 'Requested' OR (refund_assignment_instant IS NULL AND refund_start IS NULL AND refund_end IS NULL )),

        CONSTRAINT assigned_refund_status1 CHECK (refund_assignment_instant IS NULL OR refund_start IS NOT NULL OR refund_end IS NOT NULL OR refund_state = 'Assigned'),
        CONSTRAINT assigned_refund_status2 CHECK (refund_state != 'Assigned' OR (refund_assignment_instant IS NOT NULL AND refund_start IS NULL AND refund_end IS NULL )),

        CONSTRAINT ongoing_refund_status1 CHECK (refund_assignment_instant IS NULL OR refund_start IS NULL OR refund_end IS NOT NULL OR refund_state = 'OnGoing'),
        CONSTRAINT ongoing_refund_status2 CHECK (refund_state != 'OnGoing' OR (refund_assignment_instant IS NOT NULL AND refund_start IS NOT NULL AND refund_end IS NULL )),

        CONSTRAINT refunfef_refund_status1 CHECK (refund_assignment_instant IS NULL OR refund_start IS NULL OR refund_end IS NULL OR refund_state = 'Refunded'),
        CONSTRAINT refunded_refund_status2 CHECK (refund_state != 'Requested' OR (refund_assignment_instant IS NULL AND refund_start IS NULL AND refund_end IS NULL))
);

CREATE TABLE IF NOT EXISTS RefundedProduct (
        refund_request integer NOT NULL,
        product StringS NOT NULL,
        quantity IntGZ NOT NULL,

        PRIMARY KEY (refund_request, product),
        CONSTRAINT fk_refundedproduct_refundrequest FOREIGN KEY (refund_request) REFERENCES RefundRequest(id),
        CONSTRAINT fk_refundedproduct_product FOREIGN KEY (product) REFERENCES Product(code)
);
