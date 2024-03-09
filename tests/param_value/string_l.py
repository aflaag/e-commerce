from .param_interface import ParamInterface

import random
import string

class StringL(ParamInterface):
    def __init__(self):
        self.chars = string.ascii_letters + string.digits + string.punctuation.replace("\'", "")

    def right(self):
        return "".join(random.choices(self.chars, k = random.randint(1, 200)))

    def wrong(self):
        return "".join(random.choices(self.chars, k = random.randint(201, 256)))