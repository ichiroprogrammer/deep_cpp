MAKEFILE:=$(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))

TARGET:=o/deep_cpp.md
TITLE=Deep C++($(VERSION))

MD_SEC_NUM:=
#MD_SEC_NUM:=--sec_num

MD_GEN:=./md_gen/export/py

# VPATH=./md:dir1:dir2
VPATH=./md:

MDS:=term_intro.md etc.md term_explanation.md

INDEX_OPT:=--exclude $(addsuffix :1,$(MDS) sample_code.md)

include make/md.mk
