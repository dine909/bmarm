import qbs
//import "usb.qbs" as Usb
//import "../../../arm/cmsis/cmsis.qbs" as armcmsis
Module{
//    name:"mculib"

    Depends { name: "cpp" }
    Depends { name: "lib_cmsis" }

    //        qbs.architecture: "arm"
    //        cpp.architecture: "armv4t"

    cpp.assemblerFlags: [
                    "-D__STARTUP_CLEAR_BSS",
                    "-D__STARTUP_INITIALIZE_NONCACHEDATA",
    ]
    cpp.positionIndependentCode: false

    cpp.includePaths: [
        path+"/cmsis",
        path+"/drivers",
        path+"/startup",
        path+"/utilities"
    ]

    cpp.defines:  [
        "CPU_MK66FX1M0VLQ18",
        "IS_BARE_METAL",
        qbs.buildVariant === "debug"?"DEBUG":"NDEBUG"
    ]

    Group {
        name: "sdk"
//                prefix: "lib/mk66f18/"
        files: [
            "*/*.S",
            "*/*.c",
            "*/*.cpp",
            "*/*.h",
        ]
        //            cpp.driverFlags:["-fpermissive"].concat(product.cpp.driverFlags)
    }

    cpp.driverFlags: [
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",
    ]

    Group {
        name: "linker scripts"
        files: ["linker/MK66FX1M0xxx18_flash.ld"]
        fileTags: ["linkerscript"]
    }
}
