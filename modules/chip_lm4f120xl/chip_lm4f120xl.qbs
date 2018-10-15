import qbs

Module{
    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }
    Depends { name: "core_cortex" }
    Depends { name: "core_cortex_fp" }
    Depends { name: "chipGen" }

    // IMPORTANT NOTE:
    // If you intend to use this MCU for anything other than flashing an LED then you WILL have a bad time.
    // Suggest not touching it with a shit stick - It is a giant silicon bug with an on-chip processor:

    // errata rev 1:    http://www.ti.com/lit/pdf/spmz611
    // errata rev 2-5:  who knows.
    // errata rev 6&7:  http://www.ti.com/lit/pdf/spmz856

    property string chip: "none"


    chipGen.memory:  ["flash", "256k", "ram", "32k" ]

    core_cortex.core: "m4" ;
    core_cortex_fp.fp: "hard"

    cpp.defines:  ["TARGET_IS_BLIZZARD_RA1", "BOARD_CHIP_".concat(chip),]
    chipGen.origin:  [
        "flash",    "rx",   "0x00000000",
        "ram" ,     "rwx",  "0x20000000",
    ]

    cpp.includePaths: [
        path+"/include",
        path+"/inc",
        path+"/driverlib",
        path+"/startup",
        path+"/libled",
        path
    ]

    Group {
        name: "TI/Tiva SDK"
        files: [
            "libled/*.c",
            "driverlib/*.c",
            "*/*.h",
        ]
    }

    Group {
        name: "Startup"
        files: [path+"/startup/*.*"]
    }
}
