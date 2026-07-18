savedcmd_fch_test.mod := printf '%s\n'   fch_test.o | awk '!x[$$0]++ { print("./"$$0) }' > fch_test.mod
