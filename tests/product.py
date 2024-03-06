import re
import random
import string

RANDOM_PRODUCTS = 1

product_regex = r'^[A-Z0-9]+$'

def generete_wrong_products():
    """
    make wrong random emails
    """
    products = []
    chars = string.ascii_letters + string.digits + string.punctuation

    for _ in range(RANDOM_PRODUCTS):
        s = "".join(random.choices(chars, k = random.randint(1, 30)))
        if not re.match(product_regex, s):
            products.append(s)
    return products

def generete_right_products():
    """
    make right random emails
    """
    products = []
    chars = string.ascii_uppercase + string.digits

    for _ in range(RANDOM_PRODUCTS):
        s = "".join(random.choices(chars, k = random.randint(1, 30)))
        if re.match(product_regex, s):
            products.append(s)
    return products