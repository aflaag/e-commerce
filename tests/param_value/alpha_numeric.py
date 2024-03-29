from .param_interface import ParamInterface
from .alphabet import *


import re
import random
import string

RANDOM_SEED = 7
random.seed(RANDOM_SEED)


class AlphaNumeric(ParamInterface):
    def __init__(self):
        self.regex = r'^[A-Z0-9]{1,50}$'
        pass
    
    def right(self):
        chars = string.ascii_uppercase + string.digits

        while True:
            s ="".join(random.choices(chars, k = random.randint(1, 50)))

            if re.match(self.regex, s):
                return s
    
    def wrong(self):
        chars = string.ascii_letters + string.digits + ALLOWED_CHARS

        while True:
            s = "".join(random.choices(chars, k = random.randint(1, 50)))
            if not re.match(self.regex, s):
                return s
