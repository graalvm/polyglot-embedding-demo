# Polyglot Native Embedding Demo with GraalVM for JDK 21 
Demonstration project showing polyglot native embedding with GraalVM JDK 21 using Maven.
It contains a simple API class that is built into a native libary and then accessed in a C program.
The C program runs both with the JVM library and the self-contained library built by Native Image
as a part of this project. 

## Setup

[Download](https://www.graalvm.org/downloads/) GraalVM and point the `JAVA_HOME` environment variable to it.

## Maven Usage

Download Maven or import as Maven project into your IDE.

* `mvn verify` to run the C tests of the API using the JVM library.
* `mvn -Pnative-embedding-library package` to build the native embedding library by Native Image.
* `mvn -Pnative-embedding-library verify` to build the native embedding library by Native Image and run the C tests of the API exposed by the library.
* `mvn -Pnative-embedding-library verify -Dbuild.native.embedding.library.phase=none` to run the C tests without building the library (when the library is already built).

Please see the [pom.xml](./pom.xml) file for further details on the configuration.