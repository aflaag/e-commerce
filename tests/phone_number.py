import re
import random
import string

RANDOM_PHONE_NUMBER = 1

product_regex = r'^\+?[0-9-]{1,18}[0-9]$'

def generete_wrong_phone_number():
    phone_number = []
    chars = string.ascii_letters + string.digits + string.punctuation

    for _ in range(RANDOM_PHONE_NUMBER):
        s = "".join(random.choices(chars, k = random.randint(1, 30)))
        if not re.match(product_regex, s):
            phone_number.append(s)
    return phone_number

def generete_right_phone_number():
    phone_number = []
    chars = string.digits + "-"

    for _ in range(RANDOM_PHONE_NUMBER):
        s = "+".join(random.choices(chars, k = random.randint(1, 30)))
        if re.match(product_regex, s):
            phone_number.append(s)
    return phone_number