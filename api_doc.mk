# API DOC

##Supplier
- add-product
	- code: ProductCode
	- name: StringS
	- description: StringL
	- price: RealGEZ
	> Example: request add-product code banana name yoyo description ahahahaha##baba price 341.3243
- add-restock
	- quantity: IntGZ
	- supplier: StringS
	- product: ProductCode
	> Example: request add-restock quantity 10 supplier Supplier1 product P1
- add-supplier
	- business_name: StringS
	> Example: request add-supplier business_name Supplier5
- update-product
	- code: ProductCode
	- description: StringL
	> Example: request update-product code P1 description ho##capito
- view-statistic
	- supplier: StringS
	> Example: request view-statistic supplier Supplier1


## Courier
- take-purchase-delivery
	- courier: StringS
	- purchase: integer
	- deliverycode: DeliveryCode
	> Example: request take-purchase-delivery courier courier1 purchase 1 deliverycode aifjifa
- update-assigned-delivery
	- deliverycode: DeliveryCode
	- lost: timestamp
	> Example: request update-assigned-delivery deliverycode bho2 lost 2024-02-29##19:41:42
- update-refund-request
	- refund: integer
	- refund_end: timestamp
	> Example: request update-refund-request refund 1 refund_end 2024-03-02##11:05:42.660825
- view-available-purchase
	> Example: request view-available-purchase

## Customer
- add-address
	- email: Email
	- zip_code: ZipCode
	- street_number: StreetNumber
	- street: StringS
	- city: IntGZ
	> Example: request add-address email user@gmail.com zip_code 2C4A street_number 2B street aldo city 1
- add-card
	- number: CardNumber
	- email: Email
	>Example: request add-card email user@gmail.com number 007865432872
- add-curtomer
	- name: StringS
	- surname: StringS
	- email: Email
	- phone-number: PhoneNumber
	> Example: request add-customer name user surname resu email user@gmail.com phone-number +393714393392
- add-purchase
	- fare: RealGEZ
	- customer: Email
	- card: CardNumber
	- zip_code: ZipCode
	- street: StringS
	- street_number: StreetNumber
	- product: ProductCode
	- quantity: INTGZ
	$\vdots$
	- product: ProductCode
	-  quantity: INTGZ
	> Example: request add-purchase fare 0.95 customer user@gmail.com card 007865432872 zip_code 2C4A street aldo street_number 23B product P1 quantity 2  $\dots$ quantity 4 product P2
	
		The purchase information must be before the acquired product information.
- add-rating
	- product: ProductCode
	- customer: Email
	- stars: Stars
	> Example: request  add-rating product P1 customer user@gmail.com starts 4
- add-refund-request
	- assigned_delivery: DeliveryCode
	- product: ProductCode
	- quantity: IntGZ
	$\vdots$
	- product: ProductCode
	- quantity: IntGZ
	> Example: request add-refund-request assigned_delivery 3321AD34 product P1 quantity 2 $\dots$ quantity 4 product P2
- delete-purchase
	- purchase: IntGZ
	> Example delete-purchase purchase 1
- search-products
	- product_name: StringS
	> Example: request search-products product_name a
- view-order
	- purchase_id: IntGZ
	> Example: request view-order purchase_id 2
- view-refund
	- refund_id: IntGZ
	> Example: request view-refund refund_id 1
