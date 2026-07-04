savedcmd_spi-hid.mod := printf '%s\n'   spi-hid-core.o trace.o | awk '!x[$$0]++ { print("./"$$0) }' > spi-hid.mod
