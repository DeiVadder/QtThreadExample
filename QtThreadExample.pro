TEMPLATE = subdirs

SUBDIRS = AllCombined \
            NoThread \
            QtConcurrent \
            QtThreadCreate \
            SubclassQThread \
            WorkerObject \
            ConstExpr

AllCombined.subdir = projects/AllCombined
NoThread.subdir = projects/NoThread
QtConcurrent.subdir = projects/QtConcurrent
QtThreadCreate.subdir = projects/QtThreadCreate
SubclassQThread.subdir = projects/SubclassQThread
WorkerObject.subdir = projects/WorkerObject
ConstExpr.subdir = projects/ConstExpr

DISTFILES += \
    .qmake.conf \
