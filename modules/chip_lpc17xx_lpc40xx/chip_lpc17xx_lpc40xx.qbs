import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }
    Depends { name: "core_cortex" }
    Depends { name: "core_cortex_fp" }

    property string chip: "none"

    Properties {
        condition: chip_lpc17xx_lpc40xx.chip.contains("LPC408") ||
                   chip_lpc17xx_lpc40xx.chip.contains("LPC407")
        core_cortex.core: "m4"
        core_cortex_fp.fp: "hard"
        cpp.defines:  [
            "CHIP_LPC407X_8X",
            "CHIP_LPC40XX",
            "BOARD_CHIP_".concat(chip),

        ]
    }
    Properties {
        condition: chip_lpc17xx_lpc40xx.chip.contains("LPC17")
        core_cortex.core: "m3"
        cpp.defines:  [
            "CHIP_LPC175X_6X",
            "BOARD_CHIP_".concat(chip),
        ]
    }

    cpp.includePaths: [
        path+"/include",
        path+"/startup",
        path+"/lpc_ip",
        path+"/lpc_chip",
        path+"/libled"
    ]

    Group {
        name: "NXP LPC SDK"
        files: [
            "startup/*.c",
            "libled/*.c",
            "lpc_chip/*.c",
            "*/*.h",
        ]
    }

    Group {
        name: "Startup"
        files: ["cmsis/"+core_cortex.core+"/startup/*.*"]
    }
    Group {
        name: "Chip Linker Scripts"
        files: ["cmsis/"+core_cortex.core+"/linker/*.ld"]
        fileTags: ["linkerscript"]
    }
}
