import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "chip_kinetis" }
    Depends { name: "lib_cmsis" }
    Depends { name: "core_cortex" }
    Depends { name: "core_cortex_fp" }

    core_cortex.core: "m4"
    core_cortex_fp.fp: "hard"

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
        name: "Chip Linker Scripts"
        files: ["linker/*.ld"]
        fileTags: ["linkerscript"]
    }
}
