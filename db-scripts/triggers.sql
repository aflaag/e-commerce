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

CREATE OR REPLACE TRIGGER consistent_card
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

CREATE OR REPLACE TRIGGER consistent_address
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

CREATE OR REPLACE TRIGGER consistent_rating
AFTER INSERT ON rating FOR EACH ROW EXECUTE FUNCTION consistent_rating();