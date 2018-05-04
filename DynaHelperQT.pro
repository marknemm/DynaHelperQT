#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T10:52:50
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DynaHelperQT
TEMPLATE = app

INCLUDEPATH += C:/opencv/build/include \
               C:/opencv/build/include/opencv2 \
               C:/opencv/build/include/opencv

LIBS += -LC:/opencv/build/x86/vc12/lib \
    opencv_core249.lib \
    opencv_highgui249.lib \
    opencv_imgproc249.lib \
    opencv_features2d249.lib \
    opencv_calib3d249.lib \
    opencv_objdetect249.lib \
    opencv_flann249.lib \
    opencv_nonfree249.lib

SOURCES += main.cpp\
    hotkey/GlobalHotKeyThreadWin.cpp \
    hotkey/GlobalHotKeyMonitor.cpp \
    io_logging/IOLoggerWin.cpp \
    io_logging/IOLogger.cpp \
    view/macro_menu/SelectionsTable.cpp \
    view/macro_menu/CustomDialog.cpp \
    view/io_logging/IOLoggingInfo.cpp \
    controller/io_logging/IOLoggingController.cpp \
    io_logging/IOLoggerPlatform.cpp \
    model/Model.cpp \
    model/DBUtil.cpp \
    controller/Controller.cpp \
    model/MacroMetaModel.cpp \
    model/MacroEventModel.cpp \
    record_img/BoundsChecker.cpp \
    record_img/ImageConverter.cpp \
    record_img/ImagePreprocessing.cpp \
    record_img/ImageSearcher.cpp \
    record_img/ImageTextAndIconAnalyzer.cpp \
    record_img/NativeImageUtil.cpp \
    record_img/RecordImageUtil.cpp \
    record_img/RecordImgDbg.cpp \
    record_img/TargetImageExtractor.cpp \
    io_logging/IOUtil.cpp \
    model/MacroEvent.cpp \
    controller/macro_activation/MacroActivationController.cpp \
    macro_activation/MacroActivator.cpp \
    view/macro_editor/MacroEditor.cpp \
    controller/macro_editor/MacroEditorController.cpp \
    view/macro_editor/MacroEventsTable.cpp \
    view/macro_editor/MacroIdsLabel.cpp \
    view/macro_menu/MacroMenu.cpp \
    controller/macro_menu/MacroMenuController.cpp \
    view/macro_editor/QTableWidgetNumberDelegate.cpp \
    view/macro_editor/MacroEditorContextMenu.cpp \
    view/macro_menu/MacroMenuContextMenu.cpp \
    model/proxy/MacroEventEditProxy.cpp \
    model/proxy/MacroEventLogBuffer.cpp \
    controller/macro_add/MacroAddController.cpp

HEADERS  += \
    hotkey/GlobalHotKeyThreadWin.h \
    hotkey/GlobalHotKeyThreadPlatform.h \
    hotkey/GlobalHotKeyMonitor.h \
    io_logging/IOLoggerPlatform.h \
    io_logging/IOLoggerWin.h \
    io_logging/IOLogger.h \
    view/macro_menu/MacroMenu.h \
    view/macro_menu/SelectionsTable.h \
    view/macro_menu/CustomDialog.h \
    controller/Controller.h \
    view/io_logging/IOLoggingInfo.h \
    controller/io_logging/IOLoggingController.h \
    io_logging/IOLoggerListener.h \
    model/Model.h \
    model/DBUtil.h \
    model/MacroEvent.h \
    model/MacroMetaModel.h \
    model/MacroMetadata.h \
    model/MacroEventModel.h \
    record_img/BoundsChecker.h \
    record_img/ImageConverter.h \
    record_img/ImagePreprocessing.h \
    record_img/ImageSearcher.h \
    record_img/ImageTextAndIconAnalyzer.h \
    record_img/NativeImageUtil.h \
    record_img/RecordImageUtil.h \
    record_img/RecordImgDbg.h \
    record_img/TargetImageExtractor.h \
    io_logging/IOUtil.h \
    controller/macro_activation/MacroActivationController.h \
    macro_activation/MacroActivator.h \
    util/ProducerConsumerQueue.hpp \
    controller/macro_menu/MacroMenuEventListener.h \
    view/macro_editor/MacroEditor.h \
    controller/macro_editor/MacroEditorController.h \
    view/macro_editor/MacroEventsTable.h \
    view/macro_editor/MacroIdsLabel.h \
    controller/macro_menu/MacroMenuController.h \
    controller/macro_editor/MacroEditorEventListener.h \
    view/macro_editor/QTableWidgetNumberDelegate.h \
    view/macro_editor/MacroEditorContextMenu.h \
    view/macro_menu/MacroMenuContextMenu.h \
    model/proxy/MacroEventEditProxy.h \
    model/proxy/MacroEventEdit.h \
    util/ChangeLog.hpp \
    model/proxy/MacroEventLogBuffer.h \
    controller/macro_add/MacroAddController.h \
    model/proxy/MacroEventEdit.h

FORMS    += view/macro_menu/MacroMenu.ui \
    view/macro_menu/CustomDialog.ui \
    view/io_logging/IOLoggingInfo.ui \
    view/macro_editor/MacroEditor.ui

RESOURCES += \
    resources/MacroMenuResources.qrc

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
