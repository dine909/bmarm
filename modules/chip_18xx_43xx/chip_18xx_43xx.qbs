import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }

    property string chip: "none"

    Group {
        name: "Compile config for " + chip_18xx_43xx.chip
        condition: chip_18xx_43xx.chip.contains("LPC408") ||
                   chip_18xx_43xx.chip.contains("LPC407")
        cpp.defines:  [
            "CHIP_LPC407X_8X",
        ]

        cpp.driverFlags: [
            "-mcpu=cortex-m4"   ,
            "-mfloat-abi=hard",
            "-mfpu=fpv4-sp-d16",
            "-mthumb",
        ]
    }

    cpp.defines:  [
        "BOARD_CHIP_".concat(chip)
    ]

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
