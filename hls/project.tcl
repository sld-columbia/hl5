#
# BDW Project File
#

############################################################
# Project Parameters
############################################################
set TECH virtex7
set TECH_PATH "./mem_$TECH"
set VIVADO $::env(XILINX_VIVADO);

#
# Set the memory library
#
use_hls_lib "./memlib"

#
# Setup technology and include behavioral models and/or libraries
#
if {$TECH eq "virtex7"} {
    set_attr verilog_files "$TECH_PATH/*.v"
    set_attr verilog_files "$VIVADO/ids_lite/ISE/verilog/src/glbl.v"
    set_attr verilog_files "$VIVADO/data/verilog/src/retarget/RAMB16_S*.v"
    set_attr fpga_use_dsp off
    set_attr fpga_tool "vivado"
    set_attr fpga_part "xc7v2000tflg1925-2"
    set_attr fpga_part "xc7z020clg484-1"
    set TECH_IS_XILINX 1
    set CLOCK_PERIOD 50.00 ;
    set CLOCK_PERIOD_ASAP 50.00 ;
    set CLOCK_PERIOD_UNDIV2 50.00 ;
    set CLOCK_PERIOD_UNDIV4 60.00 ;
    set CLOCK_PERIOD_MULS2 60.000 ;
    set CLOCK_PERIOD_MULS4 60.000 ;
    set CLOCK_PERIOD_MULS8 60.000 ;
    set CLOCK_PERIOD_MULS16 50.50 ;
    set CLOCK_PERIOD_ADDS16 50.00 ;
    set_attr default_input_delay 0.100 ;
}

#
# Tech-agnostic global synthesis attributes.
#
set_attr message_detail             3
set_attr default_protocol           false
set_attr inline_partial_constants   true
set_attr output_style_reset_all     true
set_attr lsb_trimming               true

## miscellaneous
set_attr output_style_reset_all on
set_attr prints off

# Project paths
set INCLUDES "-I./memlib -I../src/ -I../tb/"
set_attr hls_cc_options "$INCLUDES"

## Simulation Options
#enable_waveform_logging -vcd
set_attr end_of_sim_command "make saySimPassed"
set_attr cc_options         "$INCLUDES -DCLOCK_PERIOD=$CLOCK_PERIOD -g"
#set_systemc_options -version 2.3 -gcc 4.1
use_systemc_simulator incisive


#
# Testbench or System Level Modules
# systemModule
define_system_module main ../tb/sc_main.cpp
define_system_module system ../tb/system.cpp
define_system_module tb ../tb/tb.cpp

#
# SC_MODULEs to be synthesized
#
define_hls_module hl5    ../src/hl5.cpp
define_hls_module fedec     ../src/fedec.cpp
define_hls_module execute   ../src/execute.cpp
define_hls_module memwb     ../src/memwb.cpp

define_io_config * PIN ;# define a PIN configuration for every module


#
# External arrays
#
define_external_array_access -to system -from hl5.fedec -from hl5.memwb


##
# HLS configurations
##

## Base version
define_hls_config hl5    BASIC       --clock_period=$CLOCK_PERIOD
define_hls_config fedec     BASIC       --clock_period=$CLOCK_PERIOD
define_hls_config execute   BASIC       --clock_period=$CLOCK_PERIOD
define_hls_config memwb     BASIC       --clock_period=$CLOCK_PERIOD

## Aggressive scheduling 1 version
define_hls_config hl5    AGGR1    --sched_aggressive_1=on    --clock_period=$CLOCK_PERIOD
define_hls_config fedec     AGGR1    --sched_aggressive_1=on    --clock_period=$CLOCK_PERIOD
define_hls_config execute   AGGR1    --sched_aggressive_1=on    --clock_period=$CLOCK_PERIOD
define_hls_config memwb     AGGR1    --sched_aggressive_1=on    --clock_period=$CLOCK_PERIOD

## Aggressive scheduling 2 version
define_hls_config hl5    AGGR2    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD
define_hls_config fedec     AGGR2    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD
define_hls_config execute   AGGR2    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD
define_hls_config memwb     AGGR2    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD

## Aggressive scheduling 1+2 version
define_hls_config hl5    AGGR_FULL    --sched_aggressive_1=on    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD
define_hls_config fedec     AGGR_FULL    --sched_aggressive_1=on    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD
define_hls_config execute   AGGR_FULL    --sched_aggressive_1=on    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD
define_hls_config memwb     AGGR_FULL    --sched_aggressive_1=on    --sched_aggressive_2=on    --clock_period=$CLOCK_PERIOD

## Schedule ASAP version
define_hls_config hl5    ASAP    --sched_asap=on    --clock_period=$CLOCK_PERIOD_ASAP
define_hls_config fedec     ASAP    --sched_asap=on    --clock_period=$CLOCK_PERIOD_ASAP
define_hls_config execute   ASAP    --sched_asap=on    --clock_period=$CLOCK_PERIOD_ASAP
define_hls_config memwb     ASAP    --sched_asap=on    --clock_period=$CLOCK_PERIOD_ASAP

if {$TECH eq "cmos32soi"} {
## Div unroll 2
define_hls_config hl5    DIV2 -DDIV_UROLL2=1      --clock_period=$CLOCK_PERIOD_UNDIV2
define_hls_config fedec     DIV2 -DDIV_UROLL2=1      --clock_period=$CLOCK_PERIOD_UNDIV2
define_hls_config execute   DIV2 -DDIV_UROLL2=1      --clock_period=$CLOCK_PERIOD_UNDIV2
define_hls_config memwb     DIV2 -DDIV_UROLL2=1      --clock_period=$CLOCK_PERIOD_UNDIV2

## Div unroll 4
define_hls_config hl5    DIV4 -DDIV_UROLL4=1      --clock_period=$CLOCK_PERIOD_UNDIV4
define_hls_config fedec     DIV4 -DDIV_UROLL4=1      --clock_period=$CLOCK_PERIOD_UNDIV4
define_hls_config execute   DIV4 -DDIV_UROLL4=1      --clock_period=$CLOCK_PERIOD_UNDIV4
define_hls_config memwb     DIV4 -DDIV_UROLL4=1      --clock_period=$CLOCK_PERIOD_UNDIV4

## Mul split 2
define_hls_config hl5    MUL2 -DMUL_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config fedec     MUL2 -DMUL_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config execute   MUL2 -DMUL_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config memwb     MUL2 -DMUL_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2

## Mul split 4
define_hls_config hl5    MUL4 -DMUL_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS4
define_hls_config fedec     MUL4 -DMUL_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS4
define_hls_config execute   MUL4 -DMUL_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS4
define_hls_config memwb     MUL4 -DMUL_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS4

## Mul split 8
define_hls_config hl5    MUL8 -DMUL_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS8
define_hls_config fedec     MUL8 -DMUL_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS8
define_hls_config execute   MUL8 -DMUL_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS8
define_hls_config memwb     MUL8 -DMUL_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS8

## Mul split 16
define_hls_config hl5    MUL16 -DMUL_SPLIT16=1      --clock_period=$CLOCK_PERIOD_MULS16
define_hls_config fedec     MUL16 -DMUL_SPLIT16=1      --clock_period=$CLOCK_PERIOD_MULS16
define_hls_config execute   MUL16 -DMUL_SPLIT16=1      --clock_period=$CLOCK_PERIOD_MULS16
define_hls_config memwb     MUL16 -DMUL_SPLIT16=1      --clock_period=$CLOCK_PERIOD_MULS16

## Add split 2
define_hls_config hl5    ADD2 -DADD_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config fedec     ADD2 -DADD_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config execute   ADD2 -DADD_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config memwb     ADD2 -DADD_SPLIT2=1      --clock_period=$CLOCK_PERIOD_MULS2

## Add split 4
define_hls_config hl5    ADD4 -DADD_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config fedec     ADD4 -DADD_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config execute   ADD4 -DADD_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config memwb     ADD4 -DADD_SPLIT4=1      --clock_period=$CLOCK_PERIOD_MULS2

## Add split 8
define_hls_config hl5    ADD8 -DADD_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config fedec     ADD8 -DADD_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config execute   ADD8 -DADD_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS2
define_hls_config memwb     ADD8 -DADD_SPLIT8=1      --clock_period=$CLOCK_PERIOD_MULS2

## Add split 16
define_hls_config hl5    ADD16 -DADD_SPLIT16=1      --clock_period=$CLOCK_PERIOD_ADDS16
define_hls_config fedec     ADD16 -DADD_SPLIT16=1      --clock_period=$CLOCK_PERIOD_ADDS16
define_hls_config execute   ADD16 -DADD_SPLIT16=1      --clock_period=$CLOCK_PERIOD_ADDS16
define_hls_config memwb     ADD16 -DADD_SPLIT16=1      --clock_period=$CLOCK_PERIOD_ADDS16

## Mul+Add split 16
define_hls_config hl5    MA16 -DADD_SPLIT16=1  -DMUL_SPLIT16=1    --clock_period=$CLOCK_PERIOD_ADDS16
define_hls_config fedec     MA16 -DADD_SPLIT16=1  -DMUL_SPLIT16=1    --clock_period=$CLOCK_PERIOD_ADDS16
define_hls_config execute   MA16 -DADD_SPLIT16=1  -DMUL_SPLIT16=1    --clock_period=$CLOCK_PERIOD_ADDS16
define_hls_config memwb     MA16 -DADD_SPLIT16=1  -DMUL_SPLIT16=1    --clock_period=$CLOCK_PERIOD_ADDS16
}

# # Specification of the connection topology for external arrays
# if {$EXT_ARRAYS eq "on"} {
# define_external_array_access -to system -from hl5.fedec -from hl5.memwb
# }

set exe_list [list aes256_10 aes256_1 aes256 basic dhrystone_100 div_10 div_21 fft_1 fft hist_64 matrix_mult mmult_10 mmult_21 pointer_write test]

foreach exename $exe_list {

    set DEFAULT_ARGV "../soft/$exename\.txt BEH report.txt"

    # define simulation configurations
    define_sim_config "BEH_$exename\_P" "hl5 BEH" "fedec BEH" "execute BEH" "memwb BEH" -argv $DEFAULT_ARGV -io_config PIN

    # ADD2 has been removed
    if {$TECH eq "cmos32soi"} {
    set conf_list [list BASIC AGGR1 AGGR2 AGGR_FULL ASAP DIV2 DIV4 MUL2 MUL4 MUL8 ADD4 ADD8 MA16]
    } else {
    set conf_list [list BASIC AGGR1 AGGR2 AGGR_FULL ASAP]
    }

    foreach cname $conf_list {

	set ARGV_LIST "../utils/cflow/$exename\.txt $cname report.txt"

	if {$TECH_IS_XILINX == 1} {
	    define_sim_config "$cname\_$exename\_V" "hl5 RTL_V $cname" "fedec RTL_V $cname" "execute RTL_V $cname" "memwb RTL_V $cname" -argv $ARGV_LIST -io_config PIN -verilog_top_modules glbl
	} else {
	    define_sim_config "$cname\_$exename\_V" "hl5 RTL_V $cname" "fedec RTL_V $cname" "execute RTL_V $cname" "memwb RTL_V $cname" -argv $ARGV_LIST -io_config PIN
	}
    }

}
