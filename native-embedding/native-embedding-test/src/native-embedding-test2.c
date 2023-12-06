#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>

#define CHECK_THROW() if ((*env)->ExceptionCheck(env)) { \
    printf("JNI error at %s:%d\n", __FILE__, __LINE__); \
    exit(1); \
}

JavaVM* vm = NULL;
JNIEnv* env = NULL;

jclass t_executionevent = NULL;
jclass t_sourcesection = NULL;
jclass t_value = NULL;
jclass t_charsequence = NULL;
jclass t_api2 = NULL;

jmethodID m_executionevent_getRootName = NULL;
jmethodID m_executionevent_getLocation = NULL;
jmethodID m_executionevent_getReturnValue = NULL;
jmethodID m_sourcesection_getCharacters = NULL;
jmethodID m_charsequence_length = NULL;
jmethodID m_charsequence_charAt = NULL;
jmethodID m_value_toString = NULL;
jmethodID m_value_asString = NULL;
jmethodID m_api2_helloFromJS = NULL;
jmethodID m_api2_nativeCallbackOnEachJSStatementReturn = NULL;
jmethodID m_api2_evalJSFunctionWithArgumentAndReturn = NULL;


const char* get_string_chars(jstring str) {
   jboolean isCopy;
   const char* chars = (*env)->GetStringUTFChars(env, str, &isCopy);
   CHECK_THROW()
   return chars;
}

void release_string_chars(jstring str, const char *chars) {
   (*env)->ReleaseStringUTFChars(env, str, chars);
   CHECK_THROW()
}

char *getOptionString(char *optionParam, char *optionValue) {
    const int optionStringLength = strlen(optionParam) + strlen(optionValue);
    char *optionString = calloc(optionStringLength + 1, sizeof(char));
    strcpy(optionString,  optionParam);
    strcat(optionString, optionValue);
    return optionString;
}

void create_java_vm(int argc, char** argv) {
    if (argc != 5) {
        printf("Expecting exactly 4 arguments: Java Module Path, Main Module, Java Home, and Java Library Path!\n");
        exit(1);
    }

    printf("Java Module Path: %s\n", argv[1]);
    printf("Main Module: %s\n", argv[2]);
    printf("Java Home: %s\n", argv[3]);
    printf("Java Library Path: %s\n", argv[4]);
    fflush(stdout);


    JavaVMInitArgs vm_args;
    JavaVMOption options[4] = {0};
    char *optionModulePath = getOptionString("--module-path=", argv[1]);
    options[0].optionString = optionModulePath;
    char *optionMainModule = getOptionString("-Djdk.module.main=", argv[2]);
    options[1].optionString = optionMainModule;
    char *optionJavaHome = getOptionString("-Djava.home=", argv[3]);
    options[2].optionString = optionJavaHome;
    char *optionJavaLibraryPath = getOptionString("-Djava.library.path=", argv[4]);
    options[3].optionString = optionJavaLibraryPath;

    vm_args.version = JNI_VERSION_10;
    vm_args.nOptions = 4;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    int res = JNI_CreateJavaVM(&vm, (void**)(&env), &vm_args);
    if (res != JNI_OK) {
        printf("Cannot create Java VM\n");
        exit(1);
    }

    free(optionModulePath);
    free(optionMainModule);
    free(optionJavaHome);
    free(optionJavaLibraryPath);
}

void lookup_classes_and_methods() {
    // Lookup classes
    t_executionevent = (*env)->FindClass(env, "org/graalvm/polyglot/management/ExecutionEvent");
    CHECK_THROW()
    t_sourcesection = (*env)->FindClass(env, "org/graalvm/polyglot/SourceSection");
    CHECK_THROW()
    t_charsequence = (*env)->FindClass(env, "java/lang/CharSequence");
    CHECK_THROW()
    t_value = (*env)->FindClass(env, "org/graalvm/polyglot/Value");
    CHECK_THROW()
    jclass t_api2 = (*env)->FindClass(env, "org/example/native_embedding/API2");
    CHECK_THROW()


    // Lookup methods
    m_executionevent_getRootName = (*env)->GetMethodID(env, t_executionevent, "getRootName", "()Ljava/lang/String;");
    CHECK_THROW()
    m_executionevent_getLocation = (*env)->GetMethodID(env, t_executionevent, "getLocation", "()Lorg/graalvm/polyglot/SourceSection;");
    CHECK_THROW()
    m_executionevent_getReturnValue = (*env)->GetMethodID(env, t_executionevent, "getReturnValue", "()Lorg/graalvm/polyglot/Value;");
    CHECK_THROW()
    m_sourcesection_getCharacters = (*env)->GetMethodID(env, t_sourcesection, "getCharacters", "()Ljava/lang/CharSequence;");
    CHECK_THROW()
    m_charsequence_length = (*env)->GetMethodID(env, t_charsequence, "length", "()I");
    CHECK_THROW()
    m_charsequence_charAt = (*env)->GetMethodID(env, t_charsequence, "charAt", "(I)C");
    CHECK_THROW()
    m_value_toString = (*env)->GetMethodID(env, t_value, "toString", "()Ljava/lang/String;");
    CHECK_THROW()
    m_value_asString = (*env)->GetMethodID(env, t_value, "asString", "()Ljava/lang/String;");
    CHECK_THROW()
    m_api2_helloFromJS = (*env)->GetStaticMethodID(env, t_api2, "helloFromJS", "()V");
    CHECK_THROW()
    m_api2_nativeCallbackOnEachJSStatementReturn = (*env)->GetStaticMethodID(env, t_api2, "nativeCallbackOnEachJSStatementReturn", "(Ljava/lang/String;J)V");
    CHECK_THROW()
    m_api2_evalJSFunctionWithArgumentAndReturn = (*env)->GetStaticMethodID(env, t_api2, "evalJSFunctionWithArgumentAndReturn", "(Ljava/lang/String;Ljava/lang/Object;)Lorg/graalvm/polyglot/Value;");
    CHECK_THROW()
}

void execution_event_callback(jobject event) {
  jstring root_name = (*env)->CallObjectMethodA(env, event, m_executionevent_getRootName, NULL);
  CHECK_THROW()
  if (root_name == NULL) {
    printf("Root name is NULL\n");
  } else {
    const char *root_name_chars = get_string_chars(root_name);
    printf("Root name: %s\n", root_name_chars);
    release_string_chars(root_name, root_name_chars);
  }
  jobject location = (*env)->CallObjectMethodA(env, event, m_executionevent_getLocation, NULL);
  CHECK_THROW()
  if (location == NULL) {
    printf("Section is NULL\n");
  } else {
    jobject charseq = (*env)->CallObjectMethodA(env, location, m_sourcesection_getCharacters, NULL);
    CHECK_THROW()
    const int length = (*env)->CallIntMethod(env, charseq, m_charsequence_length);
    CHECK_THROW()
    char *section_chars = calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; i++) {
      section_chars[i] = (*env)->CallCharMethod(env, charseq, m_charsequence_charAt, i);
    }
    section_chars[length] = 0;
    printf("Section: %s\n", section_chars);
  }
  jobject returnValue = (*env)->CallObjectMethodA(env, event, m_executionevent_getReturnValue, NULL);
  CHECK_THROW()
  if (returnValue == NULL) {
    printf("Return Value is NULL\n");
  } else {
    jobject returnValueString = (*env)->CallObjectMethodA(env, returnValue, m_value_toString, NULL);
    CHECK_THROW()
    if (returnValueString == NULL) {
      printf("Return Value String is NULL\n");
    } else {
      const char *return_value_chars = get_string_chars(returnValueString);
      printf("Return Value: %s\n", return_value_chars);
      release_string_chars(returnValueString, return_value_chars);
    }
  }
  fflush(stdout);
}

int main(int argc, char** argv) {

    create_java_vm(argc, argv);

    lookup_classes_and_methods();

    // Call helloFromJS
    (*env)->CallStaticObjectMethodA(env, t_api2, m_api2_helloFromJS, NULL);
    CHECK_THROW()

    // Call nativeCallbackOnEachJSStatementReturn
    jstring jsSource = (*env)->NewStringUTF(env, "function fibonacci(n) { if (n == 1 || n == 2) { return 1; } a = 1; b = 1; var c = 0; for (var i = 2; i < n; i++) { c = a + b; a = b; b = c; } return c; } fibonacci(50);");
    jvalue args [2] = {0};
    args[0].l = jsSource;
    args[1].j = (jlong)(&execution_event_callback);

    (*env)->CallStaticObjectMethodA(env, t_api2, m_api2_nativeCallbackOnEachJSStatementReturn, args);
    CHECK_THROW()
    (*env)->DeleteLocalRef(env, jsSource);
    CHECK_THROW()

    // Call evalJSFunctionWithArgumentAndReturn
    jstring jsPermutationFunction = (*env)->NewStringUTF(env, "function permutation(s) { var a = Array.from(s); permutationHelper(a, 0); return a.join(\"\"); } function permutationHelper(a, off) { if (off >= a.length) { return; } var i = Math.floor(Math.random()*(a.length - off)); var tmp = a[off]; a[off] = a[off + i]; a[off + i] = tmp; permutationHelper(a, off + 1); } permutation");
    char string_to_permute_chars[20];
    printf("Enter a string for random permutation, max. 19 characters: ");
    fflush(stdout);
    fgets(string_to_permute_chars, 20, stdin);
    int i = strlen(string_to_permute_chars) - 1;
    while (i >= 0 && (string_to_permute_chars[i] == '\n' || string_to_permute_chars[i] == '\r')) {
      string_to_permute_chars[i] = 0;
      i--;
    }
    jstring stringToPermute = (*env)->NewStringUTF(env, string_to_permute_chars);
    memset(args, 0, sizeof(args));
    args[0].l = jsPermutationFunction;
    args[1].l = stringToPermute;

    jobject permutedStringValue = (*env)->CallStaticObjectMethodA(env, t_api2, m_api2_evalJSFunctionWithArgumentAndReturn, args);
    CHECK_THROW()
    jstring permutedString = (*env)->CallObjectMethodA(env, permutedStringValue, m_value_asString, NULL);
    CHECK_THROW()
    const char *permuted_string_chars = get_string_chars(permutedString);
    printf("Random permutation of \"%s\": %s\n", string_to_permute_chars, permuted_string_chars);
    fflush(stdout);
    release_string_chars(permutedString, permuted_string_chars);

    (*env)->DeleteLocalRef(env, jsPermutationFunction);
    CHECK_THROW()
    (*env)->DeleteLocalRef(env, stringToPermute);
    CHECK_THROW()
    (*env)->DeleteLocalRef(env, permutedStringValue);
    CHECK_THROW()
    (*env)->DeleteLocalRef(env, permutedString);
    CHECK_THROW()

    // Shutdown
    (*vm)->DestroyJavaVM(vm);
    return 0;
}
