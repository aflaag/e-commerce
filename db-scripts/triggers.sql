\c :dbname 

-- drop all functions and triggers
DO $$
DECLARE
    function_name TEXT;
BEGIN
    FOR function_name IN
        SELECT p.proname
        FROM pg_proc p
        INNER JOIN pg_namespace n ON p.pronamespace = n.oid
        WHERE n.nspname = 'public'
    LOOP
        EXECUTE 'DROP FUNCTION IF EXISTS ' || function_name || ' CASCADE';
    END LOOP;
END $$;

-- [V.Card.cardAssociata]
CREATE OR REPLACE FUNCTION consistent_card() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 FROM Card, Customer
        WHERE NEW.card = Card.number
        AND NEW.customer = Customer.email
        AND Card.customer = Customer.email
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid card';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consistent_card_trg
AFTER INSERT ON Purchase FOR EACH ROW EXECUTE FUNCTION consistent_card();

-- [V.Address.indirizzoConsistente]
CREATE OR REPLACE FUNCTION consistent_address() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 FROM Address, Customer, AddCust
        WHERE NEW.zip_code = Address.zip_code
        AND NEW.street = Address.street
        AND NEW.street_number = Address.street_number
        AND NEW.customer = Customer.email
        AND AddCust.zip_code = Address.zip_code
        AND AddCust.street = Address.street
        AND AddCust.street_number = Address.street_number
        AND AddCust.customer = Customer.email
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid address';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consistent_address_trg
AFTER INSERT ON Purchase FOR EACH ROW EXECUTE FUNCTION consistent_address();

-- [V.Customer.ratingProdottiAcquistati]
CREATE OR REPLACE FUNCTION consistent_rating() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 FROM Customer, Product
        WHERE NEW.product = Product.code
        AND Customer.email = NEW.customer
        AND EXISTS (
            SELECT 1 FROM OrderedProducts, Purchase
            WHERE NEW.product = OrderedProducts.product
            AND Purchase.customer = Customer.email
            AND Purchase.id = OrderedProducts.purchase
        )
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid rating';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consistent_rating_trg
AFTER INSERT ON rating FOR EACH ROW EXECUTE FUNCTION consistent_rating();

-- [V.AssignedDelivery.istante]
CREATE OR REPLACE FUNCTION consistent_assignementdelivery_instant() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 FROM Purchase
        WHERE NEW.purchase = Purchase.id
        AND NEW.delivery_assignment_instant >= Purchase.purchase_instant
        AND Purchase.cancel_instant IS NULL
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid assignement instant';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consistent_assignementdelivery_instant_trg
AFTER INSERT ON AssignedDelivery FOR EACH ROW EXECUTE FUNCTION consistent_assignementdelivery_instant();


-- [V.RefundRequest.istante]
CREATE OR REPLACE FUNCTION consistent_refund_request_instant() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 
        FROM AssignedDelivery
        WHERE NEW.assigned_delivery = delivery_code
        AND NEW.request_instant >= delivery_end
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid refund instant';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consistent_refund_request_instant_trg
AFTER INSERT ON RefundRequest FOR EACH ROW EXECUTE FUNCTION consistent_refund_request_instant();

-- [V.Purchase.disjunction]
CREATE OR REPLACE FUNCTION purchase_disjunction() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 
        FROM AssignedDelivery
        WHERE NEW.cancel_instant IS NOT NULL
        AND NEW.id = purchase
    ) IS TRUE THEN
        RAISE EXCEPTION 'invalid cancel request';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER purchase_disjunction_trg
AFTER UPDATE ON Purchase FOR EACH ROW EXECUTE FUNCTION purchase_disjunction();

-- [V.RefoundRequest.prodottiConsistenti]
CREATE OR REPLACE FUNCTION refunded_products_consistency() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 
        FROM RefundRequest AS rr, AssignedDelivery AS ad, Purchase AS p, OrderedProducts as op
        WHERE NEW.refund_request = rr.id 
        AND rr.assigned_delivery = ad.delivery_code
        AND ad.purchase = p.id
        AND op.purchase = p.id
        AND op.product = NEW.product
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid refund request';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER refunded_products_consistency_trg
AFTER INSERT ON RefundedProduct FOR EACH ROW EXECUTE FUNCTION refunded_products_consistency();

-- [V.RefoundedProducts.QuantityConsistency]
CREATE OR REPLACE FUNCTION refunded_products_quantity_consistency() RETURNS TRIGGER AS $$
BEGIN
    IF (
        WITH purchase_id AS (
            SELECT ad.purchase AS id
            FROM RefundRequest AS rr, AssignedDelivery AS ad
            WHERE NEW.refund_request = rr.id
            AND rr.assigned_delivery = ad.delivery_code
        )
        SELECT SUM(DISTINCT op.quantity) >= SUM(rp.quantity)
        FROM RefundRequest AS rr, 
             AssignedDelivery AS ad,
             OrderedProducts as op,
             RefundedProduct as rp,
             purchase_id
        WHERE NEW.product = op.product
        AND NEW.product = rp.product
        AND op.purchase = purchase_id.id
        AND rr.id = rp.refund_request
        AND rr.assigned_delivery = ad.delivery_code
        AND ad.purchase = purchase_id.id
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid quantity refund request';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER refunded_products_quantity_consistency_trg
AFTER INSERT ON RefundedProduct FOR EACH ROW EXECUTE FUNCTION refunded_products_quantity_consistency();

--[V.Purchase.restockDisponibile]
CREATE OR REPLACE FUNCTION ordered_product_quantity_consistency() RETURNS TRIGGER AS $$
BEGIN
    IF (
        WITH restock_quantity AS (
            SELECT SUM(quantity) AS quantity
            FROM Restock
            WHERE product = NEW.product
        )
        SELECT SUM(DISTINCT rq.quantity) >= SUM(op.quantity)
        FROM OrderedProducts AS op, restock_quantity as rq
        WHERE op.product = NEW.product
    ) IS FALSE THEN
        RAISE EXCEPTION 'invalid quantity for order request';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER ordered_product_quantity_consistency_trg
AFTER INSERT ON OrderedProducts FOR EACH ROW EXECUTE FUNCTION ordered_product_quantity_consistency();