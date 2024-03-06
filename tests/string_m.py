import random
import string

RANDOM_STRING_M = 1

def generete_right_string_m():
    """
    make right random emails
    """
    strings = []
    chars = string.printable

    for _ in range(RANDOM_STRING_M):
        s = "".join(random.choices(chars, k = random.randint(1, 100)))
        strings.append(s)
    return strings