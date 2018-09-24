import qbs
import qbs.FileInfo
import "platform/teensy36/teensy36.qbs" as Board

Project {
    references: [
        "lib/mk66f18/mk66f18.qbs",
//        "lib/mk66f18/usb.qbs",
    ]

    Board {
        name : "template"

//        Depends { name: "usb" }

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
