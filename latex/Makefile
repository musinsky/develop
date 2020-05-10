# 2020-05-10

# this Makefile is example for working with AUCTeX (no multiple targets)
# default target is cite_arabic_roman
#
# $ make NAME=long_table
# $ make NAME=long_table clean

# https://github.com/musinsky/config/blob/master/Emacs/emacs.el
# Emacs with AUCTeX: C-c C-c mucha-make-opt RET
# command: make OPT=" -file-line-error  --synctex=1  -interaction=nonstopmode"
#
# $ make OPT=" -file-line-error  --synctex=1  -interaction=nonstopmode"
# $ make OPT="-synctex=9"

# file $HOME/.latexmkrc
# $pdflatex = 'pdflatex -file-line-error -synctex=1 -interaction=nonstopmode';
#
# $ latexmk -pdf -synctex=9
# pdflatex -file-line-error -synctex=1 -interaction=nonstopmode  -synctex=9
# $ latexmk -pdf -pdflatex="pdflatex -synctex=9"
# pdflatex -synctex=9   # overwrite options from $HOME/.latexmkrc


NAME		= cite_arabic_roman
# $OPT will be override from Emacs with AUCTeX, only with these options
# AUCTeX can forward/inverse search, catching the errors, etc.
# However, these options not are needed for pdflatex (working without AUCTeX)
# "-shell-escape" is not really necessary, only example how add user options
override OPT	+= -shell-escape
# override OPT	:= -shell-escape $(OPT) # append
# override OPT	:= $(OPT) -shell-escape # prepend

all:		$(NAME).pdf

$(NAME).pdf:	$(NAME).tex
		latexmk -pdf -pdflatex="pdflatex $(OPT)" $(NAME)
#		latexmk -pdf $(OPT) $(NAME)

clean:
		latexmk -C $(NAME)
