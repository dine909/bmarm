import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }

    property string chip: "none"

    cpp.defines:  [
        "CPU_"+chip,
    ]

    cpp.driverFlags: [
        "-mcpu=cortex-m0plus",
        "-mfloat-abi=soft",
        "-mthumb",
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
        files: ["linker/*.ld"]
        fileTags: ["linkerscript"]
    }
}
