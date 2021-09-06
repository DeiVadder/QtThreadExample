
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    workerthread.cpp

HEADERS += \
    mainwindow.h \
    workerthread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

