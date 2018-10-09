import qbs
import "usb.qbs" as Usb
import "../../../arm/cmsis/cmsis.qbs" as armcmsis

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

    armcmsis {

    }

    StaticLibrary {
        name: "mculib"
        Depends { name: "cpp" }
        Depends { name: "armcmsis" }

        //        qbs.architecture: "arm"
        //        cpp.architecture: "armv4t"

        cpp.assemblerFlags: [
            //            "-D__STARTUP_CLEAR_BSS",
            //            "-D__STARTUP_INITIALIZE_NONCACHEDATA",
        ]
        cpp.positionIndependentCode: false

        cpp.includePaths: [
            "cmsis",
            "drivers",
            "startup",
            "utilities"
        ]

        cpp.defines:  [
            "CPU_MK66FX1M0VLQ18",
            "IS_BARE_METAL",
            qbs.buildVariant === "debug"?"DEBUG":"NDEBUG"
        ]

        Group {
            name: "sdk"
            //        prefix: "lib/mk66f18/"
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
