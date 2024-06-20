#import TARGET
MD_SC:=sample_code.md

MDS_DEPS:=$(addprefix o/c/,$(MDS:.md=.d))

MDS_C:=$(addprefix o/c/,$(MDS) $(MD_SC))
MDS_DB:=o/db.json

MDS_FINAL:=$(MD_IND) $(addprefix o/,$(notdir $(MDS_C)))
MDS_PU:=$(addprefix o/pu/,$(notdir $(MDS_FINAL)))
HTMLS:=$(MDS_FINAL:.md=.html)

TARGET_HTML:=$(TARGET:.md=.html)
TARGET_PDF:=$(TARGET:.md=.pdf)
TARGET_PU:=$(addprefix o/pu/,$(notdir $(TARGET)))


.PHONY: md html pdf all
md: $(TARGET) $(MDS_FINAL)
html: $(TARGET_HTML) $(HTMLS)
pdf: html $(TARGET_PDF)
all: md html pdf

.PHONY: mds_pu
mds_pu: $(MDS_PU) $(TARGET_PU)

CLEANS:=$(patsubst clean%,clean, $(MAKECMDGOALS))
ifneq ($(CLEANS), clean)
include $(MDS_DEPS)
endif

VERSION_TXT=version.txt
VERSION=$(shell cat $(VERSION_TXT))
AUTHOR=$(shell git config --local user.name)

MD_GEN ?=

ifeq ($(MD_GEN),)
MD_GEN:=../md_gen/export/py
endif

$(TARGET): $(MDS_FINAL)
	$(MD_GEN)/md_join.py -o $@ $^

$(TARGET_PDF) : $(TARGET_HTML)
	$(MD_GEN)/../sh/to_pdf.sh $<

$(MD_IND) : $(MDS_DB)
	$(MD_GEN)/md_make_index.py $(MD_SEC_NUM) -o $@ $< $(INDEX_OPT)

md/$(MD_SC) : $(MAKEFILE) $(MDS)
	$(MD_GEN)/md_sample_section.py -p $(VPATH) -o md/$(MD_SC) $(MDS)

o/c/$(MD_SC): $(MDS) md/$(MD_SC)
	$(MD_GEN)/md_compile.py -o $@ md/$(MD_SC) -p $(VPATH) --mds $(MDS) $(MD_SC)

o/c/%.md: %.md
	$(MD_GEN)/md_compile.py -o $@ $< -p $(VPATH) --mds $(MDS)

o/c/%.d : %.md
	$(MD_GEN)/md_compile.py -D $(@:.d=.md) -o $@ $<

o/pu/%.md : o/%.md
	$(MD_GEN)/md_inject_pu.py $< -o $@

$(MDS_DB) : $(MDS_C)
	$(MD_GEN)/md_make_db.py $@ --mds $^

o/%.md : o/c/%.md $(MDS_DB)
	$(MD_GEN)/md_link.py $(MD_SEC_NUM) -o $@ --db $(MDS_DB) $<

o/%.html: o/%.md $(VERSION_TXT)
	[ -n "$(AUTHOR)" ]
	$(MD_GEN)/md_to_html.py --author "$(AUTHOR)" --title "$(TITLE)" -o $@ $<

%.png: %.pu
	$(MD_GEN)/plant_uml_encode.py $< -o $@
	$(MD_GEN)/png_checker.py $@ || (rm $@; exit 1)

.PHONY: clean help
clean:
	rm -f $(MDS_DEPS)
	rm -f $(MD_SC) $(MD_IND) $(MDS_DB)
	rm -f $(MDS_FINAL) $(MDS_C)
	rm -f $(HTMLS)
	rm -f $(TARGET) $(TARGET_HTML) $(TARGET_PDF)

help:
	@echo "make                     :generate $(TARGET)"
	@echo "make md MD_SEC_NUM=      :generate $(TARGET) without section number"
	@echo "make html                :generate $(TARGET_HTML)"
	@echo "make pdf                 :generate $(TARGET_PDF)"
	@echo "make mds_pu              :generate $(TARGET_PU)"
	@echo "make clean               :generated files are deleted"
	@echo "make help                :show this message"

echo:
	-echo MDS_FINAL=$(MDS_FINAL)
	-echo MDS_PU=$(MDS_PU)
	-echo MDS_C=$(MDS_C)
	-echo MDS_DEPS=$(MDS_DEPS)

