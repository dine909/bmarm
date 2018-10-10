import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }

    Depends {
        name: "chip_mk66f18"
    }

    cpp.includePaths: [
        path+"/src",
    ]

    Group {
        name: "Board Teensy 3.6"
        prefix: "src/"
        files: [
            "*/*.c",      "*.c",
            "*/*.cpp",    "*.cpp",
            "*/*.h",      "*.h",
        ]
    }
}
