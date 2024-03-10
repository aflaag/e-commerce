from .param_interface import ParamInterface


import random

RANDOM_SEED = 7
random.seed(RANDOM_SEED)

class IntGZ(ParamInterface):
    def __init__(self):
        pass

    def right(self):
        return random.randint(1, 10000)

    def wrong(self):
        return random.randint(- 10000, 0)