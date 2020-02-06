###########################################################################
#
#  Copyright 2017 Columbia University, SLD Group
#
###########################################################################
#
# Makefile - top-level project makefile
#
######################################################################

# list subdirectories that contain memory libraries
# (must be in subdirectories of the current directory)
TECH = virtex7
MEMLIBS = memlib

# list subdirectories that contain interface libraries
# (must be in subdirectories of the current directory)
IFLIBS =

# Uncomment this line to use the Insight debugger
#BDW_DEBUG = 1

saySimPassed:
	@bdw_sim_pass

# Disable timing checks and flush output to screen immediately
BDW_VLOGSIM_ARGS = -NOTIMINGCHECKS -UNBUFFERED

-include Makefile.prj

# Memory wrappers
MEMGEN = ./memgen.py
MEMTECH = ./mem_$(TECH)
MEMGEN_OUT = memlib

memlib: memlist_$(TECH)
	@$(MEMGEN) $(MEMTECH) $< $(MEMGEN_OUT) | tee memgen.log
	@count=`ls -1 memlib/*.bdm 2>/dev/null | wc -l`; \
	if [ $$count != 0 ]; then \
		bdw_memgen ./memlib/*.bdm; \
	fi;

Makefile.prj : memlib project.tcl
	@bdw_makegen

# Clean out undesirable junk files from the project directory
# Uses the automatically created clean_all target from Makefile.prj
clean: clean_all clean_libs
	@rm -f transcript vsim* *.wlf data.out
	@rm -f *~ *.bak *.BAK
	@rm -rf work debussy* vfast*
	@rm -f Makefile.prj
	@rm -f msg_help.html
	@rm -rf core
	@rm -rf core.*
	@rm -rf .stack.*
	@rm -rf *irun.history*
	@rm -rf *.pro *.pro.user
	@rm -rf INCA_libs
	@rm -rf *.log
	@rm -rf *.shm
	@rm -rf *.diag
	@rm -rf *.key
	@rm -rf memlib
	@rm -rf *.csv
	@rm -rf .simvision
	@rm -rf .Xil
	@rm -rf .fail*
	@rm -rf *.bin
	@rm -rf *.txt
	@rm -rf *.err
	@rm -rf *.svcf
	@rm -rf *.ccpp
	@rm -rf .project.tcl.check
	@rm -rf .stratusIDEProjectSettings
	@rm -rf .syn.qws
	@rm -rf .stack.*
	@rm -f transcript vsim* *.wlf data.out
	@rm -f *~ *.bak *.BAK
	@rm -rf work debussy* vfast*

CLEAN: clean


