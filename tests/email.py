import random
import string
import re

RANDOM_EMAILS = 1
RANDOM_REQUESTS = 1

email_regex = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'

def generate_random_wrong_emails():
    """
    make wrong random emails
    """
    emails = []
    chars = string.ascii_letters + string.digits + string.punctuation

    for _ in range(RANDOM_EMAILS):
        s = "".join(random.choices(chars, k = random.randint(1, 256)))
        if not re.match(email_regex, s):
            emails.append(s)
    return emails


def generate_random_right_emails():
    """
    make right random emails
    """
    emails = []
    chars = string.ascii_letters + string.digits

    for _ in range(RANDOM_EMAILS):
        a = random.randint(1, 200)
        b = random.randint(1, 255 - a -2)
        c = random.randint(2, 255 - b - c)

        s = "".join(random.choices(chars, k = random.randint(1, a))) # username
        s += "@".join(random.choices(chars, k = random.randint(1, b))) # domain
        s += ".".join(random.choices(string.ascii_letters, k = random.randint(1, c))) # ext

        if re.match(email_regex, s):
            emails.append(s)
    return emails