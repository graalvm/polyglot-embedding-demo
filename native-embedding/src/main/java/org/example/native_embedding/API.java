package org.example.native_embedding;

import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.CHeader;
import org.graalvm.nativeimage.c.function.CEntryPoint;
import org.graalvm.nativeimage.c.struct.CPointerTo;
import org.graalvm.nativeimage.c.type.CCharPointer;
import org.graalvm.nativeimage.c.type.CConst;
import org.graalvm.nativeimage.c.type.CLongPointer;
import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.PolyglotException;
import org.graalvm.polyglot.Value;
import org.graalvm.word.PointerBase;
import org.graalvm.word.UnsignedWord;

import java.nio.charset.StandardCharsets;

@CHeader(value = APIHeader.class)
public class API {

    @CPointerTo(nameOfCType = "size_t")
    public interface SizeTPointer extends PointerBase {

        UnsignedWord read();

        void write(UnsignedWord value);

    }

    private static int handleException(Throwable t) {
        if (t instanceof PolyglotException) {
            return 1;
        } else {
            return 2;
        }
    }

    @CEntryPoint(name = "nea_hello_from_js")
    public static int nea_hello_from_js(IsolateThread thread) {
        try {
            try (Context context = Context.newBuilder().allowAllAccess(true).build()) {
                context.eval("js", "print('Hello JavaScript!');");
            }
        } catch (Throwable t) {
            return handleException(t);
        }
        return 0;
    }

    @CEntryPoint(name = "nea_string_permutation_js")
    public static int nea_string_permutation_js(IsolateThread thread, @CConst CCharPointer original_string_utf8,
                                                CCharPointer result_buffer, UnsignedWord result_buffer_length,
                                                SizeTPointer result_length) {
        try {
            nullCheck(original_string_utf8, "original_string_utf8");
            String originalString = CTypeConversion.utf8ToJavaString(original_string_utf8);
            try (Context context = Context.newBuilder().allowAllAccess(true).build()) {
                Value permutation = context.eval("js", """
                        function permutation(s) {
                          var a = Array.from(s);
                          permutationHelper(a, 0);
                          return a.join("");
                        }
                                            
                        function permutationHelper(a, off) {
                          if (off >= a.length) {
                            return;
                          }
                          var i = Math.floor(Math.random()*(a.length - off))
                          var tmp = a[off];
                          a[off] = a[off + i];
                          a[off + i] = tmp;
                          permutationHelper(a, off + 1);
                        }
                        permutation
                        """);
                writeUTF8String(permutation.execute(originalString).toString(), result_buffer, result_buffer_length,
                        result_length);
            }
        } catch (Throwable t) {
            return handleException(t);
        }
        return 0;
    }

    @CEntryPoint(name = "nea_fibonacci_js")
    public static int nea_fibonacci_js(IsolateThread thread, int n, CLongPointer result) {
        try {
            try (Context context = Context.newBuilder().allowAllAccess(true).build()) {
                Value fibonacci = context.eval("js", """
                        function fibonacci(n) {
                          if (n == 1 || n == 2) {
                            return 1;
                          }
                          a = 1;
                          b = 1;
                          var c = 0;
                          for (var i = 2; i < n; i++) {
                            c = a + b;
                            a = b;
                            b = c;
                          }
                          return c;
                        }
                        fibonacci
                        """);
                result.write(fibonacci.execute(n).asLong());
            }
        } catch (Throwable t) {
            return handleException(t);
        }
        return 0;
    }

    private static void nullCheck(PointerBase ptr, String fieldName) {
        if (ptr.isNull()) {
            throw new NullPointerException(fieldName + " must be not be null");
        }
    }

    private static void writeUTF8String(String valueString, CCharPointer buffer, UnsignedWord length,
                                        SizeTPointer result) {
        result.write(CTypeConversion.toCString(valueString, StandardCharsets.UTF_8, buffer, length));
    }
}
