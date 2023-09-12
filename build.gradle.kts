import java.nio.file.*

plugins {
    `java`
    `application`
}

java {
    sourceCompatibility = JavaVersion.VERSION_21
    targetCompatibility = JavaVersion.VERSION_21
}

repositories {
    maven {
        url = uri("file:///Users/christianhumer/graal/graalvm/maven-resource-bundle-ee-23.1.0-dev")
    }
    mavenCentral()
}


group = "org.example"
version = "1.0-SNAPSHOT"
description = "embedding"
var graalVMVersion: String = "23.1.0-SNAPSHOT";
val isGraalVM = Files.exists(Paths.get("${System.getProperty("java.home")}/lib/graalvm"))

dependencies {
    implementation("org.graalvm.polyglot:polyglot:$graalVMVersion")
    implementation("org.graalvm.polyglot:js:$graalVMVersion")
    testImplementation("junit:junit:4.13.2")
}

tasks.withType<Test> {
    useJUnitPlatform()
}
application {
    mainModule.set("embedding")
    mainClass.set("org.example.embedding.Main")
}

if (!isGraalVM) {
   /*
    * Path used when running on a different JDK than GraalVM.
    * This profile may be removed if you are always running with a
    * GraalVM JDK or if you don't want to use the optimizing runtime.
    *
    * Note: Using this path unlocks experimental options and is therefore
    * not recommended for production use.
    */
    configurations {
        create("compilerClasspath") {
            extendsFrom(configurations.getByName("implementation"))
            isCanBeResolved = true
        }
    }
    dependencies {
        implementation("org.graalvm.compiler:compiler:$graalVMVersion")
    }
    application {
        val compilerDependencies = configurations.getByName("compilerClasspath")
                .filter { it.name.endsWith(".jar") }  // Filter out POMs
        applicationDefaultJvmArgs = listOf("-XX:+UnlockExperimentalVMOptions",
                "-XX:+EnableJVMCI",
                "--upgrade-module-path=${compilerDependencies.asPath}")
    }
}
