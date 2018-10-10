import qbs
Module{

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }

    cpp.defines:  [
        "CHIP_LPC407X_8X",
    ]

    cpp.driverFlags: [
        "-mcpu=cortex-m4"   ,
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",
        "-mthumb",
    ]

    cpp.assemblerFlags: [
//                    "-D__STARTUP_CLEAR_BSS",
//                    "-D__STARTUP_INITIALIZE_NONCACHEDATA",
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
