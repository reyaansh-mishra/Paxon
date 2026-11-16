# Steps

1. Pre-allocate a bunch of RAM
2. Store All we can (right now Memory map)
3. Enable custom malloc on the bunch of RAM
4. fopen() kernel bins  [OPTIONAL FOR NOW]
5. Build TTs
6. ExitBootServices
7. Switch Stack
8. TTBR0_EL1
9. jmp kernel_main() -> This is probably a seperate bin
