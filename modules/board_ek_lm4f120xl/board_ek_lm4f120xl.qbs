import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }
    Depends { name: "chip_lm4f120xl" }

    // IMPORTANT NOTE:
    // If you intend to use this MCU for anything other than flashing an LED then you WILL have a bad time.
    // Suggest not touching it with a shit stick - It is a giant silicon bug with an on-chip processor:
    // errata rev 1:    http://www.ti.com/lit/pdf/spmz611
    // errata rev 2-5:  who knows.
    // errata rev 6&7:  http://www.ti.com/lit/pdf/spmz856

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
