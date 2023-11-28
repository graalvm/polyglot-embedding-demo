package org.example.native_embedding;

import org.graalvm.nativeimage.c.CHeader.Header;

public class APIHeader implements Header {
    @Override
    public String name() {
        return "native_embedding_api";
    }
}
