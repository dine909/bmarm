import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }

    Depends {
        name: "chip_mk66f18"
    }

    cpp.includePaths: [
        path+"/board/src",
        path+"/board",
    ]

    Group {
        name: "board"
        prefix: "board/"
        files: [
            "*/*.c",      "*.c",
            "*/*.cpp",    "*.cpp",
            "*/*.h",      "*.h",
        ]
    }
}
