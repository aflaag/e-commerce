from .param_interface import ParamInterface

import random

class Stars(ParamInterface):
    def __init__(self):
        pass

    def right(self):
        return random.randint(1, 6)

    def wrong(self):
        return random.randint(-100 ,1) if random.random() > 0.5 else random.randint(6, 100)