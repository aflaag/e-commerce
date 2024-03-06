import re
import random
import string

RANDOM_CARDS = 1

card_regex = r'^[0-9]{13,19}$'

def generete_wrong_cards():
    cards = []
    chars = string.printable

    for _ in range(RANDOM_CARDS):
        s = "".join(random.choices(chars, k = random.randint(1, 30)))
        if not re.match(card_regex, s):
            cards.append(s)
    return cards

def generete_right_cards():
    cards = []
    chars = string.digits

    for _ in range(RANDOM_CARDS):
        s = "".join(random.choices(chars, k = random.randint(1, 30)))
        if re.match(card_regex, s):
            cards.append(s)
    return cards