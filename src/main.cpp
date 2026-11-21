#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include "backend.h"
class PreviewProvider: public QQuickImageProvider { public: explicit PreviewProvider(Backend *b):QQuickImageProvider(QQuickImageProvider::Image),backend(b){} QImage requestImage(const QString&,QSize *size,const QSize&) override {QImage im=backend->preview();if(size)*size=im.size();return im;} Backend *backend; };
#include "backend.h"
int main(int argc,char**argv){QGuiApplication app(argc,argv);app.setApplicationName("Kindlewall");app.setDesktopFileName("kindlewall");Backend backend;QQmlApplicationEngine engine;engine.addImageProvider("preview",new PreviewProvider(&backend));engine.rootContext()->setContextProperty("backend",&backend);engine.load(QUrl("qrc:/Main.qml"));if(engine.rootObjects().isEmpty())return 1;return app.exec();}
