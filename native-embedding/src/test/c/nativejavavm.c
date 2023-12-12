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

jclass t_value = NULL;
jclass t_api = NULL;

jmethodID m_value_asString = NULL;
jmethodID m_api_helloWithCallback = NULL;
jmethodID m_api_fibonacci = NULL;
jmethodID m_api_eval = NULL;
jmethodID m_api_setup = NULL;
jmethodID m_api_tearDown = NULL;


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
    if (argc < 5) {
        printf("Expecting at least 4 arguments starting with: Java Module Path, Main Module, Java Home, and Java Library Path!\n");
        exit(1);
    }

    printf("Java Module Path: %s\n", argv[1]);
    printf("Main Module: %s\n", argv[2]);
    printf("Java Home: %s\n", argv[3]);
    printf("Java Library Path: %s\n", argv[4]);
    fflush(stdout);

    // Create Java VM through JNI. Works both with the JVM library and the library built by Native Image.
    // For the JVM library the module related parameters and the java.home parameter are necessary.
    // For the library built by Native Image, these parameters are redundant, because no module loading occurs
    // on SVM used by the Native Image, everything is already built into the library.

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
    // Lookup classes needed by the examples
    t_value = (*env)->FindClass(env, "org/graalvm/polyglot/Value");
    CHECK_THROW()
    jclass t_api = (*env)->FindClass(env, "org/example/native_embedding/API");
    CHECK_THROW()


    // Lookup methods needed by the examples
    m_value_asString = (*env)->GetMethodID(env, t_value, "asString", "()Ljava/lang/String;");
    CHECK_THROW()
    m_api_setup = (*env)->GetStaticMethodID(env, t_api, "setup", "()V");
    CHECK_THROW()
    m_api_tearDown = (*env)->GetStaticMethodID(env, t_api, "tearDown", "()V");
    CHECK_THROW()
    m_api_helloWithCallback = (*env)->GetStaticMethodID(env, t_api, "helloWithCallback", "(J)V");
    CHECK_THROW()
    m_api_fibonacci = (*env)->GetStaticMethodID(env, t_api, "fibonacci", "(I)J");
    CHECK_THROW()
    m_api_eval = (*env)->GetStaticMethodID(env, t_api, "eval", "(Ljava/lang/String;)V");
    CHECK_THROW()
}
