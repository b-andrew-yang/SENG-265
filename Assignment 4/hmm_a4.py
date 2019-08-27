import random

def main():
    print(generate_states(300))

def generate_states(index):
    # "h" is healthy, "i" is injured
    # Assume start state is always healthy

    state = "h"
    gen_list = list("h")

    for x in range(1, index):
        state = gen_probabilities(state)
        gen_list.append(state)

    return gen_list

def gen_probabilities(state):
    rand_int = random.randint(0,9) + 1

    if state == "h":
        if rand_int > 3:
            state = "h"
        elif rand_int <= 3:
            state = "i"
    elif state == "i":
        if rand_int > 5:
            state = "h"
        elif rand_int <= 5:
            state = 'i'

    return rand_int
