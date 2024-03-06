import random
import string

RANDOM_REQUESTS = 1

type_requests = {"take-purchase-delivery", "update-assigned-delivery", "update-refund-request", "view-available-purchase", \
                 "add-customer", "add-address", "search-products", "add-card", "add-purchase", "add-rating", "add-refund-request", \
                 "delete-purchase", "view-order", "view-refund", "add-product", "add-restock", "add-supplier", "update-product", "view-statistic"}

def generate_random_type_requests():
    """
    make some random request with random type request
    """
    requests = []
    chars = string.printable

    for _ in range(RANDOM_REQUESTS):
        s = "".join(random.choices(chars, k = random.randint(1, 35)))
        if s not in type_requests:
            requests.append(s)
    return requests