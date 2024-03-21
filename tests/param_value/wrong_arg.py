from .param_interface import ParamInterface
from .alphabet import *


import random
import string

RANDOM_SEED = 7
random.seed(RANDOM_SEED)

class WrongArg(ParamInterface):
    def __init__(self):
        self.chars = string.ascii_letters + string.digits + ALLOWED_CHARS
        pass

    def right(self):
        return "".join(random.choices(self.chars, k = random.randint(3, 5)))