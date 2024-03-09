from .param_interface import ParamInterface
from .alphabet import *

import random
import datetime
import string
import re

start_date = datetime.datetime(2024, 1, 1)
end_date = datetime.datetime.now()

class Timestamp(ParamInterface):
    def __init__(self):
        self.regex = r'\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}'
        pass

    def right(self):
        random_seconds = random.randint(0, int((end_date - start_date).total_seconds()))

        return start_date + datetime.timedelta(seconds=random_seconds)
    
    def wrong(self):
        chars = string.ascii_letters + string.digits + ALLOWED_CHARS
        while True:
            s = "".join(random.choices(chars, k = random.randint(1, 50)))
            if not re.match(self.regex, s):
                return s
