#pragma once
#include <QObject>
#include <QImage>
#include <QStringList>
#include <QUrl>
#include <QSize>

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList files READ files NOTIFY filesChanged)
    Q_PROPERTY(int index READ index NOTIFY indexChanged)
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY indexChanged)
    Q_PROPERTY(QUrl currentUrl READ currentUrl NOTIFY indexChanged)
    Q_PROPERTY(QString outputDir READ outputDir WRITE setOutputDir NOTIFY outputDirChanged)
    Q_PROPERTY(QString kindleDir READ kindleDir WRITE setKindleDir NOTIFY kindleDirChanged)
    Q_PROPERTY(int targetWidth READ targetWidth WRITE setTargetWidth NOTIFY targetChanged)
    Q_PROPERTY(int targetHeight READ targetHeight WRITE setTargetHeight NOTIFY targetChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(int zoom READ zoom NOTIFY transformChanged)
    Q_PROPERTY(int angle READ angle NOTIFY transformChanged)
    Q_PROPERTY(int cropLeft READ cropLeft NOTIFY cropChanged)
    Q_PROPERTY(int cropTop READ cropTop NOTIFY cropChanged)
    Q_PROPERTY(int cropWidth READ cropWidth NOTIFY cropChanged)
    Q_PROPERTY(int cropHeight READ cropHeight NOTIFY cropChanged)
public:
    explicit Backend(QObject *parent=nullptr);
    QStringList files() const{return m_files;} int index() const{return m_index;}
    QString currentPath() const{return m_index<m_files.size()?m_files[m_index]:QString();}
    QUrl currentUrl() const{return currentPath().isEmpty()?QUrl():QUrl::fromLocalFile(currentPath());}
    QString outputDir() const{return m_output;} void setOutputDir(const QString&);
    QString kindleDir() const{return m_kindle;} void setKindleDir(const QString&);
    int targetWidth() const{return m_width;} int targetHeight() const{return m_height;} void setTargetWidth(int v){setResolution(v,m_height);} void setTargetHeight(int v){setResolution(m_width,v);}
    QString message() const{return m_message;}
    int zoom()const{return m_zoom;} int angle()const{return m_angle;}
    Q_INVOKABLE void setZoom(int); Q_INVOKABLE void setAngle(int);
    int cropLeft() const; int cropTop() const; int cropWidth() const; int cropHeight() const;
    Q_INVOKABLE void loadFolder(const QUrl&); Q_INVOKABLE void setResolution(int,int);
    Q_INVOKABLE QImage preview() const;
    Q_INVOKABLE void moveCrop(int,int); Q_INVOKABLE void finish(); Q_INVOKABLE void skip(); Q_INVOKABLE void undo();
    Q_INVOKABLE void chooseOutput(const QUrl&); Q_INVOKABLE void chooseKindle(const QUrl&); Q_INVOKABLE void pushToKindle();
signals: void filesChanged(); void indexChanged(); void outputDirChanged(); void kindleDirChanged(); void targetChanged(); void messageChanged(); void cropChanged(); void transformChanged();
private: void setMessage(QString); QStringList m_files; int m_index=0,m_width=1072,m_height=1448,m_x=0,m_y=0,m_zoom=100,m_angle=0; QString m_output,m_kindle,m_message; struct Action{int index;QString outputPath;bool skipped;}; QList<Action> m_history;
};
