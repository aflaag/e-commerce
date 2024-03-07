# API DOC

## Customer
- add-address
	- email: Email
	- zip_code: ZipCode
	- street_number: StreetNumber
	- street: StringS
	- city: IntGZ
	> Example: add-address email user@gmail.com zip_code 2C4A street_number 2B street aldo city 1
- add-card
	- number: CardNumber
	- email: Email
	>Example: add-card email user@gmail.com number 007865432872
- add-curtomer
	- name: StringS
	- surname: StringS
	- email: Email
	- phone-number: PhoneNumber
	> Example: add-customer name user surname resu email user@gmail.com phone-number +393714393392
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
	> Example: add-purchase fare 0.95 customer user@gmail.com card 007865432872 zip_code 2C4A street aldo street_number 23B product P1 quantity 2  $\dots$ quantity 4 product P2
	
		The purchase information must be before the acquired product information.
- add-rating
	- product: ProductCode
	- customer: Email
	- stars: Stars
	> Example:  add-rating product P1 customer user@gmail.com starts 4
- add-refund-request
	- assigned_delivery: DeliveryCode
	- product: ProductCode
	- quantity: IntGZ
	$\vdots$
	- product: ProductCode
	- quantity: IntGZ
	> Example: add-refund-request assigned_delivery 3321AD34 product P1 quantity 2 $\dots$ quantity 4 product P2
- delete-purchase
	- purchase: IntGZ
	> Example delete-purchase purchase 1
- search-products
	- product_name: StringS
	> Example: search-products product_name a
- view-order
	- purchase_id: IntGZ
	> Example: view-order purchase_id 2
- view-refund
	- refund_id: IntGZ
	> Example: view-refund refund_id 1
