import qbs
import "usb.qbs" as Usb

Project {
    name: "sdk"

    property bool USB_host: false
    property bool USB_device: false
    property bool USB_otg: false

    property pathList externalIncludes: []

    Usb {
        device:  USB_device
        host:    USB_host
        otg:     USB_otg
        externalIncludes: project.externalIncludes
    }

    StaticLibrary {
        name: "mk66f18"
        Depends { name: "cpp" }

        //        qbs.architecture: "arm"
        //        cpp.architecture: "armv4t"

        cpp.assemblerFlags: ["-D__STARTUP_CLEAR_BSS", "-D__STARTUP_INITIALIZE_NONCACHEDATA"]
        cpp.positionIndependentCode: false

        cpp.includePaths: [
            "CMSIS",
            "drivers",
            "startup",
            "utilities"
        ]

        cpp.defines: { return ["CPU_MK66FX1M0VLQ18","TWR_K65F180M","TOWER"].concat(qbs.buildVariant === "debug"?"DEBUG":"NDEBUG")}

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


//        cpp.cxxLanguageVersion: "c++14"
        cpp.linkerFlags: ["--gc-sections", "-static", "-z", "muldefs","-lm","-lc","-lgcc","-lnosys"]


        Export {
            Depends { name: "cpp" }
            cpp.includePaths: product.cpp.includePaths
            cpp.defines:product.cpp.defines
            cpp.assemblerFlags: product.cpp.assemblerFlags
            cpp.driverFlags: product.cpp.driverFlags
            cpp.commonCompilerFlags: product.cpp.commonCompilerFlags
            cpp.positionIndependentCode: product.cpp.positionIndependentCode
            //            cpp.architecture: product.cpp.architecture
            cpp.linkerFlags:product.cpp.linkerFlags
            Group {
                name: "linker scripts"
                files: ["../../lib/mk66f18/MK66FX1M0xxx18_flash.ld"]
                fileTags: ["linkerscript"]
            }
        }
    }
}
