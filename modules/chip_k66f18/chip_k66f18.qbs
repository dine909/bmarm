import qbs
Module{

    Depends { name: "cpp" }
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
        name: "sdk"
        files: [
            "*/*.S",
            "*/*.c",
            "*/*.cpp",
            "*/*.h",
        ]
    }

    Group {
        name: "linker scripts"
        files: ["linker/MK66FX1M0xxx18_flash.ld"]
        fileTags: ["linkerscript"]
    }
}
