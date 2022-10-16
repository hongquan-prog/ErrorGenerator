#!/bin/bash

../build/error-generator -i="test.h" -o="test.c" -input-begin="//error-code-begin" -input-end="//error-code-end" -output-begin="//generate-begin" -output-end="//generate-end"