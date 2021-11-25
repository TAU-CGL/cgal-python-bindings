LANGUAGE = en_US
DOCUMENTS = cgal_python_bindings.pdf
TEXFILES := $(sort $(wildcard *.tex))

PDFLATEX = pdflatex
PDFLATEXFLAGS = -halt-on-error -file-line-error
BIBTEX = bibtex
BIBTEXFLAGS =

#GRAPHICS_STATIC = $(foreach g, \
#	prototyp.pdf, \
#	pics/$g)

GRAPHICS_STATIC =

GRAPHICS_PDF =

GRAPHICS_PDFTEX = $(foreach g, \
	, \
	graphics/$g.pdf_tex)

cgal_python_bindings.pdf: \
	../../header.tex \
	../../abrev-short.bib \
	cgal_python_bindings.tex \
	abstract.tex \
	introduction.tex \
	comparison.tex \
	generation.tex \
	modules.tex \
	code.tex \
	../../cgal.bib ../cgal_python_bindings.bib
#	$(GRAPHICS_STATIC) $(GRAPHICS_PDF) $(GRAPHICS_PDFTEX)

.DEFAULT_GOAL = all
.PHONY: all todo spellcheck clean

all: $(DOCUMENTS)

todo:
	for texfile in $(TEXFILES); do \
		echo $$texfile; \
		grep -hnT TODO $$texfile || true; \
		grep -hnTo '\\\(todo\|missingfigure\)\(\[.*\]\)\?{.*}' $$texfile | detex || true; \
	done

spellcheck:
	for texfile in $(TEXFILES); do \
		aspell --lang=$(LANGUAGE) check $$texfile; \
	done

$(DOCUMENTS): %.pdf: %.tex
	$(PDFLATEX) $(PDFLATEXFLAGS) \\input{$<}
	if grep -e '\citation' -e '\bibdata' -e '\bibstyle' *.aux; then \
		$(BIBTEX) $(BIBTEXFLAGS) $* && $(PDFLATEX) $(PDFLATEXFLAGS) \\input{$<}; \
	fi
	$(PDFLATEX) $(PDFLATEXFLAGS) -recorder \\input{$<}
	grep '^OUTPUT ' $*.fls \
		| sed -e 's/^OUTPUT //' \
		| grep -vx $@ \
		| xargs rm
	rm $*.fls
	rm -f $*.bbl $*.blg

%.ps: %.pdf
	pdf2ps -dLanguageLevel=3 $< $@

$(GRAPHICS_PDF): %.pdf: %.svg
	inkscape --without-gui --export-pdf=$*.pdf $*.svg

$(GRAPHICS_PDFTEX): %.pdf_tex: %.svg
	inkscape --without-gui --export-latex --export-pdf=$*.pdf $*.svg

clean:
	rm -f $(DOCUMENTS)
	rm -f *.aux *.bbl *.blg *.log *.out
#	rm -f $(GRAPHICS_PDF)
#	rm -f $(GRAPHICS_PDFTEX)
#	rm -f $(GRAPHICS_PDFTEX:.pdf_tex=.pdf)
