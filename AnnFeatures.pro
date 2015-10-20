TEMPLATE = app
CONFIG += console
CONFIG += c++11
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
    lib_features/lbpfeatures.cpp \
    lib_features/integralfeature.cpp \
    lib_features/brightfeature.cpp \
    lib_features/pointyfeature.cpp \
    lib_detector/detector.cpp \
    lib_stasm/stasm_lib.cpp \
    lib_stasm/stasm.cpp \
    lib_stasm/startshape.cpp \
    lib_stasm/shapemod.cpp \
    lib_stasm/shapehacks.cpp \
    lib_stasm/shape17.cpp \
    lib_stasm/print.cpp \
    lib_stasm/pinstart.cpp \
    lib_stasm/misc.cpp \
    lib_stasm/landmarks.cpp \
    lib_stasm/hatdesc.cpp \
    lib_stasm/hat.cpp \
    lib_stasm/faceroi.cpp \
    lib_stasm/eyedist.cpp \
    lib_stasm/eyedet.cpp \
    lib_stasm/err.cpp \
    lib_stasm/convshape.cpp \
    lib_stasm/classicdesc.cpp \
    lib_stasm/asm.cpp \
    lib_stasm/MOD_1/facedet.cpp \
    lib_stasm/MOD_1/initasm.cpp \
    lib_features/siftfeatures.cpp \
    lib_features/surffeatures.cpp \
    lib_features/fastfeatures.cpp \
    lib_features/geffeatures.cpp \
    lib_features/testfeature.cpp

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
-lopencv_nonfree \
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
    lib_features/lbpfeatures.h \
    lib_features/integralfeature.h \
    lib_features/brightfeature.h \
    lib_features/pointyfeature.h \
    lib_detector/detector.h \
    lib_stasm/stasm_landmarks.h \
    lib_stasm/stasm.h \
    lib_stasm/stasm_lib_ext.h \
    lib_stasm/stasm_lib.h \
    lib_stasm/startshape.h \
    lib_stasm/shapemod.h \
    lib_stasm/shapehacks.h \
    lib_stasm/shape17.h \
    lib_stasm/print.h \
    lib_stasm/pinstart.h \
    lib_stasm/misc.h \
    lib_stasm/landtab_muct77.h \
    lib_stasm/landmarks.h \
    lib_stasm/hatdesc.h \
    lib_stasm/hat.h \
    lib_stasm/faceroi.h \
    lib_stasm/eyedist.h \
    lib_stasm/eyedet.h \
    lib_stasm/err.h \
    lib_stasm/convshape.h \
    lib_stasm/classicdesc.h \
    lib_stasm/basedesc.h \
    lib_stasm/atface.h \
    lib_stasm/asm.h \
    lib_stasm/MOD_1/initasm.h \
    lib_stasm/MOD_1/facedet.h \
    lib_features/siftfeatures.h \
    lib_features/surffeatures.h \
    lib_features/fastfeatures.h \
    lib_features/geffeatures.h \
    lib_features/testfeature.h

