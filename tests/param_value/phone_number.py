from .param_interface import ParamInterface

import re
import random
import string


class PhoneNumber(ParamInterface):
    def __init__(self):
        self.regex = r'^\+?[0-9-]{1,18}[0-9]$'
    
    def right(self):
        chars = string.digits + "-"

        while True:
            phone_number = "+" + "".join(random.choices(chars, k = random.randint(7, 18)))

            if re.match(self.regex, phone_number):
                return phone_number
    
    def wrong(self):
        chars = string.ascii_letters + string.digits + string.punctuation.replace("\'", "")

        while True:
            phone_number = "".join(random.choices(chars, k = random.randint(1, 30)))
            if not re.match(self.regex, phone_number):
                return phone_number
