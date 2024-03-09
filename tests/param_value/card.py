from .param_interface import ParamInterface

import re
import random
import string

class Card(ParamInterface):

    def __init__(self):
        self.regex = r'^[0-9]{13,19}$'

    def right(self):
        chars = string.digits

        while True:
            card = "".join(random.choices(chars, k = random.randint(13, 19)))
            if re.match(self.regex, card):
                return card
    
    def wrong(self):
        chars = string.ascii_letters + string.digits + string.punctuation.replace("\'", "")

        while True:
            card = "".join(random.choices(chars, k = random.randint(1, 30)))
            if not re.match(self.regex, card):
                return card
