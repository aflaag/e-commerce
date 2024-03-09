import string

ALLOWED_CHARS = string.punctuation.translate(str.maketrans("", "", "\'\"%\\`"))
