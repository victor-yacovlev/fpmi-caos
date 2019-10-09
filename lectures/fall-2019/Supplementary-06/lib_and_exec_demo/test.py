#!/usr/bin/python3

import sys
from ctypes import cdll

lib = cdll.LoadLibrary("./"+sys.argv[1])
func = lib["callable_function"]
main = lib["main"]

print("Calling function from lib...")
func();

print("Calling main(10, NULL)...")
ret = main(10, 0)
print("Return value is {}".format(ret))
