from .param_interface import ParamInterface
from .alphabet import *


import random
import string
import re

RANDOM_SEED = 7
random.seed(RANDOM_SEED)


class Email(ParamInterface):
    
    def __init__(self):
        self.regex = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'

    def right(self):
        chars = string.ascii_letters + string.digits

        while True:
            a = random.randint(1, 200)
            b = random.randint(1, 255 - a -2)
            c = random.randint(2, min(255 - b - a, 10))

            param = "".join(random.choices(chars, k = random.randint(1, a))) # username
            param += "@" + "".join(random.choices(chars, k = random.randint(1, b))) # domain
            param += "." + "".join(random.choices(string.ascii_letters, k = random.randint(2, c))) # ext

            if re.match(self.regex, param):
                return param
    
    def wrong(self):
        chars = string.ascii_letters + string.digits + ALLOWED_CHARS

        while True:
            param = "".join(random.choices(chars, k = random.randint(1, 256)))
            if not re.match(self.regex, param):
                return param
