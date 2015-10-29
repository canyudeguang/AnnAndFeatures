TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
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
    lib_features/lbpfeatures.cpp \
    lib_classifiers/ann.cpp \
    lib_classifiers/classifier.cpp \
    lib_classifiers/svm.cpp \
    lib_classifiers/boostclass.cpp \
    lib_classifiers/kn.cpp \
    lib_classifiers/decisiontree.cpp \
    lib_features/brightfeature.cpp \
    lib_features/featurespicker.cpp \
    lib_classifiers/myann.cpp \
    mainold.cpp \
    lib_classifiers/myannsettings.cpp \
    lib_features/integralfeature.cpp \
    lib_classifiers/labelextractor.cpp

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
    lib_features/lbpfeatures.h \
    lib_classifiers/ann.h \
    lib_classifiers/classifier.h \
    lib_classifiers/svm.h \
    lib_classifiers/boostclass.h \
    lib_classifiers/kn.h \
    lib_classifiers/decisiontree.h \
    lib_features/brightfeature.h \
    lib_features/featurespicker.h \
    lib_classifiers/myann.h \
    lib_classifiers/myannsettings.h \
    lib_features/integralfeature.h \
    lib_classifiers/labelextractor.h

