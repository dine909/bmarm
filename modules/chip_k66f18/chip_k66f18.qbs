import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "chip_kinetis" }
    Depends { name: "lib_cmsis" }
    Depends { name: "core_cortex" }
    Depends { name: "core_cortex_fp" }
    Depends { name: "chipGen" }

    core_cortex.core: "m4"
    core_cortex_fp.fp: "hard"
//    core_cortex_fp.fp: "hard"

    chipGen.memory:  ["flash", "1280k - 0x410", "ram", "256k" ]

    chipGen.origin:  [
        "flash",    "rx",   "0x00000410",
        "ram" ,     "rwx",  "0x1FFFF000",
    ]

    cpp.defines:  [
        "CPU_MK66FX1M0VLQ18",
    ]

    cpp.assemblerFlags: [
                    "-D__STARTUP_CLEAR_BSS",
                    "-D__STARTUP_INITIALIZE_NONCACHEDATA",
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
        name: "NXP Freescale SDK"
        files: [
            "*/*.S",
            "*/*.c",
            "*/*.cpp",
            "*/*.h",
        ]
    }

    Group {
        name: "Linker Scripts (Chip)"
        files: ["linker/*.ld"]
        fileTags: ["orderedLinkerScript_chip"]
    }
}
