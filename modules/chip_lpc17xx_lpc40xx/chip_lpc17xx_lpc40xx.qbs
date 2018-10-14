import qbs

Module{
    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }
    Depends { name: "core_cortex" }
    Depends { name: "core_cortex_fp" }
    Depends { name: "chipGen" }

    property string chip: "none"
    property string uchip: chip_lpc17xx_lpc40xx.chip.toUpperCase()

    property bool lpc176x: (new RegExp(/^LPC176[0-9]/)).exec(chip_lpc17xx_lpc40xx.chip)

    Properties { condition: uchip==="LPC4088" ;  chipGen.memory:  ["flash", "512k", "ram", "64k", "pram", "32k" ] }

    Properties { condition: uchip==="LPC1769" ;  chipGen.memory:  ["flash", "512k", "ram", "32k", "pram", "32k" ] }

    Properties {
        condition: chip_lpc17xx_lpc40xx.chip.contains("LPC408") || chip_lpc17xx_lpc40xx.chip.contains("LPC407")
        core_cortex.core: "m4" ;core_cortex_fp.fp: "hard"
        cpp.defines:  [ "CHIP_LPC407X_8X", "CHIP_LPC40XX", "BOARD_CHIP_".concat(chip),]
        chipGen.origin:  [
            "flash",    "rx",   "0x00000000",
            "ram" ,     "rwx",  "0x10000000",
            "pram" ,    "rwx",  "0x20002000"
        ]
    }
    Properties {
        condition: lpc176x
        core_cortex.core: "m3"
        cpp.defines:  ["CHIP_LPC175X_6X", "BOARD_CHIP_".concat(chip), ]
        chipGen.origin:  [
            "flash",    "rx",   "0x00000000",
            "ram" ,     "rwx",  "0x10000000",
            "pram" ,    "rwx",  "0x2007C000"
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
//    Group {
//        name: "Linker Scripts (Chip)"
//        files: ["linker/*.ld"]
//        fileTags: ["orderedLinkerScript_chip"]
//    }
}
