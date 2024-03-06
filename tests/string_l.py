import random
import string

RANDOM_STRING_L = 1

def generete_right_string_l():
    """
    make right random emails
    """
    strings = []
    chars = string.printable

    for _ in range(RANDOM_STRING_L):
        s = "".join(random.choices(chars, k = random.randint(1, 200)))
        strings.append(s)
    return strings