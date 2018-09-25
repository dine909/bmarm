import qbs
import qbs.FileInfo
import "platform/teensy36/teensy36.qbs" as Board
import "lib/mk66f18/usb.qbs" as Usb

Project {


    references: [
        "lib/mk66f18/mk66f18.qbs",
    ]

    Usb {
        device:  false
        host:    false
        otg:     false
    }

    Board {
        name : "template"

//        cpp.defines: ["__cplusplus"]
        Depends { name: "usb"}

        Group {
            name: "application"
            prefix: "src/"
            files: [
                "*.c",
                "*.cpp",
                "*.h",
            ]
        }
    }
}
