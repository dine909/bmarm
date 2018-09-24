import qbs

StaticLibrary {
    name: "mk66f18"
    Depends { name: "cpp" }

//        qbs.architecture: "arm"
//        cpp.architecture: "armv4t"

    cpp.assemblerFlags: ["-D__STARTUP_CLEAR_BSS", "-D__STARTUP_INITIALIZE_NONCACHEDATA"]

    cpp.includePaths: ["lib/mk66f18/CMSIS",
        "lib/mk66f18/drivers",
        "lib/mk66f18/startup",
        "lib/mk66f18/utilities"
    ]

    cpp.defines:  ["NDEBUG","CPU_MK66FX1M0VLQ18","TWR_K65F180M","TOWER","DEBUG"]

    Group {
        name: "sdk"
//        prefix: "lib/mk66f18/"
        files: [
            "*/*.S",
            "*/*.c",
            "*/*.cpp",
            "*/*.h",
        ]
    }

    cpp.driverFlags: [
        "-mcpu=cortex-m4"   ,
        "--specs=nano.specs",
        "--specs=nosys.specs",
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",
        "-fno-exceptions",
        "-mthumb",
        "-mapcs",
        "-MMD",
        "-MP",
        "-fno-common",
        "-ffunction-sections",
        "-fdata-sections",
        "-ffreestanding",
        "-fno-builtin",
        "-static"
    ]
    cpp.cxxFlags: [
        "-fno-rtti"
    ]


    cpp.cLanguageVersion: "c11"
    cpp.linkerFlags: ["--gc-sections", "-static", "-z", "muldefs","-lm","-lc","-lgcc","-lnosys"]


    Export {
        Depends { name: "cpp" }
        cpp.includePaths: product.cpp.includePaths
        cpp.defines:product.cpp.defines
        cpp.assemblerFlags: product.cpp.assemblerFlags
        cpp.driverFlags: product.cpp.driverFlags
        cpp.commonCompilerFlags: product.cpp.commonCompilerFlags
//            cpp.architecture: product.cpp.architecture
        cpp.linkerFlags:product.cpp.linkerFlags

    }
}
