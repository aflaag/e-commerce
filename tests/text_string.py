import random
import string

RANDOM_TEXT_STRING = 1

def generete_right_text():
    """
    make right random emails
    """
    strings = []
    chars = string.printable

    for _ in range(RANDOM_TEXT_STRING):
        s = "".join(random.choices(chars, k = random.randint(201, 250)))
        strings.append(s)
    return strings