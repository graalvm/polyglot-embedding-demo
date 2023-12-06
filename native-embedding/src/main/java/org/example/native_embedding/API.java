package org.example.native_embedding;

import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.Value;
import org.graalvm.polyglot.management.ExecutionListener;
import org.graalvm.polyglot.proxy.ProxyExecutable;

public class API {
    private static final Context context = Context.newBuilder().allowAllAccess(true).build();

    public static void helloWithCallback(long nameCallbackFunctionPointer) {
        ProxyExecutable nameCallback = (args) -> CallbackTrampoline.jumpToCallback(nameCallbackFunctionPointer, args[0]);
        context.getBindings("js").putMember("nameCallback", nameCallback);
        context.eval("js", "print('Hello from ' + nameCallback('name'))");
    }
}
