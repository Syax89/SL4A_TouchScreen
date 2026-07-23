#!/usr/bin/env python3
"""Check the AMD SPI PSP-ownership and deferred-setup safety contract."""

from pathlib import Path


root = Path(__file__).parents[1]
controller = (root / "driver" / "spi-amd.c").read_text()


def body(name: str) -> str:
    start = controller.index(f"static {name}")
    brace = controller.index("{", start)
    depth = 0
    for index in range(brace, len(controller)):
        if controller[index] == "{":
            depth += 1
        elif controller[index] == "}":
            depth -= 1
            if not depth:
                return controller[brace:index + 1]
    raise AssertionError(f"unterminated {name}")


assert "#define AMD_SPI_MISC_CNTRL_REG\t0xFC" in controller
assert "#define AMD_SPI_PSP_OWNS\tBIT(10)" in controller
assert "amd_spi_readreg16(amd_spi, AMD_SPI_MISC_CNTRL_REG) & AMD_SPI_PSP_OWNS" in controller
assert "F_50MHz = 0x4" in controller
assert "F_4MHz = 0x32" in controller
assert "F_3_17MHz = 0x3f" in controller
frequency = body("int amd_set_spi_freq")
assert "amd_spi->speed_hz == 100000000" in frequency
assert "AMD_SPI_SPI100_MASK : 0" not in frequency
assert "if (amd_spi_freq[i].spd7_val)" in frequency

setup = body("int amd_spi_host_setup")
assert "amd_set_spi_freq" not in setup
assert "writereg" not in setup

transfer = body("int amd_spi_host_transfer")
assert transfer.index("amd_spi_check_psp_ownership") < transfer.index("amd_set_spi_freq")
assert transfer.index("amd_set_spi_freq") < transfer.index("amd_spi_select_chip")

segment = body("int amd_spi_exec_segment")
assert segment.index("amd_spi_check_psp_ownership") < segment.index("AMD_SPI_FIFO_CLEAR")

execute = body("int amd_spi_execute_opcode")
assert execute.index("amd_spi_check_psp_ownership") < execute.index("AMD_SPI_CMD_TRIGGER_REG")

print("spi-amd ownership safety contract: PASS")
