import qbs
Module{

    Depends { name: "cpp" }

    Group {
        name: "Linker Scripts (1)"
        files: ["linker/*.ld"]
        fileTags: ["orderedLinkerScript_chip_family"]
    }
}

