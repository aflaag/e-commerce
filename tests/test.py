from requests_init import requests, apis, ports

import socket
import random

HOST = "127.0.0.1"  # The server's hostname or IP address
RANDOM_SEED = 7

random.seed(RANDOM_SEED)

#set a seed for random
"""
Test generator
    base structure of a request: Type-of-request key1-arg value1-arg ..... keyn-arg valuen-arg

        Type-of-request should be in an enum/set/list
        key, value are separated by spaces, if exists a space in value it cointain the next key, same but different for key
        spaces in key,value are ##
"""

if __name__ == "__main__":
    counter = [(k, 100) for k in requests.keys()]
    num_requests = sum([v for _, v in counter])
    test = 1
    correct = unknow = wrong = 0
    to_save = []

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

        ta_cpy = true_args
        fa_cpy = false_args

        true_response = True if false_args <= 0 else False

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
                req += " " + str(requests[k][index][arg_index][1]().right()).replace(" ", "##")
                true_args -= 1
            else:
                req += " " + str(requests[k][index][arg_index][1]().wrong()).replace(" ", "##")
                false_args -= 1

            to_insert.pop(pop_i)
        
        handler, port = ports[random.randint(0, 2)]
        
        if k == "search-products" and args == 2 and req.split()[1] == "product_name":
            true_response = True

        response = ""
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, port))
            print(f"TEST[{test}/{num_requests}]")
            print(f"Sending to {handler} a {k} request")

            s.send(req.encode())

            response = s.recv(2048).decode()

        if k not in apis[handler]:
            if not response.startswith("BAD_REQUEST"):
                print(f"WRONG:\n\tExpected: BAD_REQUEST\n\tRecived: {response}")
                wrong += 1
                to_save.append(("1", k, handler, req, response))
            else:
                print("CORRECT")
                correct += 1
        else:
            if true_response:
                print(f"Expected unknow\n\tResponse: {response}")
                unknow += 1
            else:
                if not response.startswith("BAD_REQUEST") and not response.startswith("DB_ERROR"):
                    print(f"WRONG:\n\tExpected: BAD_REQUEST | DB_ERROR\n\tRecived: {response}")
                    wrong += 1
                    to_save.append(("2", k, handler, req, response, ta_cpy, fa_cpy))
                else:
                    print("CORRECT")
                    correct += 1
        print()
        test += 1
    
    print(f"Runned {num_requests} tests:")
    print(f"\tCorrect {correct} / {num_requests} tests")
    print(f"\tUnknow {unknow} / {num_requests} tests")
    print(f"\tWrong {wrong} / {num_requests} tests")

    for elem in to_save:
        print(elem)
