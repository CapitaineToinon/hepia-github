cmd_/home/antoine/hepia-github/second/os/kernel/hello-4.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /home/antoine/hepia-github/second/os/kernel/hello-4.ko /home/antoine/hepia-github/second/os/kernel/hello-4.o /home/antoine/hepia-github/second/os/kernel/hello-4.mod.o;  true