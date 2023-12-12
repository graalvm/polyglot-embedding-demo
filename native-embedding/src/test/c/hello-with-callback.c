#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "nativejavavm.c"


jobject name_callback(jobject arg) {
  char *name_chars = "Unknown";
  if (arg != NULL) {
    // The argument is of type org.graalvm.polyglot.Value, convert it to String by calling
    // the asString method on it
    jobject argString = (*env)->CallObjectMethodA(env, arg, m_value_asString, NULL);
    CHECK_THROW()
    if (argString != NULL) {
      const char *arg_chars = get_string_chars(argString);
      // In case the string is equal to "name" assign "Native Callback" to the variable
      // name_chars that will be used to create the value returned form this method
      if (strcmp(arg_chars, "name") == 0) {
         name_chars = "Native Callback";
      }
      release_string_chars(argString, arg_chars);
    }
  }
  // Create a jstring from name_chars and return it
  jstring name = (*env)->NewStringUTF(env, name_chars);
  CHECK_THROW()
  return name;
}

int main(int argc, char** argv) {

    create_java_vm(argc, argv);

    lookup_classes_and_methods();


    // Create polyglot context
    (*env)->CallStaticObjectMethodA(env, t_api, m_api_setup, NULL);
    CHECK_THROW()

    // Call the "helloWithCallback" API method, pass the pointer to the callback function
    // as jlong to the API method.
    jvalue args [1] = {0};
    args[0].j = (jlong)(&name_callback); // j in jvalue is for jlong (long long int)
    (*env)->CallStaticObjectMethodA(env, t_api, m_api_helloWithCallback, args);
    CHECK_THROW()

    // Close polyglot context
    (*env)->CallStaticVoidMethodA(env, t_api, m_api_tearDown, args);
    CHECK_THROW()

    (*vm)->DestroyJavaVM(vm);
    return 0;
}

