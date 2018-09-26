import qbs


StaticLibrary {
    name: "usb"

    property bool host: false
    property bool device: false
    property bool otg: false
    //    property bool freertos: false


    Depends { name: "cpp" }
    Depends { name: "mk66f18" }


    //    cpp.defines: [ { return (freertos ? "" : "USB_STACK_BM")} ]
    cpp.defines: "SDK_OS_BAREMETAL"

    cpp.includePaths: [
        "utilities/usb",
        "middleware/usb/include",
        "middleware/usb/phy",
        "middleware/usb/osa",
        "middleware/usb/device",
        "middleware/usb/host",
        "middleware/usb/otg",
    ]
    Group {
        name: "utilities"
        condition:  {return host || device || otg}
        prefix: "utilities/usb/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "include"
        condition:  {return host || device || otg}
        prefix: "middleware/usb/include/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "osa"
        condition:  {return host || device || otg}
        prefix: "middleware/usb/osa/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "phy"
        condition:  {return host || device || otg}
        prefix: "middleware/usb/phy/"
        files: ["*.c","*.h"]
    }


    Group {
        name: "device"
        condition: device
        prefix: "middleware/usb/device/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "host"
        condition: host
        prefix: "middleware/usb/host/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "otg"
        condition: otg
        prefix: "middleware/usb/otg/"
        files: ["*.c","*.h"]
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: product.cpp.includePaths
        cpp.defines: product.cpp.defines
    }
}
