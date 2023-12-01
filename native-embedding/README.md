# Polyglot Native Embedding Demo with GraalVM for JDK 21 
Demonstration project showing polyglot native embedding with GraalVM JDK 21 using Maven.
It contains a simple API class that is built into a native libary and then accessed in a C program.
The C program runs both with the JVM library and the self-contained library built by Native Image
as a part of this project. 

## Setup

[Download](https://www.graalvm.org/downloads/) GraalVM and point the `JAVA_HOME` environment variable to it.

## Maven Usage

Download Maven or import as Maven project into your IDE.

* `mvn package` to build the native embedding library by Native Image.
* `mvn -Pbuild-and-run-tests` to build the library and also run the C program that tests the API exposed by the library.
* `mvn -P-build-native-embedding-library -Pbuild-and-run-tests` to run the test without building the library.

Please see the [pom.xml](./pom.xml) file for further details on the configuration.