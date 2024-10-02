#!/usr/bin/env bash

# 2024-10-02

# https://www.gnu.org/software/bash/manual/bash.html#Quoting
# https://stackoverflow.com/q/6697753
# https://stackoverflow.com/q/31224368

printf 'printf with single quotes\n'
printf '1) \ END\t'        ; printf '1) \tab END\t'        ; printf '1) \x5c END\n'
printf '2) \\ END\t'       ; printf '2) \\tab END\t'       ; printf '2) \\x5c END\n'
printf '3) \\\ END\t'      ; printf '3) \\\tab END\t'      ; printf '3) \\\x5c END\n'
printf '4) \\\\ END\t'     ; printf '4) \\\\tab END\t'     ; printf '4) \\\\x5c END\n'
printf '5) \\\\\ END\t'    ; printf '5) \\\\\tab END\t'    ; printf '5) \\\\\x5c END\n'
printf '6) \\\\\\ END\t'   ; printf '6) \\\\\\tab END\t'   ; printf '6) \\\\\\x5c END\n'
printf '7) \\\\\\\ END\t'  ; printf '7) \\\\\\\tab END\t'  ; printf '7) \\\\\\\x5c END\n'
printf '8) \\\\\\\\ END\t' ; printf '8) \\\\\\\\tab END\t' ; printf '8) \\\\\\\\x5c END\n'
printf '9) \\\\\\\\\ END\t'; printf '9) \\\\\\\\\tab END\t'; printf '9) \\\\\\\\\x5c END\n'

printf "printf with double quotes\n"
printf "1) \ END\t"        ; printf "1) \tab END\t"        ; printf "1) \x5c END\n"
printf "2) \\ END\t"       ; printf "2) \\tab END\t"       ; printf "2) \\x5c END\n"
printf "3) \\\ END\t"      ; printf "3) \\\tab END\t"      ; printf "3) \\\x5c END\n"
printf "4) \\\\ END\t"     ; printf "4) \\\\tab END\t"     ; printf "4) \\\\x5c END\n"
printf "5) \\\\\ END\t"    ; printf "5) \\\\\tab END\t"    ; printf "5) \\\\\x5c END\n"
printf "6) \\\\\\ END\t"   ; printf "6) \\\\\\tab END\t"   ; printf "6) \\\\\\x5c END\n"
printf "7) \\\\\\\ END\t"  ; printf "7) \\\\\\\tab END\t"  ; printf "7) \\\\\\\x5c END\n"
printf "8) \\\\\\\\ END\t" ; printf "8) \\\\\\\\tab END\t" ; printf "8) \\\\\\\\x5c END\n"
printf "9) \\\\\\\\\ END\t"; printf "9) \\\\\\\\\tab END\t"; printf "9) \\\\\\\\\x5c END\n"
printf "\n"

# NOTE: double quotes |printf "%s" $var| and single quotes |printf '%s' $var|
# have the same output (printf with argument)
VAR_SINGLE_1='\tab'    ; VAR_DOUBLE_1="\tab"
VAR_SINGLE_2='\\tab'   ; VAR_DOUBLE_2="\\tab"
VAR_SINGLE_3='\\\tab'  ; VAR_DOUBLE_3="\\\tab"
VAR_SINGLE_4='\\\\tab' ; VAR_DOUBLE_4="\\\\tab"
VAR_SINGLE_5='\\\\\tab'; VAR_DOUBLE_5="\\\\\tab"
# printf 'printf with single quotes and argument VAR declare\n'
# printf 'in single (1st column) and double (2nd column) quotes\n'
# printf '1) %s \t 1) %s \n' "$VAR_SINGLE_1" "$VAR_DOUBLE_1"
# printf '2) %s \t 2) %s \n' "$VAR_SINGLE_2" "$VAR_DOUBLE_2"
# printf '3) %s \t 3) %s \n' "$VAR_SINGLE_3" "$VAR_DOUBLE_3"
# printf '4) %s \t 4) %s \n' "$VAR_SINGLE_4" "$VAR_DOUBLE_4"
# printf '5) %s \t 5) %s \n' "$VAR_SINGLE_5" "$VAR_DOUBLE_5"

printf "printf with double quotes and argument VAR declare\n"
printf "in single (1st column) and double (2nd column) quotes\n"
printf "1) %s \t 1) %s \n" "$VAR_SINGLE_1" "$VAR_DOUBLE_1"
printf "2) %s \t 2) %s \n" "$VAR_SINGLE_2" "$VAR_DOUBLE_2"
printf "3) %s \t 3) %s \n" "$VAR_SINGLE_3" "$VAR_DOUBLE_3"
printf "4) %s \t 4) %s \n" "$VAR_SINGLE_4" "$VAR_DOUBLE_4"
printf "5) %s \t 5) %s \n" "$VAR_SINGLE_5" "$VAR_DOUBLE_5"
printf "\n"

VAR_SINGLE_1='\x5c'    ; VAR_DOUBLE_1="\x5c"
VAR_SINGLE_2='\\x5c'   ; VAR_DOUBLE_2="\\x5c"
VAR_SINGLE_3='\\\x5c'  ; VAR_DOUBLE_3="\\\x5c"
VAR_SINGLE_4='\\\\x5c' ; VAR_DOUBLE_4="\\\\x5c"
VAR_SINGLE_5='\\\\\x5c'; VAR_DOUBLE_5="\\\\\x5c"
# printf 'printf with single quotes and argument VAR declare\n'
# printf 'in single (1st column) and double (2nd column) quotes\n'
# printf '1) %s \t 1) %s \n' "$VAR_SINGLE_1" "$VAR_DOUBLE_1"
# printf '2) %s \t 2) %s \n' "$VAR_SINGLE_2" "$VAR_DOUBLE_2"
# printf '3) %s \t 3) %s \n' "$VAR_SINGLE_3" "$VAR_DOUBLE_3"
# printf '4) %s \t 4) %s \n' "$VAR_SINGLE_4" "$VAR_DOUBLE_4"
# printf '5) %s \t 5) %s \n' "$VAR_SINGLE_5" "$VAR_DOUBLE_5"

printf "printf with double quotes and argument VAR declare\n"
printf "in single (1st column) and double (2nd column) quotes\n"
printf "1) %s \t 1) %s \n" "$VAR_SINGLE_1" "$VAR_DOUBLE_1"
printf "2) %s \t 2) %s \n" "$VAR_SINGLE_2" "$VAR_DOUBLE_2"
printf "3) %s \t 3) %s \n" "$VAR_SINGLE_3" "$VAR_DOUBLE_3"
printf "4) %s \t 4) %s \n" "$VAR_SINGLE_4" "$VAR_DOUBLE_4"
printf "5) %s \t 5) %s \n" "$VAR_SINGLE_5" "$VAR_DOUBLE_5"
printf "\n"

# https://www.shellcheck.net/wiki/SC1003
VAR_SINGLE_1='\'
VAR_SINGLE_2='\\'
VAR_SINGLE_3='\\\'
VAR_SINGLE_4='\\\\'
VAR_SINGLE_5='\\\\\'

# https://www.shellcheck.net/wiki/SC2140
VAR_DOUBLE_1="none1"; VAR_DOUBLE_3="none3"; VAR_DOUBLE_5="none5"
# VAR_DOUBLE_1="\"       # wrong declaration
VAR_DOUBLE_2="\\"
# VAR_DOUBLE_3="\\\"     # wrong declaration
VAR_DOUBLE_4="\\\\"
# VAR_DOUBLE_5="\\\\\"   # wrong declaration
# printf 'printf with single quotes and argument VAR declare\n'
# printf 'in single (1st column) and double (2nd column) quotes\n'
# printf '1) %s \t\t 1) %s \n' "$VAR_SINGLE_1" "$VAR_DOUBLE_1"
# printf '2) %s \t\t 2) %s \n' "$VAR_SINGLE_2" "$VAR_DOUBLE_2"
# printf '3) %s \t\t 3) %s \n' "$VAR_SINGLE_3" "$VAR_DOUBLE_3"
# printf '4) %s \t 4) %s \n' "$VAR_SINGLE_4" "$VAR_DOUBLE_4"
# printf '5) %s \t 5) %s \n' "$VAR_SINGLE_5" "$VAR_DOUBLE_5"

printf "printf with double quotes and argument VAR declare\n"
printf "in single (1st column) and double (2nd column) quotes\n"
printf "1) %s \t\t 1) %s \n" "$VAR_SINGLE_1" "$VAR_DOUBLE_1"
printf "2) %s \t\t 2) %s \n" "$VAR_SINGLE_2" "$VAR_DOUBLE_2"
printf "3) %s \t\t 3) %s \n" "$VAR_SINGLE_3" "$VAR_DOUBLE_3"
printf "4) %s \t 4) %s \n" "$VAR_SINGLE_4" "$VAR_DOUBLE_4"
printf "5) %s \t 5) %s \n" "$VAR_SINGLE_5" "$VAR_DOUBLE_5"
printf "\n"



BS_direct2="\\"; BS_direct4="\\\\" # !!! not "\", "\\\" or "\\\\\" (error) !!!
printf "VAR declare in double quotes, direct2: ${BS_direct2}tab, direct4: ${BS_direct4}tab\n"
BS_direct2='\\'; BS_direct4='\\\\'
printf "VAR declare in single quotes, direct2: ${BS_direct2}tab, direct4: ${BS_direct4}tab\n"
BS_direct2='\x5c'; BS_direct4='\x5c\x5c'   # same effect with single or double quotes
printf "VAR as hex, direct2: ${BS_direct2}tab, direct4: ${BS_direct4}tab\n"

BS_argument_s="\\"   # or '\'   # with %s FORMAT control
BS_argument_b="\x5c"            # with %b FORMAT control
# NOTE: double quotes |printf "%s" $var| and single quotes |printf '%s' $var|
# have the same output with argument
printf "VAR as %%s argument: %s, VAR as %%b argument: %b\n" \
       "${BS_argument_s}tab" "${BS_argument_b}tab"
printf 'VAR as %%s argument: %s, VAR as %%b argument: %b\n' \
       "${BS_argument_s}tab" "${BS_argument_b}tab"
printf "\n"

# SUMMARUM (pre pripad pouzivanie backslash)
# preferovat printf output FORMAT premenych cez ARGUMENTS (nie direct)
# printf '%s' "$var" has the same output as printf "%s" "$var"

# Rozne zapisy toho isteho: '$1n \ 2\pi^{0}$ \\' (LaTeX example)
# TEST: ? is or is not backslash escape sequence ?
# kdekolvek za poslednym '\' napisat napr. 'tab'
# 1st) cez ARGUMENTS (printf '%s' "$var" has the same output as printf "%s" "$var")
# shellcheck disable=SC1003
printf "$%dn %s %d%s$ %s\n" 1 '\'  2 '\pi^{0}' '\\'   # prefer 1
printf "$%dn %s %d%s$ %s\n" 1 "\\" 2 '\pi^{0}' "\\\\" # prefer 2
# 2nd) direct
printf "\$1n \\\ 2\\\pi^{0}$ \\\\\\\\\n" # OK, but serious ?!
# shellcheck disable=SC2016
printf '$1n \\ 2\\pi^{0}$ \\\\\n'        # OK, maybe
printf "\n"

printf "=== backslash and sed ===\n"
# https://www.shellcheck.net/wiki/SC2001 (not in these cases, in real case 'sed -i')
VAR_SINGLE_1='\tab'
VAR_DOUBLE_1="\tab"
sed "s/replace/$VAR_SINGLE_1/" <<< "1)___replace___" # wrong
sed "s/replace/$VAR_DOUBLE_1/" <<< "2)___replace___" # wrong
VAR_SINGLE_1='\\tab'
VAR_DOUBLE_1="\\tab"
sed "s/replace/$VAR_SINGLE_1/" <<< "3)___replace___" # OK
sed "s/replace/$VAR_DOUBLE_1/" <<< "4)___replace___" # wrong
VAR_SINGLE_1='\tab'
VAR_DOUBLE_1="\tab"
# Shell Parameter Expansion ('\' replaced by '\\')
sed "s/replace/${VAR_SINGLE_1//\\/\\\\}/" <<< "5)___replace___" # OK
sed "s/replace/${VAR_DOUBLE_1//\\/\\\\}/" <<< "6)___replace___" # OK

# particles="{}^{3}\!H\!e\, p\,"
# impulse="13.5"
# title="$\mathbf{$particles}$ \textbf{at $impulse GeV\!/c}"
# https://www.gnu.org/software/bash/manual/html_node/Shell-Parameter-Expansion.html
# ${parameter//pattern/string} => all matches of 'pattern' are replaced with 'string'
# ${parameter/pattern/string}  => only the first match is replaced
# printf "title (original): %s\n" "$title"
# printf "title (expanded): %s\n" "${title//\\/\\\\}" # '\' replaced by '\\'
# sed -i "s|ReplaceTableTitle|${title//\\/\\\\}|" "$F_FINAL"

printf "sed delimiters\n"
# sed "s/replace/GeV/c/" <<< "___replace___" # error
sed "s/replace/GeV\/c/" <<< "___replace___" # OK
sed "s|replace|GeV/c|"  <<< "___replace___" # OK
printf "\n"
