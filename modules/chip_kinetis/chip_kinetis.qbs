import qbs
Module{

    Depends { name: "cpp" }

    Group {
        name: "Linker Scripts (Chip Family)"
        files: ["linker/*.ld"]
        fileTags: ["orderedLinkerScript_chip_family"]
    }
}

