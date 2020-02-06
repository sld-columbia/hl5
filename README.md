# HL5: A 32-bit RISC-V processor designed with HLS

This is a high-level description in SystemC of an in-order 32-bit RISC-V core.

## Installing the RISC-V toolchain

Test programs must be compiled with the RISC-V toolchain.

```bash
git clone --recursive https://github.com/riscv/riscv-gnu-toolchain.git
cd riscv-gnu-toolchain
mkdir build
cd build
../configure --prefix=<custom_target_path> --with-arch=rv32ima --with-abi=ilp32
make
```

To compile the test programs, make sure that `riscv32-unknown-elf-gcc` is PATH.

```bash
cd <hl5>/soft
make
```

## Simulating and Synthesizing HL5

Simulation and synthesis run within the _Cadence Stratus HLS 17.2_ environment.
In addition to Stratus HLS, simulation is configured to use _Incisive_ as the
RTL and SystemC simulator.  You may change this configuration from the Stratus
`project.tcl` script.

For instance, to run HLS and generate RTL for the `BASIC` HLS configuration,
enter the HLS folder and run the following target.

```bash
cd <hl5>/hls
make hls_BASIC
```

To run a simulation of the C program `soft/aes256_1.c`, compile the test
programs, then execute the following targets.

```bash
cd <hl5>/hls
# Behavioral simulation
make sim_BEH_aes256_1_P
# RTL simulation for BASIC HLS configuration
make sim_BASIC_aes256_1_V
# RTL simulation with GUI support
make debug_BASIC_aes256_1_V
```

The simulation will print on screen the content of the program counter, the
instruction register and the register file. Colors highlight stalls (in red),
registers for which there is a pending write back (yellow) and registers that
have been updated during the latest instruction commit.

The target for synthesis is set to the Xilinx Artix7 FPGA, which is available on
the ZYNQ SoCs.

If you use this work, please reference the following paper.

> _*[HL5: A 32-bit RISC-V Processor Designed with High-Level
> Synthesis](https://www.sld.cs.columbia.edu/pubs/mantovani_cicc20.pdf).*
> Paolo Mantovani, Robert Margelli, Davide Giri, and Luca P. Carloni.
> In the Proceedings of the Custom Integrated Circuits Conference (CICC), 2020._
