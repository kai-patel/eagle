#!/bin/bash
SCRIPT_DIR=$(dirname "$0")
ctest --test-dir ${SCRIPT_DIR}/../build/ "$@"
