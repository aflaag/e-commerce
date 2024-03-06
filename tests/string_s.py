import random
import string

RANDOM_STRING_S = 1

def generete_right_string_s():
    """
    make right random emails
    """
    strings = []
    chars = string.printable

    for _ in range(RANDOM_STRING_S):
        s = "".join(random.choices(chars, k = random.randint(1, 50)))
        strings.append(s)
    return strings