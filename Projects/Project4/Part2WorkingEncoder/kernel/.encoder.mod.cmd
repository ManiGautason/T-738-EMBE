cmd_/home/mani/Project4/Part2/kernel/encoder.mod := printf '%s\n'   encoder.o | awk '!x[$$0]++ { print("/home/mani/Project4/Part2/kernel/"$$0) }' > /home/mani/Project4/Part2/kernel/encoder.mod
