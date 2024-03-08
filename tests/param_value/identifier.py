from .param_interface import ParamInterface

import random

class Identifier(ParamInterface):
    def __init__(self):
        pass

    def right(self):
        return random.randint(1, 100)

    def wrong(self):
        return random.randint(- 100, 0)