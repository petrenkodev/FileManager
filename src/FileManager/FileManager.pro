include( ../../common.pri )

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    action.cpp \
    fsutils.cpp \
    main.cpp \
    mainwindow.cpp \
    makedirdialog.cpp \
    panelview.cpp \
    propertiesdialog.cpp \
    selectdelegate.cpp \
    sortproxy.cpp

HEADERS += \
    action.h \
    fsutils.h \
    mainwindow.h \
    makedirdialog.h \
    panelview.h \
    propertiesdialog.h \
    selectdelegate.h \
    sortproxy.h

FORMS += \
    mainwindow.ui \
    makedirdialog.ui \
    panelview.ui \
    propertiesdialog.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
    icons/about.ico \
    icons/app.png \
    icons/copy.ico \
    icons/copy_to_folder.ico \
    icons/cut.ico \
    icons/delete.ico \
    icons/exit.ico \
    icons/go-home.ico \
    icons/help.ico \
    icons/home.ico \
    icons/move.ico \
    icons/new_folder.ico \
    icons/paste.ico \
    icons/stock_exit.ico

DESTDIR = $${BIN_PATH}/
