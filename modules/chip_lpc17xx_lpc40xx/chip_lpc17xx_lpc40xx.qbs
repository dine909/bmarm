import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }

    property string chip: "none"

    Properties {
        name: "Compile config for " + chip_lpc17xx_lpc40xx.chip
        condition: chip_lpc17xx_lpc40xx.chip.contains("LPC408") ||
                   chip_lpc17xx_lpc40xx.chip.contains("LPC407")

        cpp.defines:  [
            "CHIP_LPC407X_8X",
            "BOARD_CHIP_".concat(chip)
        ]

        cpp.driverFlags: [
            "-mcpu=cortex-m4"   ,
            "-mfloat-abi=hard",
            "-mfpu=fpv4-sp-d16",
            "-mthumb",
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
        name: "sdk"
        files: [
            "startup/*.c",
            "libled/*.c",
            "lpc_chip/*.c",
            "*/*.s",
//            "*/*.S",
//            "*/*.c",
//            "*/*.cpp",
            "*/*.h",
        ]
    }

    Group {
        name: "linker scripts"
        files: ["linker/*.ld"]
        fileTags: ["linkerscript"]
    }
}
