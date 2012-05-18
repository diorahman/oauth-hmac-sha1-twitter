#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <qdeclarative.h>

#include "twitter.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<Twitter>("Social", 1, 0, "Twitter");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/OAuthTest/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
