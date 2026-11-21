#include "backend.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QSettings>
#include <QImageReader>
#include <QImageWriter>
#include <QFile>
#include <QStandardPaths>
#include <QDateTime>
#include <QTransform>
#include <QPainter>
#include <QSaveFile>
#include <QImageWriter>
Backend::Backend(QObject *parent):QObject(parent){QSettings s("Kindlewall","Kindlewall");m_output=s.value("output",QDir::homePath()+"/Kindlewall").toString();m_kindle=s.value("kindle").toString();}
void Backend::setOutputDir(const QString&s){if(s.isEmpty()||m_output==s)return;m_output=s;QSettings("Kindlewall","Kindlewall").setValue("output",s);emit outputDirChanged();}
void Backend::setKindleDir(const QString&s){if(m_kindle==s)return;m_kindle=s;QSettings("Kindlewall","Kindlewall").setValue("kindle",s);emit kindleDirChanged();}
void Backend::setMessage(QString s){m_message=s;emit messageChanged();}
void Backend::loadFolder(const QUrl &url){
    QString path=url.toLocalFile();
    if(path.isEmpty() && url.isRelative()) path=url.toString();
    QFileInfo selected(path);
    if(!selected.exists() || !selected.isDir()){setMessage("Folder unavailable: "+url.toString());return;}
    m_files.clear();
    QDirIterator it(path,QDir::Files,QDirIterator::Subdirectories);
    const QStringList allowed={"jpg","jpeg","png","webp","bmp","tif","tiff"};
    while(it.hasNext()){QString file=it.next();if(allowed.contains(QFileInfo(file).suffix(),Qt::CaseInsensitive))m_files<<file;}
    m_files.sort(Qt::CaseInsensitive);m_index=0;m_x=m_y=0;
    emit filesChanged();emit indexChanged();emit cropChanged();
    if(m_files.isEmpty())setMessage("No supported images found in "+path);
    else setMessage(QString::number(m_files.size())+" images loaded");
}

void Backend::setResolution(int w,int h){if(w<1||h<1||w>20000||h>20000)return;m_width=w;m_height=h;emit targetChanged();emit cropChanged();}
QImage Backend::preview()const{if(currentPath().isEmpty())return {};QImageReader r(currentPath());r.setAutoTransform(true);QImage src=r.read();if(src.isNull())return {};if(m_angle)src=src.transformed(QTransform().rotate(m_angle),Qt::SmoothTransformation);if(m_zoom<100){int w=qRound(src.width()*100.0/m_zoom),h=qRound(src.height()*100.0/m_zoom);QImage canvas(w,h,QImage::Format_ARGB32);canvas.fill(Qt::black);QImage bg=src.scaled(w,h,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);QPainter p(&canvas);p.drawImage((w-bg.width())/2,(h-bg.height())/2,bg);QImage fg=src.scaled(qRound(src.width()*m_zoom/100.0),qRound(src.height()*m_zoom/100.0),Qt::KeepAspectRatio,Qt::SmoothTransformation);p.drawImage((w-fg.width())/2,(h-fg.height())/2,fg);src=canvas;}return src;}
void Backend::setZoom(int z){m_zoom=qBound(50,z,200);m_x=m_y=0;emit transformChanged();emit cropChanged();}
void Backend::setAngle(int a){m_angle=qBound(-180,a,180);m_x=m_y=0;emit transformChanged();emit cropChanged();}
int Backend::cropWidth()const{auto im=preview();if(im.isNull())return 0;double s=qMax(double(m_width)/im.width(),double(m_height)/im.height())*qMax(1.0,m_zoom/100.0);return qMin(im.width(),int(m_width/s));}
int Backend::cropHeight()const{auto im=preview();if(im.isNull())return 0;double s=qMax(double(m_width)/im.width(),double(m_height)/im.height())*qMax(1.0,m_zoom/100.0);return qMin(im.height(),int(m_height/s));}
int Backend::cropLeft()const{auto im=preview();int travel=im.width()-cropWidth();return travel<=0?0:qBound(0,int(double(m_x)/travel*10000),10000);}
int Backend::cropTop()const{auto im=preview();int travel=im.height()-cropHeight();return travel<=0?0:qBound(0,int(double(m_y)/travel*10000),10000);}
void Backend::moveCrop(int dx,int dy){auto im=preview();if(im.isNull())return;double s=qMax(double(m_width)/im.width(),double(m_height)/im.height())*qMax(1.0,m_zoom/100.0);int cw=qMin(im.width(),int(m_width/s)),ch=qMin(im.height(),int(m_height/s));m_x=qBound(0,m_x+dx*8,im.width()-cw);m_y=qBound(0,m_y+dy*8,im.height()-ch);emit cropChanged();}
void Backend::finish(){if(currentPath().isEmpty())return;QImage im=preview();if(im.isNull()){setMessage("Could not read image");return;}if(!QDir().mkpath(m_output)){setMessage("Cannot create output folder");return;}double scale=qMax(double(m_width)/im.width(),double(m_height)/im.height())*qMax(1.0,m_zoom/100.0);int cw=qBound(1,qMin(im.width(),int(m_width/scale)),im.width()),ch=qBound(1,qMin(im.height(),int(m_height/scale)),im.height());m_x=qBound(0,m_x,im.width()-cw);m_y=qBound(0,m_y,im.height()-ch);QImage out=im.copy(m_x,m_y,cw,ch).scaled(m_width,m_height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);QString base=QFileInfo(currentPath()).completeBaseName(),dst=m_output+"/"+base+".jpg";for(int n=2;QFile::exists(dst);++n)dst=m_output+"/"+base+"-"+QString::number(n)+".jpg";QSaveFile file(dst);if(!file.open(QIODevice::WriteOnly)){setMessage("Cannot write to output folder");return;}QImageWriter writer(&file,"jpg");writer.setQuality(94);if(!writer.write(out)||!file.commit()){file.cancelWriting();setMessage("Could not save output image");return;}m_history<<Action{m_index,dst,false};m_index++;m_x=m_y=0;emit indexChanged();emit cropChanged();setMessage("Saved "+QFileInfo(dst).fileName());}
void Backend::skip(){if(currentPath().isEmpty())return;m_history<<Action{m_index,QString(),true};m_index++;m_x=m_y=0;emit indexChanged();emit cropChanged();setMessage("Skipped image");}
void Backend::undo(){if(m_history.isEmpty())return;auto a=m_history.takeLast();if(!a.skipped&&!a.outputPath.isEmpty()&&!QFile::remove(a.outputPath)){m_history<<a;setMessage("Could not undo: saved file is unavailable");return;}m_index=a.index;m_x=m_y=0;emit indexChanged();emit cropChanged();setMessage("Undid last action");}
void Backend::chooseOutput(const QUrl&u){if(!u.isEmpty())setOutputDir(u.toLocalFile());}void Backend::chooseKindle(const QUrl&u){if(!u.isEmpty())setKindleDir(u.toLocalFile());}
void Backend::pushToKindle(){if(m_kindle.isEmpty()){setMessage("Choose your Kindle wallpapers folder first");return;}QDir().mkpath(m_kindle);QDir d(m_output);int count=0;for(auto f:d.entryList({"*.jpg","*.png"},QDir::Files)){QString dst=m_kindle+"/"+f;if(QFile::exists(dst))dst=m_kindle+"/"+QFileInfo(f).completeBaseName()+"-"+QString::number(QDateTime::currentMSecsSinceEpoch())+".jpg";if(QFile::copy(d.filePath(f),dst))count++;}setMessage("Copied "+QString::number(count)+" wallpapers to Kindle");}
