QT += quick
TEMPLATE = lib
CONFIG += c++11 staticlib debug_and_release

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
    container.cpp \
    app.cpp \
    socketinterface.cpp


HEADERS += \
    container.h \
    app.h \
    socketinterface.h

# Set globals
HC_PATH        = ../../holosqape/holochain-rust
HC_TARGET_PATH = $$HC_PATH/target

# Define holochain lib path
android {
    LIBS += -L$$HC_TARGET_PATH/armv7-linux-androideabi/release/
} else {
    CONFIG(debug, debug|release) {
        LIBS += -L$$HC_TARGET_PATH/debug/
        macx: LIBS += -framework Security
    } else {
        LIBS += -L$$HC_TARGET_PATH/release/
    }
}
message(Bindings: HC_LIB_PATH = $$LIBS)

# Add dependencies
LIBS += -lholochain_dna_c_binding -lholochain_core_api_c_binding
!win32 {
    LIBS += -ldl
}

#
include(vendor/vendor.pri)
