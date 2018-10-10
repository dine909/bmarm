import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }
    Depends { name: "chip_kl25z" }

    chip_kl25z.chip: "MKL25Z128VLK4"

    cpp.includePaths: [
        path+"/src",
    ]

    Group {
        name: "Board FRDM KL25Z"
        prefix: "src/"
        files: [
            "*/*.c",      "*.c",
            "*/*.cpp",    "*.cpp",
            "*/*.h",      "*.h",
        ]
    }
}
