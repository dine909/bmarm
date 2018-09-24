import qbs

StaticLibrary {
    name: "usb"
    Depends { name: "cpp" }
    Depends { name: "mk66f18" }

    cpp.defines: ["USB_STACK_BM"]

    cpp.includePaths: [
        "lib/mk66f18/utilities/usb",
        "lib/mk66f18/middleware/usb/include",
        "lib/mk66f18/middleware/usb/phy",
        "lib/mk66f18/middleware/usb/osa",
        "lib/mk66f18/middleware/usb/device",

    ]

    Group {
        name: "usb utilities"
        prefix: "utilities/usb/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "usb include"
        prefix: "middleware/usb/include/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "usb osa"
        prefix: "middleware/usb/osa/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "usb phy"
        prefix: "middleware/usb/phy/"
        files: ["*.c","*.h"]
    }
    Group {
        name: "usb device"
        prefix: "middleware/usb/device/"
        files: ["*.c","*.h"]
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: product.cpp.includePaths
        cpp.defines: product.cpp.defines
    }
}
