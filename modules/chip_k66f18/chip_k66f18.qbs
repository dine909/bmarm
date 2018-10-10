import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }

    cpp.defines:  [
        "CORE_M4",
        "CPU_MK66FX1M0VLQ18",
    ]

    cpp.driverFlags: [
        "-mcpu=cortex-m4"   ,
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",
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
        files: ["linker/MK66FX1M0xxx18_flash.ld"]
        fileTags: ["linkerscript"]
    }
}
