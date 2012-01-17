include( examples.pri )
LIBS += -L/opt/fontconfig-2.4.2/lib
LIBS += -lnidaqmxbase
LIBS += /usr/local/lib/LabVIEW-2010/patchlib/libstdc++.so.6

# Include qwt libs -- avoid putting them in lib?
LIBS += -I/usr/local/qwt-5.1.2/lib
TARGET = data_plot

# Include the qwt headers
INCLUDEPATH += /usr/local/qwt-5.1.2/include/

# Include the NI libs in rpath
QMAKE_LFLAGS += -Wl,-rpath,/usr/local/lib/LabVIEW-2010/patchlib
QMAKE_INCPATH += -I/usr/local/natinst/nidaqmxbase/include
HEADERS = data_plot.h \
    hist_plot.h \
    histogram_item.h \
    main.h \
    para.h
SOURCES = data_plot.cpp \
    main.cpp \
    hist_plot.cpp \
    histogram_item.cpp \
    para.cpp
FORMS += 
