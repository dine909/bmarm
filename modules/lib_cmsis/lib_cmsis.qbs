import qbs
//import "../../common/armbuild.qbs" as armbuild
Module {
    //    armbuild {}

    Depends { name: "cpp" }
    Depends { name: "armbuild" }

    cpp.positionIndependentCode: false

    cpp.includePaths: [
        path+"/include"
    ]

    cpp.defines:  [
        "IS_BARE_METAL",
        qbs.buildVariant === "debug"?"DEBUG":"NDEBUG"
    ]
    Group {
        name: "cmsis"
        prefix: "include"
        files: [
            "*/*.h",
        ]
        //            cpp.driverFlags:["-fpermissive"].concat(product.cpp.driverFlags)
    }
    cpp.driverFlags: [
        "--specs=nano.specs",
        "--specs=nosys.specs",
//        "-mfloat-abi=hard",
//        "-mfpu=fpv4-sp-d16",
        "-fno-exceptions",
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


    //        cpp.cxxLanguageVersion: "c++14"
    cpp.linkerFlags: [
        "--gc-sections",
        "-static",
        "-z", "muldefs",
        "-lm","-lc","-lgcc","-lnosys"
    ]
    //    Group {
    //        name: "generic linker"
    //        prefix: product.cmsis.module_prefix
    //        files: ["linker/*.ld"]
    //        fileTags: ["linkerscript"]
    //    }
    //    Group {
    //        name: "startup"
    //        prefix: product.cmsis.module_prefix+"/startup"
    //        files: [
    //            "*/*.S",
    //            "*/*.c",
    //            "*/*.cpp",
    //            "*/*.h",
    //        ]
    //        //            cpp.driverFlags:["-fpermissive"].concat(product.cpp.driverFlags)
    //    }
}
