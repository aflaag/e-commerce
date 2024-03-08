from .param_interface import ParamInterface

import random
import string

class StringS(ParamInterface):
    def __init__(self):
        self.chars = string.ascii_letters + string.digits + string.punctuation
        pass

    def right(self):
        return "".join(random.choices(self.chars, k = random.randint(1, 50)))

    def wrong(self):
        return "".join(random.choices(self.chars, k = random.randint(51, 256)))