cmd_/home/mani/Project4/Part2/kernel/enc.mod := printf '%s\n'   enc.o | awk '!x[$$0]++ { print("/home/mani/Project4/Part2/kernel/"$$0) }' > /home/mani/Project4/Part2/kernel/enc.mod
