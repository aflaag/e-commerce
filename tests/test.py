import random
from requests_init import requests, apis, ports
import socket

HOST = "127.0.0.1"  # The server's hostname or IP address

random.seed(7)

#set a seed for random
"""
Test generator
    base structure of a request: Type-of-request key1-arg value1-arg ..... keyn-arg valuen-arg

        Type-of-request should be in an enum/set/list
        key, value are separated by spaces, if exists a space in value it cointain the next key, same but different for key
        spaces in key,value are ##
"""

def send_recv_request(request, port, type):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, port))
        print(f"Sending to {handler} a {type} request")

        s.send(request.encode())

        return s.recv(2048).decode()


def check_response(response, type, handler):
    if type not in apis[handler]:
        if response != "BAD_REQUEST":
            print(f"WRONG:\n\tExpected: INVALID_REQUEST\n\tRecived: {response}")
        else:
            print("CORRECT")
    else:
        if true_response:
            print(f"Expected unknow\n\tResponse: {response}")
        else:
            if response != "BAD_REQUEST" and response != "DB_ERROR":
                print(f"WRONG:\n\tExpected: BAD_REQUEST | DB_ERROR\n\tRecived: {response}")
            else:
                print("CORRECT")


if __name__ == "__main__":
    ajfaj = 0
    counter = [(k, 100) for k in requests.keys()]

    while counter:
        i = random.randint(0, len(counter) -1)
        k, v = counter[i]

        # making a request
        counter[i] = (k, v-1)

        # this is the last request
        if v == 1:
            counter.pop(i)
        
        # adding random arguments
        args = (len(requests[k]) * 2) + random.randint(0, 2) * 2

        true_args =  random.randint(0, min(len(requests[k]), args))
        false_args = args - true_args

        true_response = True if false_args == 0 else False

        req = k
        to_insert = [i for i in range(len(requests[k]))]

        for i in range(args, 0, -2):
            if len(to_insert) == 0 and false_args == 0:
                # the possible arguments are finisched and also the false ones
                break

            if true_args == 0 or len(to_insert) == 0:
                # remain just false args
                req += " boh boh"
                continue

            pop_i = random.randint(0, len(to_insert) -1)
            index = to_insert[pop_i]
            arg_index = random.randint(0, len(requests[k][index]) -1)

            # check for key wrong or right
            if random.randint(1, i) > false_args:
                # put a true one
                req += " " + requests[k][index][arg_index][0]
                true_args -= 1
            else:
                req += " boh"
                false_args -= 1

            # check for value wrong or right
            if random.randint(0, i -1) >= false_args:
                # put a true one
                req += " " + str(requests[k][index][arg_index][1]().right())
                true_args -= 1
            else:
                req += " " + str(requests[k][index][arg_index][1]().wrong())
                false_args -= 1

            to_insert.pop(pop_i)
        
        handler, port = ports[random.randint(0, 2)]

        response = send_recv_request(req, port, k)

        check_response(response, k, handler)