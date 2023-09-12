:; exec emacs --script "$0" -- "$@"

;; 2023-09-12
;; #####################################
;; :; exec emacs --script "$0" -- "$@"
;; ONLY this shebang prevent Emacs from processing command line
;; arguments using the usual approach of an 'empty' argument '--'.
;; Use '--' argument to separate Emacs options from arguments.
;; $ sh emacs02.cmd.sh -bla1 -bla2
;; #####################################
;; :; exec emacs --script "$0" "$@"
;; this shebang script don't separate Emacs options from arguments
;; $ sh emacs02.cmd.sh bla1 bla2 --version
;; #####################################
;; #!/usr/bin/emacs --script
;; this shebang script must executable
;; $ chmod 744 emacs02.cmd.sh
;; $ ./emacs02.cmd.sh bla1 bla2
;; #####################################
;; #!/usr/bin/env emacs --script
;; this shebang script not working
;; #!/usr/bin/env -S emacs --script
;; this shebang script working (must be executable)
;; #####################################

;; https://www.emacswiki.org/emacs/EmacsScripts
;; https://kitchingroup.cheme.cmu.edu/blog/2014/08/06/Writing-scripts-in-Emacs-lisp/
;; https://stackoverflow.com/q/6238331
;; https://gist.github.com/Kreyren/a12e3618525477c3dd5fb16c3de45bf3

;; https://www.gnu.org/software/emacs/manual/html_node/emacs/Initial-Options.html
;;
;; --script file
;; Run Emacs in batch mode, like '--batch', and then read and execute the Lisp
;; code in file. The normal use of this option is in executable script files
;; that run Emacs. They can start with this text on the first line
;; #!/usr/bin/emacs --script
;; which will invoke Emacs with '--script' and supply the name of the script
;; file as file. Emacs Lisp then treats the '#!' on this first line as a comment
;; delimiter.

;; https://www.gnu.org/software/emacs/manual/html_node/elisp/Command_002dLine-Arguments.html

(message (format "\narg[0]: %S" (nth 0 command-line-args)))
(message (format "arg[1]: %S" (nth 1 command-line-args)))
(message (format "arg[2]: %S" (nth 2 command-line-args)))
(message (format "arg[3]: %S" (nth 3 command-line-args)))
(message (format "arg[4]: %S" (nth 4 command-line-args)))
(message (format "arg[5]: %S" (nth 5 command-line-args)))
(message (format "arg[6]: %S" (nth 6 command-line-args)))
(message (format "arg[7]: %S" (nth 7 command-line-args)))

(defun print-version ()
  (print (version)))

(message (format "\narguments 1: %s" command-line-args))
(message (format "arguments 2: %S" command-line-args-left))
;;(print argv)

(when (member "-scriptload" command-line-args) ; just as an example
  (print-version))

;; Local Variables:
;; mode: emacs-lisp
;; End:
