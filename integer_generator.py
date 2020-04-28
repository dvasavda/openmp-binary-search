'''
Generates random integers and places all data into input1.txt

Each integer will be writtin into a new line in input1.txt

'''
import random

nums_to_generate = 200000000
lower_bound = 0
upper_bound = nums_to_generate


output_file = open("input1.txt", "w" )

print("Generating", nums_to_generate, "random integers...")
print("(This may take a while)")

for i in range(nums_to_generate):
    line = str(i)
    output_file.write(line)
    output_file.write('\n')

print("Random numbers generated:", nums_to_generate)
print("Generated numbers range from:", lower_bound, "to", upper_bound)
print("Random number generation completed.\n")

output_file.close()