cmd_/home/mani/Project4/Part1D/Module.symvers :=  sed 's/ko$$/o/'  /home/mani/Project4/Part1D/modules.order | scripts/mod/modpost -m -a    -o /home/mani/Project4/Part1D/Module.symvers -e -i Module.symvers -T - 