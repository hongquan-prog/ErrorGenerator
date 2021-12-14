#!/bin/bash

../build/error-generator -input-begin="//error code begin" -input-end="//error code end" -o=test.c -output-begin="// generate begin" -output-end="// generate end" test.h -o=test.c