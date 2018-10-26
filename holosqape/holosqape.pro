QT += quick
QT += websockets
CONFIG += c++11 debug_and_release
CONFIG -= qtquickcompiler

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS +=

INCLUDEPATH += ../bindings

# Set globals
win32:HC_PATH = ../../holochain-rust
else:HC_PATH = ../../holosqape/holochain-rust
HC_TARGET_PATH = $$HC_PATH/target
# Set target dir
CONFIG(debug, debug|release) {
    TARGET_DIR = debug
} else {
    TARGET_DIR = release
}

# Define holochain lib path
android:LIBS += -L$$HC_TARGET_PATH/armv7-linux-androideabi/release/
win32:LIBS += -L$$HC_TARGET_PATH/$$TARGET_DIR/
else:LIBS += -L$$HC_TARGET_PATH/$$TARGET_DIR/

# Define bindings path
win32:LIBS += -L../bindings/$$TARGET_DIR
else:LIBS += -L../bindings

# Add dependencies
LIBS += -lbindings -lholochain_dna_c_binding -lholochain_core_api_c_binding
# Windows specific
!win32 {
    LIBS += -ldl
}
win32 {
    LIBS += -lws2_32 -lshell32 -lDbghelp -lUserenv -lAdvapi32
}
# Mac specific
macx:CONFIG(debug, debug|release) {
    LIBS += -framework Security
}

message(holosqape: LIBS = $$LIBS)

android {
    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
