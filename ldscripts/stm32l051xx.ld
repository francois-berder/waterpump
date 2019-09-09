MEMORY
{
    flash       :   ORIGIN = 0x08000000, LENGTH = 32K
    eeprom      :   ORIGIN = 0x08080000, LENGTH = 2K
    sram        :   ORIGIN = 0x20000000, LENGTH = 8K
}

/* Include main link script. Note: it will be searched in -L paths. */
INCLUDE cortex.ld
