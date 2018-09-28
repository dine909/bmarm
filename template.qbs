import qbs
import qbs.FileInfo

import "platform/teensy36/teensy36.qbs" as Board

Board {
    projectName : "template"
    projectFiles : [
        "*.c",
        "*.cpp",
        "*.h",
        "*.dmp"
    ]

    USE_USB_device: false
    USE_USB_host: false
    USE_USB_otg: false

}
