import qbs
import qbs.FileInfo
import qbs.TextFile

Module{

    Rule {
        id: linkerGen
        inputs: [
            "orderedLinkerScript",
            "orderedLinkerScript_0",
            "orderedLinkerScript_chip",
            "orderedLinkerScript_chip_family",
            "orderedLinkerScript_board",
            "orderedLinkerScript_cmsis",
            "orderedLinkerScript_1",
            "orderedLinkerScript_2",
            "orderedLinkerScript_3",
            "orderedLinkerScript_4",
            "orderedLinkerScript_5",
        ]
        alwaysRun: true
        multiplex: true

        Artifact {
            fileTags: "linkerscript"
            filePath: FileInfo.joinPaths(product.buildDirectory,"linker.ld")
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description =  output.fileName;
            cmd.highlight = "linkergen";
            cmd.sourceCode = function() {
                var args = Object.keys(inputs).reduce(function (acc, key) {
                    var value = inputs[key];
                    if(value) {
                        return acc.concat(value);
                    }
                    return acc;
                }, []);

                var ofile = new TextFile(output.filePath, TextFile.WriteOnly);
                for (f in args){
                    var file = new TextFile(args[f].filePath);
                    ofile.write("/* "+args[f].filePath+" */\n\n")
                    ofile.write(file.readAll());
                    file.close()
                }
                ofile.close();
            }
            return [cmd];
        }
    }

}
