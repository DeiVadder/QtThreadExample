
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    workerobject.cpp

HEADERS += \
    mainwindow.h \
    workerobject.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target