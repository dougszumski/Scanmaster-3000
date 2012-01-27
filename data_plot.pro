#  Scanmaster-3000 STM aqusition software
#
#  Copyright 2011, 2012 Doug Szumski <d.s.szumski@gmail.com>
#
#  This file is part of Scanmaster-3000.
#
#    Scanmaster-3000 is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Scanmaster-3000 is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with scanmaster-3000.  If not, see <http://www.gnu.org/licenses/>.
#
#  Scanmaster-3000 is based in part on the work of
#           the Qwt project (http://qwt.sf.net).

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
