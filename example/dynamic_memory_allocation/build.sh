#!/bin/bash -e

readonly BASE_DIR=$(cd $(dirname $0); pwd)
readonly BASENAME="$(basename $0)"

export SAN_BUILD=false
$BASE_DIR/../../build/build_core.sh $BASE_DIR $@
