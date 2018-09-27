import qbs
import qbs.FileInfo

Module {
    name: "dmp"
    FileTagger {
        patterns: ["*.dmp"]
        fileTags: ["dmp"]
    }
    Rule {
        inputs: ["dmp"]
        outputFileTags: ["o"]
        prepare: {
            var args = ["-I","binary",
                        "-O","elf32-littlearm",
                        "-B","arm",
                        "--rename-section",".data=.rodata",
                        FileInfo.fileName(input.filePath),
                        output.filePath];

            var cmd = new Command(FileInfo.path(product.cpp.compilerPath) + "/arm-none-eabi-objcopy", args);
            cmd.workingDirectory=FileInfo.path(input.filePath);
            cmd.description = "converting data:  " + FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
        Artifact {
            fileTags: ["obj"]
            filePath: FileInfo.baseName(input.filePath) + ".o"
        }
    }
}
