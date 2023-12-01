package org.example.native_embedding;

import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.Value;
import org.graalvm.polyglot.management.ExecutionListener;

public class API2 {
    private static final Context context = Context.newBuilder().allowAllAccess(true).build();

    public static void helloFromJS() {
        context.eval("js", "print('Hello JavaScript!');");
        System.out.flush();
    }

    public static void nativeCallbackOnEachJSStatementReturn(String jsSource, long callbackPointer) {
        System.out.flush();
        try (ExecutionListener listener =
                     ExecutionListener.newBuilder().statements(true).collectReturnValue(true).onReturn(executionEvent -> {
                         if (executionEvent.getReturnValue() != null) {
                         }
                         if (callbackPointer != 0L) {
                             ExecutionEventConsumer.consumeExecutionEvent(executionEvent, callbackPointer);
                         }
                     }).attach(context.getEngine())) {
            context.eval("js", jsSource);
        }
    }

    public static Value evalJSFunctionWithArgumentAndReturn(String jsSource, Object arg) {
        Value executable = context.eval("js", jsSource);
        System.out.flush();
        return executable.execute(arg);
    }
}
