# API DOC

## Supplier

- add-product
  - code: `ProductCode`
  - name: `StringS`
  - description: `StringL`
  - price: `RealGEZ`
      ```
      add-product code banana name yoyo description ahahahaha##baba price 341.3243
      ```

- add-restock
  - quantity: `IntGZ`
  - supplier: `StringS`
  - product: `ProductCode`
      ```
      add-restock quantity 10 supplier Supplier1 product P1
      ```

- add-supplier
  - business_name: `StringS`
      ```
      add-supplier business_name Supplier5
      ```

- update-product
  - code: `ProductCode`
  - description: `StringL`
      ```
      update-product code P1 description ho##capito
      ```

- view-statistic
  - supplier: `StringS`
      ```
      view-statistic supplier Supplier1
      ```

## Courier

- take-purchase-delivery
  - courier: `StringS`
  - purchase: `integer`
  - deliverycode: `DeliveryCode`
      ```
      take-purchase-delivery courier courier1 purchase 1 deliverycode aifjifa
      ```

- update-assigned-delivery
  - deliverycode: `DeliveryCode`
  - start: `timestamp`
  - lost: `timestamp`
  - end: `timestamp`
      ```
      update-assigned-delivery deliverycode bho2 lost 2024-02-29##19:41:42
      ```

- update-refund-request
  - refund: `integer`
  - _switch_
    - _option_
      - courier: `StringS`
          ```
          update-refund-request TODO
          ```
    - _option_
      - refund_start: `timestamp`
      - refund_end: `timestamp`
          ```
          update-refund-request refund 1 refund_end 2024-03-02##11:05:42.660825
          ```

- view-available-purchase
      ```sh
      view-available-purchase
      ```

## Customer

- add-address
  - email: `Email`
  - zip_code: `ZipCode`
  - street_number: `StreetNumber`
  - street: `StringM`
  - city: `IntGZ`
      ```
      add-address email user@gmail.com zip_code 2C4A street_number 2B street aldo city 1
      ```

- add-card
  - number: `CardNumber`
  - email: `Email`
      ```
      add-card email user@gmail.com number 007865432872
      ```

- add-curtomer
  - name: `StringS`
  - surname: `StringS`
  - email: `Email`
  - phone-number: `PhoneNumber`
      ```
      add-customer name user surname resu email user@gmail.com phone-number +393714393392
      ```

- add-purchase **[order matters]**
  - fare: `RealGEZ`
  - customer: `Email`
  - card: `CardNumber`
  - zip_code: `ZipCode`
  - street: `StringM`
  - street_number: `StreetNumber`
  - product: `ProductCode`
  - quantity: `IntGZ`
  - product: `ProductCode`
  - quantity: `IntGZ`
      ```
      add-purchase fare 0.95 customer user@gmail.com card 007865432872 zip_code 2C4A street aldo street_number 23B product P1 quantity 2  $\dots$ quantity 4 product P2
      ```

- add-rating
  - product: `ProductCode`
  - customer: `Email`
  - stars: `Stars`
      ```
      add-rating product P1 customer user@gmail.com starts 4
      ```

- add-refund-request
  - assigned_delivery: `DeliveryCode`
  - product: `ProductCode`
  - quantity: `IntGZ`
  - product: `ProductCode`
  - quantity: `IntGZ`
      ```
      add-refund-request assigned_delivery 3321AD34 product P1 quantity 2 $\dots$ quantity 4 product P2
      ```

- delete-purchase
  - purchase: `IntGZ`
      ```
      delete-purchase purchase 1
      ```

- search-products
  - product_name: `StringS`
      ```
      search-products product_name a
      ```

- view-order
  - purchase_id: `IntGZ`
      ```
      view-order purchase_id 2
      ```

- view-refund
  - refund_id: `IntGZ`
      ```
      view-refund refund_id 1
      ```

