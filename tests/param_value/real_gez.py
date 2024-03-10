from .param_interface import ParamInterface


import random

RANDOM_SEED = 7
random.seed(RANDOM_SEED)

class RealGEZ(ParamInterface):
    def __init__(self):
        pass

    def right(self):
        return random.uniform(0, float("inf"))

    def wrong(self):
        return random.uniform(- float("inf"), 0)