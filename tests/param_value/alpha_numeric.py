from .param_interface import ParamInterface

import re
import random
import string


class AlphaNumeric(ParamInterface):
    def __init__(self):
        self.regex = r'^[A-Z0-9]{1,50}$'
    
    def right(self):
        chars = string.ascii_uppercase + string.digits

        while True:
            s ="".join(random.choices(chars, k = random.randint(1, 50)))

            if re.match(self.regex, s):
                return s
    
    def wrong(self):
        chars = string.ascii_letters + string.digits + string.punctuation.replace("\'", "")

        while True:
            s = "".join(random.choices(chars, k = random.randint(1, 50)))
            if not re.match(self.regex, s):
                return s
