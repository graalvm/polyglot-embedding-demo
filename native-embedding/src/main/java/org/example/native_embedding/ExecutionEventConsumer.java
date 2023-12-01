package org.example.native_embedding;

import org.graalvm.polyglot.management.ExecutionEvent;

public class ExecutionEventConsumer {
    static {
        System.loadLibrary("ExecutionEventConsumer");
    }

    static native void consumeExecutionEvent(ExecutionEvent executionEvent, long callbackPointer);
}
