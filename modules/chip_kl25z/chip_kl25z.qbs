import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }
    Depends { name: "core_cortex" }
    Depends { name: "core_cortex_fp" }
    Depends { name: "chip_kinetis" }
    Depends { name: "chipGen" }

    property string chip: "none"

    cpp.defines:  [
        "CPU_"+chip,
        "BOARD_CHIP_".concat(chip),
    ]

    core_cortex.core: "m0plus"
//    core_cortex_fp.fp: "hard"

    chipGen.memory:  ["flash", "128k - 0x410", "ram", "16k" ]

    chipGen.origin:  [
        "flash",    "rx",   "0x00000410",
        "ram" ,     "rwx",  "0x1FFFF000",
    ]


    cpp.assemblerFlags: [
        //                    "-D__STARTUP_CLEAR_BSS",
        //                    "-D__STARTUP_INITIALIZE_NONCACHEDATA",
    ]

    cpp.includePaths: [
        path+"/include",
        path+"/cmsis",
        path+"/drivers",
        path+"/startup",
        path+"/utilities",
        path+"/libled"
    ]

    Group {
        name: "sdk"
        files: [
            "*/*.S",
            "*/*.c",
            "*/*.cpp",
            "*/*.h",
        ]
    }

    Group {
        name: "Linker Scripts (0)"
        files: ["linker/*.ld"]
        fileTags: ["orderedLinkerScript_chip"]
    }
}
