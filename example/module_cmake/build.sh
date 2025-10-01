#!/bin/bash

readonly BASE_DIR=$(cd $(dirname $0); pwd)
readonly BASENAME="$(basename $0)"
readonly MD_GEN=$(cd $BASE_DIR/../../md_gen/export/; pwd)


function help(){
    echo "$BASENAME  [option]"
    echo "    -a    : equal -ce"
    echo "    -c    : clean before build"
    echo "    -C    : clean libgtest.a if with -c"
    echo "    -d    : dry run"
    echo "    -e    : find files which encoding is not utf-8"
    echo "    -g    : build by g++ only"
    echo "    -i    : execute IT"
    echo "    -j N  : make -j N for sample build [default:0]"
    echo "    -h    : show this message"

    exit $1
}

function cmd_launcher()
{
    $DRY_RUN || eval "$@"
}

function find_not_utf8() {
    pushd $BASE_DIR > /dev/null

    cmd_launcher $MD_GEN/sh/find_not_utf8.sh

    popd > /dev/null
}

function build(){
    local dir="$1"
    mkdir $dir
    cmake -B $dir
    make -C $dir
}

function format() {
    clang-format -i *.cpp *.cppm
}
function exec() {
    local dir="$1"
    
    $dir/main
}

CLEAN=false
CLEAN_GTEST=false
IT=false
DRY_RUN=false
CHECK_ENCODING=false

while getopts ":aSCcdeghij:" flag; do
    case $flag in 
    a) CLEAN=true; CHECK_ENCODING=true ;;
    c) CLEAN=true ;; 
    C) CLEAN_GTEST=true ;; 
    d) DRY_RUN=true ; set -x ;; 
    e) CHECK_ENCODING=true ;; 
    g) CLANG_BUILD=true; SCAN_BUILD=false; SAN_BUILD=false ;;
    i) IT=true ;; 
    j) PARALLEL="$OPTARG" ;; 
    h) help 0 ;; 
    \?) help 1 ;; 
    esac
done

cd $BASE_DIR

$CHECK_ENCODING && find_not_utf8
$CLEAN && cmd_launcher "rm -rf build"
cmd_launcher format
cmd_launcher build "build"
cmd_launcher exec "build"

