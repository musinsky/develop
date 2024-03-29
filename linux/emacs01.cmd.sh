#!/usr/bin/env bash

# 2023-09-14

TNAME='emacs01.test'
cat << EOF > "${TNAME}1.c"
#include <stdio.h>
int main() {
 printf("Hello World");
 return 0;
}
EOF
cat << EOF > "${TNAME}1.el"
(setq c-basic-offset 5)
(indent-region (point-min) (point-max) nil)
EOF
cat << EOF > "${TNAME}2.el"
(setq c-basic-offset 10)
(indent-region (point-min) (point-max) nil)
(write-file "${TNAME}2.c")
EOF
cat << EOF > "${TNAME}3.el"
(message (format "arg[0]: %S" (nth 0 command-line-args)))
(message (format "arg[1]: %S" (nth 1 command-line-args)))
(message (format "arg[2]: %S" (nth 2 command-line-args)))
(message (format "arg[3]: %S" (nth 3 command-line-args)))
(message (format "arg[4]: %S" (nth 4 command-line-args)))
(message (format "arg[5]: %S" (nth 5 command-line-args)))
(message (format "arg[6]: %S" (nth 6 command-line-args)))
(message (format "arg[7]: %S" (nth 7 command-line-args)))
(message (format "arg[8]: %S" (nth 8 command-line-args)))
(message (format "arg[9]: %S" (nth 9 command-line-args)))

(message (format "function arguments left before: %S" command-line-args-left))
(let ((num1 (string-to-number (pop command-line-args-left)))
      (num2 (string-to-number (pop command-line-args-left))))
  ;;(setq c-basic-offset num1)
  (message "function numeric arguments: %d and %d" num1 num2))
(message "function string argument:   %S" (pop command-line-args-left))
(message (format "function arguments left after:  %S" command-line-args-left))
(message (format "function arguments all:  %s" command-line-args))

(setq c-basic-offset (string-to-number (nth 4 command-line-args)))
(indent-region (point-min) (point-max) nil)
(write-file "${TNAME}3.c")
EOF

emacs --batch "${TNAME}1.c" -l "${TNAME}1.el" -f save-buffer
emacs --batch "${TNAME}1.c" -l "${TNAME}2.el"
# $ sh emacs01.cmd.sh a1 a2 a3
emacs --batch "${TNAME}1.c" -l "${TNAME}3.el" 20 123 bla_bla "$@"

# https://www.gnu.org/software/emacs/manual/html_node/emacs/Emacs-Invocation.html
# https://www.gnu.org/software/emacs/manual/html_node/emacs/Initial-Options.html
# https://www.gnu.org/software/emacs/manual/html_node/emacs/Command-Example.html
# https://www.gnu.org/software/emacs/manual/html_node/elisp/Batch-Mode.html
# https://www.gnu.org/software/emacs/manual/html_node/elisp/Command_002dLine-Arguments.html

# mucha-clean in one line
# emacs --batch sample.tex -l $HOME/.emacs.el -f mucha-clean -f save-buffer

# --batch
# Run Emacs in batch mode. Batch mode is used for running programs written in
# Emacs Lisp from shell scripts, makefiles, and so on. To invoke a Lisp program,
# use the '-batch' option in conjunction with one or more of '-l', '-f' or
# '--eval' (see Action Arguments). See Command Argument Example, for an example.
#
# Batch mode implies '-q' (do not load an initialization file), but
# site-start.el is loaded nonetheless. Emacs will send messages to stderr.

# -l file, --load=file
# Load the lisp code in the file file.

# -f function, --funcall=function
# Execute the lisp function function.
