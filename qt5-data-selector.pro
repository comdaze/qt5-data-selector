QT += core widgets

CONFIG += c++11 sdk_no_version_check

TARGET = Qt5DataSelector
TEMPLATE = app

# Include directories
INCLUDEPATH += include

# Source files
SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/TimeWidget.cpp \
    src/DeviceManager.cpp \
    src/DeviceWidget.cpp \
    src/CustomButton.cpp

# Header files
HEADERS += \
    include/MainWindow.h \
    include/TimeWidget.h \
    include/DeviceManager.h \
    include/DeviceInfo.h \
    include/DeviceWidget.h \
    include/CustomButton.h

# Resources
RESOURCES += resources.qrc

# Output directory
DESTDIR = bin