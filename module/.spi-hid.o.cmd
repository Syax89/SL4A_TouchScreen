savedcmd_spi-hid.o := ld.lld -m elf_x86_64 -mllvm -import-instr-limit=5 --mllvm=-enable-fs-discriminator=true --mllvm=-improved-fs-discriminator=true -plugin-opt=thinlto -plugin-opt=-split-machine-functions -z noexecstack   -r -o spi-hid.o @spi-hid.mod  ; /usr/lib/modules/7.1.2-3-cachyos/build/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --mcount --mnop --orc --retpoline --rethunk --sls --static-call --uaccess --prefix=16  --link  --module spi-hid.o

spi-hid.o: $(wildcard /usr/lib/modules/7.1.2-3-cachyos/build/tools/objtool/objtool)
