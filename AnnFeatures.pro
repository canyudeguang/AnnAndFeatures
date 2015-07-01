TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    lib_ann/ann.cpp \
    lib_ann/classifier.cpp \
    lib_features/cornerfeatures.cpp \
    lib_features/edgefeatures.cpp \
    lib_features/experimentfeature.cpp \
    lib_features/facedescription.cpp \
    lib_features/featureextractor.cpp \
    lib_features/grayscalefeatures.cpp \
    lib_features/histogram.cpp \
    lib_features/histogramfeatures.cpp \
    lib_features/maskfeatures.cpp \
    lib_support/cvSupport.cpp \
    lib_support/support.cpp \
    lib_features/rawfeatures.cpp \
    lib_features/skeletfeatures.cpp \
    lib_features/hogfeatures.cpp \
    lib_features/blobfeatures.cpp \
    lib_features/lbpfeatures.cpp

INCLUDE += /usr/local/include/opencv /usr/local/include/opencv2
LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    lib_ann/ann.h \
    lib_ann/classifier.h \
    lib_features/cornerfeatures.h \
    lib_features/edgefeatures.h \
    lib_features/experimentfeature.h \
    lib_features/facedescription.h \
    lib_features/featureextractor.h \
    lib_features/grayscalefeatures.h \
    lib_features/histogram.h \
    lib_features/histogramfeatures.h \
    lib_features/maskfeatures.h \
    lib_features/mouthwidthfeature.h \
    lib_support/cvSupport.h \
    lib_support/support.h \
    lib_features/rawfeatures.h \
    lib_features/skeletfeatures.h \
    lib_features/hogfeatures.h \
    lib_features/blobfeatures.h \
    lib_features/lbpfeatures.h

