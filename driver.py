#!/usr//bin/python3
#
# driver.py - The driver tests the correctness
import subprocess
import re
import os
import sys
import argparse
import shutil
import json


def is_tool(name):
    """Check whether `name` is on PATH and marked as executable."""

    # from whichcraft import which
    from shutil import which

    return which(name) is not None


# Basic tests
# The points are per test points. The number of tests that passed or failed directly from C test-suite
#   "arraylist": {
#       "timeout 60 ./Arraylist/arraylist.bin --verbose": 5
#       },

tests_json = """{
  "strcmp": {
      "timeout 60 ./word-count/test_strcmp.bin --verbose": 5
      }
    }
"""

difftests_json = """{
    "tokenize": {
    "timeout 2 ./word-count/tokenize.bin ./word-count/txt/input.txt > ./word-count/out/input.txt.tokenize; python3 compare.py  ./word-count/out/input.txt.tokenize ./word-count/reference/input.txt.tokenize": 20
    },
    "dictionary": {
    "timeout 2 ./word-count/dictionary.bin ./word-count/txt/input.txt > ./word-count/out/input.txt.dictionary; python3 compare.py  ./word-count/out/input.txt.dictionary ./word-count/reference/input.txt.dictionary": 20
    },
    "poscount": {
    "timeout 2 ./word-count/poscount.bin ./word-count/txt/input.txt > ./word-count/out/input.txt.poscount; python3 compare.py  ./word-count/out/input.txt.poscount ./word-count/reference/input.txt.poscount": 20
    },
    "mapcount": {
    "timeout 2 ./word-count/mapcount.bin  ./word-count/txt/input.txt ./word-count/txt/small.txt > ./word-count/out/input.small.mapcount; python3 compare.py ./word-count/out/input.small.mapcount ./word-count/reference/input.small.mapcount": 40
    }
}"""

valgrindtests_json = """{
    "valgrindtokenize": {
    "timeout 240 valgrind --leak-check=full ./word-count/tokenize.bin ./word-count/txt/input.txt": 5
    },
    "valgrinddictionary": {
    "timeout 240 valgrind --leak-check=full ./word-count/dictionary.bin ./word-count/txt/input.txt": 5
    },
    "valgrindposcount": {
    "timeout 240 valgrind --leak-check=full ./word-count/poscount.bin ./word-count/txt/input.txt": 5
    },
    "valgrindmapcount": {
    "timeout 240 valgrind --leak-check=full ./word-count/mapcount.bin  ./word-count/txt/input.txt ./word-count/txt/small.txt": 5
    }

}"""


# "ciphmsg": {
#     "cd Cipher; ./cipher.bin encrypt --verbose": 10
# },
# "ciphfile": {
#     "cd Cipher; ./cipher.bin encrypt-files --verbose": 10
# },
# "decrypt": {
#     "cd Cipher; ./cipher.bin key --verbose": 10
# }


Final = {}
Error = ""
Success = ""
PassOrFail = 0
DiffTestResults = {}  # Track diff test results for valgrind dependency
#
# main - Main function
#


def runtests(test, name):
    total = 0
    points = 0
    global Success
    global Final
    global Error
    global PassOrFail
    for steps in test.keys():
        print(steps)
        p = subprocess.Popen(
            steps, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout_data, stderr_data = p.communicate()
        total = total + test[steps]
        out_lines = stdout_data.splitlines()
        success_cases = 0
        failed_cases = 0
        count_tests = 0
        for lines in out_lines:
            if b"Count of run unit tests" in lines:
                num_tests = re.findall(r'\d+', lines.decode('utf-8'))
                points += int(num_tests[0])*test[steps]
            if b"Count of failed unit tests" in lines:
                num_tests = re.findall(r'\d+', lines.decode('utf-8'))
                points -= int(num_tests[0])*test[steps]
            if b"Count of all unit tests" in lines:
                count_tests = re.findall(r'\d+', lines.decode('utf-8'))
        points = points//int(count_tests[0])
        if(p.returncode != 0):
            Error += "#### " + "*"*5+steps+"*"*5
            Error += "\n ``` \n" + stdout_data.decode(errors="ignore")
            Error += "\n```\n"
            PassOrFail = p.returncode
        else:
            Success += "#### " + "*"*5+steps+"*"*5
            Success += "\n ```" + \
                stdout_data.decode(errors="ignore") + "\n```\n"
        print(total, points)
        if points < total:
            Final[name.lower()] = {"mark": points,
                                   "comment": "Program exited with return code"+str(p.returncode)}
        else:
            Final[name.lower()] = {"mark": points,
                                   "comment": "Program ran and output matched."}


def rundifftests(test, name):
    total = 0
    points = 0
    global Success
    global Final
    global Error
    global PassOrFail
    global DiffTestResults
    
    for steps in test.keys():
        print(steps)
        p = subprocess.Popen(
            steps, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout_data, stderr_data = p.communicate()
        total = total + test[steps]
        out_lines = stdout_data.splitlines()
        success_cases = 0
        failed_cases = 0
        test_passed = False
        
        if(p.returncode != 0):
            Error += "#### " + "*"*5+steps+"*"*5
            Error += "\n ```" + stdout_data.decode(errors="ignore")
            Error += "\n```\n"
            points = 0
            PassOrFail = p.returncode
            test_passed = False
        else:
            points = total
            Success += "#### " + "*"*5+steps+"*"*5
            Success += "\n ```" + \
                stdout_data.decode(errors="ignore") + "\n```\n"
            test_passed = True

        # Store diff test result for valgrind dependency check
        DiffTestResults[steps] = test_passed

#        p = subprocess.Popen("egrep \"strcmp|strlen|isalnum|strdup|strcat|strncpy|strncmp\" word-count/*.c",
 #                            shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p = subprocess.Popen("egrep -n \"strcmp|strlen|isalnum|isdigit|isalpha|strdup|strcat|strncpy|strncmp|strtok\" word-count/vector_string.c word-count/dictionary.c word-count/tokenize.c word-count/linecount.c",
                             shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.wait()
        return_grep = 0
        return_grep = (p.returncode != 0)
        p = subprocess.Popen("tail -n +22 word-count/table_string.c | egrep -n \"strcmp|strlen|isalnum|strdup|strcat|strncpy|strncmp|strtok\"",
                             shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.wait()
        return_grep = return_grep & (p.returncode != 0)
        PassOrFail |= (return_grep == 0)
        if points < total:
            Final[name.lower()] = {"mark": points,
                                   "comment": "Program exited with return code. Either your output does not match the reference output. Check the log to see if you program has any errors. Also see word-count/out/input.txt and compare against your output. Ensure you are using the prints match exactly as specified. Check hints section on assignment page." + name.lower()+str(p.returncode)}
        else:
            if (return_grep != 0):
                Final[name.lower()] = {"mark": points,
                                       "comment": "Program ran and output matched."}
            else:
                Final[name.lower()] = {"mark": 0,
                                       "comment": "You have used a banned string function in your code. You have been zeroed out for all of word-count. Remove the offending string and try again. See hints page on how to search for offending string"}


def runvalgrindtests(test, name):
    total = 0
    points = 0
    global Success
    global Final
    global Error
    global PassOrFail
    global DiffTestResults

    if not is_tool("valgrind"):
        print("Valgrind not found; Not grading valgrind")
        return
        
    # Mapping of valgrind tests to their corresponding diff tests
    valgrind_to_diff_mapping = {
        "valgrindtokenize": "tokenize",
        "valgrinddictionary": "dictionary", 
        "valgrindposcount": "poscount",
        "valgrindmapcount": "mapcount"
    }
    
    for steps in test.keys():
        print(steps)
        
        # Check if corresponding diff test passed
        diff_test_key = valgrind_to_diff_mapping.get(steps)
        if diff_test_key and diff_test_key in DiffTestResults:
            if not DiffTestResults[diff_test_key]:
                # Corresponding diff test failed, fail this valgrind test
                Error += f"#### *****{steps}*****\n"
                Error += "```\nValgrind test skipped because corresponding diff test failed.\n```\n"
                Final[name.lower()] = {
                    "mark": 0,
                    "comment": f"Valgrind test failed because corresponding diff test ({diff_test_key}) failed."
                }
                PassOrFail = 1
                continue
        
        p = subprocess.Popen(
            steps, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout_data, stderr_data = p.communicate()
        total = total + test[steps]
        out_lines = stderr_data.splitlines()
        success_cases = 0
        failed_cases = 0
        returncode = 0

        for lines in out_lines:
            if b"All heap blocks were freed -- no leaks are possible" in lines:
                returncode = 0
                break
            else:
                returncode = 1
        PassOrFail |= (returncode != 0)
        if(returncode != 0):
            Error += "#### " + "*"*5+steps+"*"*5
            Error += "\n ```" + stderr_data.decode(errors="ignore")
            Error += "\n```\n"
        else:
            points = total
            Success += "#### " + "*"*5+steps+"*"*5
            Success += "\n ```" + \
                stderr_data.decode(errors="ignore") + "\n```\n"

        if points < total:
            if (points < 0):
                points = 0
            Final[name.lower()] = {"mark": points,
                                   "comment": "Valgrind found a leak. Check assignment page on how to run valgrind and check for leaks"}
        else:
            Final[name.lower()] = {"mark": points,
                                   "comment": "valgrind ran and found no leaks."}


def main():
        # Parse the command line arguments

    # Basic Tests
    test_dict = json.loads(tests_json)
    for parts in test_dict.keys():
        runtests(test_dict[parts], parts)

    test_dict = json.loads(difftests_json)
    for parts in test_dict.keys():
        rundifftests(test_dict[parts], parts)

    test_dict = json.loads(valgrindtests_json)
    for parts in test_dict.keys():
        runvalgrindtests(test_dict[parts], parts)

    githubprefix = os.path.basename(os.getcwd())
    Final["userid"] = "GithubID:" + githubprefix
    j = json.dumps(Final, indent=2)

    with open(githubprefix + "_Grade"+".json", "w+") as text_file:
        text_file.write(j)

    with open("LOG.md", "w+") as text_file:
        text_file.write("## " + '*'*20 + 'FAILED' + '*'*20 + '\n' + Error)
        text_file.write("\n" + "*" * 40)
        text_file.write("\n## " + '*'*20 + 'SUCCESS' + '*'*20 + '\n' + Success)

    sys.exit(PassOrFail)


    # execute main only if called as a script
if __name__ == "__main__":
    main()
