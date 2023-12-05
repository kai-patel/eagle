#!/bin/bash
SCRIPT_DIR=$(dirname "$0")
cmake -S ${SCRIPT_DIR}/.. -B ${SCRIPT_DIR}/../build
