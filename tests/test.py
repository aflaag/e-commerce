import email
import request

RANDOM_REQUESTS = 1


#set a seed for random
"""
Test generator
    base structure of a request: Type-of-request key1-arg value1-arg ..... keyn-arg valuen-arg

        Type-of-request should be in an enum/set/list
        key, value are separated by spaces, if exists a space in value it cointain the next key, same but different for key
        spaces in key,value are ##
"""


random_requests = request.generate_random_type_requests()

available_emails = ['user1@gmail.com', 'user2@gmail.com', 'user3@gmail.com']

random_emails = email.generate_random_emails()


