#include <stdio.h>
#include "org_example_native_embedding_ExecutionEventConsumer.h"

JNIEXPORT void JNICALL Java_org_example_native_1embedding_ExecutionEventConsumer_consumeExecutionEvent
  (JNIEnv *env, jclass clazz, jobject event, jlong callback_ptr) {
     if (callback_ptr) {
         void (*pmethod)(jobject);
         pmethod = (void (*)(jobject))callback_ptr;
         pmethod(event);
     }
     return;
}
