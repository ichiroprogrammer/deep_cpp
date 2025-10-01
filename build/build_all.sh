#!/bin/bash -e

readonly BASE_DIR=$(cd $(dirname $0); pwd)
readonly BASENAME="$(basename $0)"

readonly EXEC_DIR=$1
readonly EXEC_BASENAME=$2
shift 2


readonly MD_GEN=$(cd $EXEC_DIR/md_gen/export/; pwd)

function help(){
    echo "$EXEC_BASENAME [option]"
    echo "    -a    : equal -cef"
    echo "    -c    : clean before build"
    echo "    -d    : dry run"
    echo "    -e    : find files which encoding is not utf-8"
    echo "    -g    : build by g++ only"
    echo "    -S    : not exec make san-ut"
    echo "    -f    : build md/html/pdf/"
    echo "    -j N  : make -j N for example build [default:0]"
    echo "    -x    : set -x"
    echo "    -h    : show this message"

    exit $1
}

CLEAN=false
DRY_RUN=false
CHECK_ENCODING=false
GCC_ONLY=false
NOT_SAN=false
FULL=false

while getopts ":aScdxefgj:h" flag; do
    case $flag in 
    a) CLEAN=true; CHECK_ENCODING=true; FULL=true ;; 
    c) CLEAN=true ;; 
    d) DRY_RUN=true ;; 
    x) set -x ;; 
    e) CHECK_ENCODING=true ;; 
    f) FULL=true ;; 
    g) GCC_ONLY=true ;;
    S) NOT_SAN=true ;;
    j) PARALLEL="$OPTARG" ;; 
    h) help 0 ;; 
    \?) help 1 ;; 
    esac
done

readonly PARA="-j$([ -z "$PARALLEL" ] && : || echo $PARALLEL)"

shift $(expr ${OPTIND} - 1)

echo -n "CLEAN          = " ; $CLEAN && echo true || echo false
echo -n "DRY_RUN        = " ; $DRY_RUN && echo true || echo false
echo -n "CHECK_ENCODING = " ; $CHECK_ENCODING && echo true || echo false
echo -n "FULL           = " ; $FULL && echo true || echo false
echo -n "GCC_ONLY       = " ; $GCC_ONLY && echo true || echo false

set -x

function cmd_launcher()
{
    $DRY_RUN || eval "$@"
}

function build_code() {
    local -r dir=$1

    if [[ -z "$PARALLEL" ]]; then
        local build_opt=
    else 
        local build_opt="-j$PARALLEL"
    fi

    $CLEAN && build_opt="$build_opt -c"
    $DRY_RUN && build_opt="$build_opt -d"
    $GCC_ONLY && build_opt="$build_opt -g"
    $NOT_SAN && build_opt="$build_opt -S"

    $dir/build.sh $build_opt
}

cd ${EXEC_DIR} > /dev/null

$CHECK_ENCODING && cmd_launcher $MD_GEN/sh/find_not_utf8.sh -e ".*\\.png$" -e ".*\\.pdf$"

[ -d example ] && build_code example

[ -d exercise ] && build_code exercise

$CLEAN && cmd_launcher make clean

cmd_launcher make $PARA 

$FULL && cmd_launcher make $PARA html
$FULL && cmd_launcher make $PARA md_pu

exit 0
