#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "nativejavavm.c"


jobject name_callback(jobject arg) {
  char *name_chars = "Unknown";
  if (arg != NULL) {
    jobject argString = (*env)->CallObjectMethodA(env, arg, m_value_asString, NULL);
    CHECK_THROW()
    if (argString != NULL) {
      const char *arg_chars = get_string_chars(argString);
      if (strcmp(arg_chars, "name") == 0) {
         name_chars = "Native Callback";
      }
      release_string_chars(argString, arg_chars);
    }
  }
  jstring name = (*env)->NewStringUTF(env, name_chars);
  CHECK_THROW()
  return name;
}

int main(int argc, char** argv) {

    create_java_vm(argc, argv);

    lookup_classes_and_methods();

    jvalue args [1] = {0};
    args[0].j = (jlong)(&name_callback);

    (*env)->CallStaticObjectMethodA(env, t_api, m_api_helloWithCallback, args);
    CHECK_THROW()

    (*vm)->DestroyJavaVM(vm);
    return 0;
}

