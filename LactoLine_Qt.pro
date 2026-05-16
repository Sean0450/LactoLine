QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/third_party/include
INCLUDEPATH += $$PWD/third_party/sqlite3

SOURCES += \
    Database/TasksTable.cpp \
    Date/Date.cpp \
    Dialogs/TaskDataDialog.cpp \
    Footer.cpp \
    GeneralValues/Amount.cpp \
    GeneralValues/Name.cpp \
    GeneralValues/Priority.cpp \
    GeneralValues/Progress.cpp \
    LedIndicator.cpp \
    MainMeny.cpp \
    Product.cpp \
    Tasks/CurrentShiftTaskGroup.cpp \
    Tasks/Task.cpp \
    Tasks/TaskGroup.cpp \
    Tasks/TaskWidget.cpp \
    Tasks/Tasklist.cpp \
    main.cpp \
    mainwindow.cpp \
    third_party/sqlite3/sqlite3.c \
    third_party/src/Backup.cpp \
    third_party/src/Column.cpp \
    third_party/src/Database.cpp \
    third_party/src/Exception.cpp \
    third_party/src/Savepoint.cpp \
    third_party/src/Statement.cpp \
    third_party/src/Transaction.cpp

HEADERS += \
    Color.hpp \
    Database/TasksTable.hpp \
    Date/Date.hpp \
    Dialogs/TaskDataDialog.hpp \
    Footer.hpp \
    GeneralValues/Amount.hpp \
    GeneralValues/Name.hpp \
    GeneralValues/Priority.hpp \
    GeneralValues/Progress.hpp \
    LedIndicator.hpp \
    MainMeny.hpp \
    Product.hpp \
    Resources.hpp \
    Tasks/ChangedData.hpp \
    Tasks/CurrentShiftTaskGroup.hpp \
    Tasks/Task.hpp \
    Tasks/TaskData.hpp \
    Tasks/TaskGroup.hpp \
    Tasks/TaskWidget.hpp \
    Tasks/Tasklist.hpp \
    mainwindow.hpp \
    third_party/include/SQLiteCpp/Assertion.h \
    third_party/include/SQLiteCpp/Backup.h \
    third_party/include/SQLiteCpp/Column.h \
    third_party/include/SQLiteCpp/Database.h \
    third_party/include/SQLiteCpp/Exception.h \
    third_party/include/SQLiteCpp/ExecuteMany.h \
    third_party/include/SQLiteCpp/SQLiteCpp.h \
    third_party/include/SQLiteCpp/SQLiteCppExport.h \
    third_party/include/SQLiteCpp/Savepoint.h \
    third_party/include/SQLiteCpp/Statement.h \
    third_party/include/SQLiteCpp/Transaction.h \
    third_party/include/SQLiteCpp/Utils.h \
    third_party/include/SQLiteCpp/VariadicBind.h \
    third_party/sqlite3/sqlite3.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    LactoLine_Qt_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    third_party/sqlite3/CMakeLists.txt \
    third_party/sqlite3/README.md
