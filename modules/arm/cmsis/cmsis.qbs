import qbs
import "../../common/armbuild.qbs" as armbuild
Project {
    name: "sdk"

    property bool USB_host: false
    property bool USB_device: false
    property bool USB_otg: false

    property pathList externalIncludes: []

    armbuild {}

    StaticLibrary {
        name: "armcmsis"
        Depends { name: "cpp" }

        //        qbs.architecture: "arm"
        //        cpp.architecture: "armv4t"

        cpp.assemblerFlags: [
            //            "-D__STARTUP_CLEAR_BSS",
            //            "-D__STARTUP_INITIALIZE_NONCACHEDATA",
        ]
        cpp.positionIndependentCode: false

        cpp.includePaths: [
            "include"
        ]

        cpp.defines:  [
            "IS_BARE_METAL",
            qbs.buildVariant === "debug"?"DEBUG":"NDEBUG"
        ]

        cpp.driverFlags: [
            "--specs=nano.specs",
            "--specs=nosys.specs",
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


        //        cpp.cxxLanguageVersion: "c++14"
        cpp.linkerFlags: [
            "--gc-sections",
            "-static", "-z", "muldefs",
            "-lm","-lc","-lgcc","-lnosys"
        ]


        Export {
            Depends { name: "cpp" }
            Depends { name: "armbuild" }

            cpp.includePaths: product.cpp.includePaths
            cpp.defines:product.cpp.defines
            cpp.assemblerFlags: product.cpp.assemblerFlags
            cpp.driverFlags: product.cpp.driverFlags
            cpp.commonCompilerFlags: product.cpp.commonCompilerFlags
            cpp.positionIndependentCode: product.cpp.positionIndependentCode
            //            cpp.architecture: product.cpp.architecture
            cpp.linkerFlags:product.cpp.linkerFlags
            Group {
                name: "generic linker"
                files: ["linker/*.ld"]
                fileTags: ["linkerscript"]
            }
            Group {
                name: "startup"
                prefix: "startup"
                files: [
                    "*/*.S",
                    "*/*.c",
                    "*/*.cpp",
                    "*/*.h",
                ]
                //            cpp.driverFlags:["-fpermissive"].concat(product.cpp.driverFlags)
            }
        }
    }
}
