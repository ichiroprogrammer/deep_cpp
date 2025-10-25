MAKEFILE:=$(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))

TARGET:=o/deep_cpp.md
TITLE=Deep C++($(VERSION))

MD_SEC_NUM:=
#MD_SEC_NUM:=--sec_num

MD_GEN:=./md_gen/export/py

# VPATH=./md:dir1:dir2
VPATH=./md:

MDS:=deep_intro.md solid.md design_pattern.md template_meta_programming.md \
     dynamic_memory_allocation.md cpp_idiom.md cpp_standard.md cpp_semantics.md

INDEX_OPT:=--exclude $(addsuffix :1,$(MDS) sample_code.md)

include make/md.mk
