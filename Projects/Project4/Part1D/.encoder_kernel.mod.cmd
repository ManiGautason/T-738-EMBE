cmd_/home/mani/Project4/Part1D/encoder_kernel.mod := printf '%s\n'   encoder_kernel.o | awk '!x[$$0]++ { print("/home/mani/Project4/Part1D/"$$0) }' > /home/mani/Project4/Part1D/encoder_kernel.mod
