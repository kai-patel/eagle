#!/bin/bash
SCRIPT_DIR=$(dirname "$0")
"${SCRIPT_DIR}/configure.sh" && "${SCRIPT_DIR}/compile.sh"
