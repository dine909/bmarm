import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }
    Depends { name: "core_cortex" }
    Depends { name: "core_cortex_fp" }
    Depends { name: "chip_kinetis" }

    property string chip: "none"

    cpp.defines:  [
        "CPU_"+chip,
    ]

    core_cortex.core: "m0plus"
//    core_cortex_fp.fp: "soft"

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
