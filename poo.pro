TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += sql
RC_FILE = main.rc

# Input
HEADERS += CategoryView.h \
           Dock.h \
           MainWindow.h \
           SubWindow.h \
           WrapTextEdit.h \
           docks/Biographies.h \
           docks/Habits.h \
           docks/Opinions.h \
           windows/Contacts.h \
           windows/Notes.h \
           windows/Passwords.h
SOURCES += CategoryView.cc \
           Dock.cc \
           main.cc \
           MainWindow.cc \
           SubWindow.cc \
           WrapTextEdit.cc \
           docks/Biographies.cc \
           docks/Habits.cc \
           docks/Opinions.cc \
           windows/Contacts.cc \
           windows/Notes.cc \
           windows/Passwords.cc
