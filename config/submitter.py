#!/usr/bin/python
import os
import sys

c_test = ''
c_send = 'sframe_batch.py '
option = sys.argv[len(sys.argv)-1]+' '

if 'non ' in option:
    option = ' '
elif option not in ['-f ', '-s ', '-r ']:
    sys.exit("How to use: ./submitter.py <XML files> <true/false (test output or not)> <option ('-s', '-f', '-r', 'non')>")


if 'true' in sys.argv:
    c_test = 'echo '
elif 'True' in sys.argv:
    c_test = 'echo '
elif 'false' in sys.argv:
    c_test = ''
elif 'False' in sys.argv:
    c_test = ''
else:
    sys.exit("How to use: ./submitter.py <XML files> <true/false (test output or not)> <option ('-s', '-f', '-r', 'non')>")


# if sys.argv[len(sys.argv)-2] == 'true':
#     c_test = 'echo '

file_list = []

for file in sys.argv[1:]:
    if '.xml' in file:
        file_list.append(file)
        os.system('echo submitting: %s ' %file)

os.system('echo \n')
for i in file_list:
    os.system(c_test+c_send+option+i)
