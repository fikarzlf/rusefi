package com.rusefi.output;

import com.rusefi.ConfigDefinition;
import com.rusefi.ReaderState;
import com.rusefi.util.LazyFile;

import java.io.IOException;

import static com.rusefi.ConfigDefinition.EOL;

public class FileFsioSettingsConsumer extends FsioSettingsConsumer {
    private final LazyFile enumFile;
    private final LazyFile gettersFile;

    public FileFsioSettingsConsumer(ReaderState state, String destCFsioConstantsFileName, String destCFsioGettersFileName) {
        super(state);
        enumFile = new LazyFile(destCFsioConstantsFileName);
        gettersFile = new LazyFile(destCFsioGettersFileName);
    }

    @Override
    public void startFile() {
        enumFile.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + EOL);
        enumFile.write("// by " + getClass() + EOL);

        gettersFile.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + EOL);
        gettersFile.write("// by " + getClass() + EOL);
    }

    @Override
    public void endFile() throws IOException {
        enumFile.write(enumDefinition.toString());
        enumFile.close();

        gettersFile.write(getContent());
        gettersFile.close();
    }
}