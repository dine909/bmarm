import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }
    Depends { name: "chip_lm4f120xl" }

    chip_lm4f120xl.chip: "LM4F120XL"

    cpp.includePaths: [
        path+"/src",
    ]

    Group {
        name: "Board EK-LM4F120XL"
        prefix: "src/"
        files: [
            "*/*.c",      "*.c",
            "*/*.cpp",    "*.cpp",
            "*/*.h",      "*.h",
        ]
    }
}
