package org.example.native_embedding;

import org.graalvm.polyglot.Value;

public class CallbackTrampoline {
    static {
        System.loadLibrary("CallbackTrampoline");
    }

    static native Object jumpToCallback(long callbackPointer, Value arg);
}
