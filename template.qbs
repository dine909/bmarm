import qbs
import qbs.FileInfo
import "platform/teensy36/teensy36.qbs" as Board

Project {
    references: ["lib/mk66f18/mk66f18.qbs"]

    Board {
        name : "template"

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
