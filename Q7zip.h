#ifndef Q7ZIP_H
#define Q7ZIP_H


/*
 * uesr windows execute 7z.exe in qt qrc file.
 * Only Windows is currently supported.
 *
 * build taget in the Pro file director,
 * case changed target build path in Qt Pro File:
 *
 *_______________Q7zip.pro___________________
 *
 * DESTDIR += $$PWD
 *___________________________________________
 *
 *
 *
 * user method in main function:
 *____________________________________________
 * Q7zip zip;
 * zip.setOutToConselo();
 * zip.inCompress("D:\\outtest.log","D:\\log.7z");
 * zip.unCompress("D:\\log.7z","D:\\log");
 *____________________________________________
 *
 */


#include "q7zip_global.h"
#include <QQueue>
#include <QTimer>
#include <QProcess>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

struct QRC_INFO
{
    QString FilePath;
    QString ExeName;
    QString DllName;
}static __QRC_INFO{":/7zExecute/","7z.exe","7z.dll"};

static QString __AppBkdFilePath="/Tool//";



class Q7ZIPSHARED_EXPORT PrivateProcessQueue:public QObject,public QQueue<QProcess*>
{
    Q_OBJECT
public:
    static PrivateProcessQueue* getInstance()
    {
        return s_ProcessQueue;
    }

    static QTimer* getTimerHeadler()
    {
        return s_queryHeadler;
    }

private:
    PrivateProcessQueue(){}
    ~PrivateProcessQueue(){}
    static PrivateProcessQueue* s_ProcessQueue;
    static QTimer* s_queryHeadler;
    QProcess* m_currProcess = nullptr;

    void executeFirstProcess()
    {
        m_currProcess = this->at(0);
        qDebug()<<"\nPID:"<<m_currProcess->pid()
               <<"\nArgument:"<<m_currProcess->arguments();

        connect(m_currProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [=](int exitCode, QProcess::ExitStatus exitStatus)
        {

            qDebug()<<"\nexitCode:"<<exitCode
                  <<"\nexitStatus:"<<exitStatus;

            m_currProcess->close();
            m_currProcess = nullptr;

        });
        pop_front();
        m_currProcess->start();
    }

signals:
    void SIG_ProcessInQueue();
    void SIG_ProcessOutQueue();

public slots:
    void SLT_ExeAllProcess()
    {
        connect(PrivateProcessQueue::getTimerHeadler(),&QTimer::timeout,[=](){
            if(!m_currProcess && !isEmpty())
            {
                executeFirstProcess();
            }
        });
        PrivateProcessQueue::getTimerHeadler()->start(20);
    }
};

class Q7ZIPSHARED_EXPORT Q7zip: public QObject
{
    Q_OBJECT
public:
    Q7zip()
    {
        m_CurrApplictionDir = QCoreApplication::applicationDirPath();
        check7zExecute();
    }

    void inCompress(QString srcFilePath,QString archive)
    {
        QProcess * process7z = new QProcess();
        process7z->setProgram(m_CurrApplictionDir+__AppBkdFilePath+__QRC_INFO.ExeName);
        process7z->setWorkingDirectory(m_CurrApplictionDir+__AppBkdFilePath);
        process7z->setArguments({"a","-bd",archive,srcFilePath});

        if(m_ProcessChannelMode == QProcess::ProcessChannelMode::ForwardedChannels)
        {
            connect(process7z,&QProcess::readyReadStandardOutput,[=](){
                qDebug()<<process7z->readAllStandardOutput().data();
            });
        }

        if(PrivateProcessQueue::getTimerHeadler() == nullptr)
        {
            qDebug()<<__FUNCTION__<<"ProcessQueue Headler is nullptr";
            return;
        }
        else if(!PrivateProcessQueue::getTimerHeadler()->isActive())
        {
            PrivateProcessQueue::getInstance()->SLT_ExeAllProcess();
        }
        PrivateProcessQueue::getInstance()->push_back(process7z);
    }

    void unCompress(QString archive,QString dstFilePath)
    {

        QProcess * process7z = new QProcess();
        process7z->setProgram(m_CurrApplictionDir+__AppBkdFilePath+__QRC_INFO.ExeName);
        process7z->setWorkingDirectory(m_CurrApplictionDir+__AppBkdFilePath);
        process7z->setArguments({"x","-y","-bd",archive,QString("-o%1").arg(dstFilePath)});

        if(m_ProcessChannelMode == QProcess::ProcessChannelMode::ForwardedChannels)
        {
            connect(process7z,&QProcess::readyReadStandardOutput,[=](){
                qDebug()<<process7z->readAllStandardOutput().data();
            });
        }

        if(PrivateProcessQueue::getTimerHeadler() == nullptr)
        {
            qDebug()<<__FUNCTION__<<"ProcessQueue Headler is nullptr";
            return;
        }
        else if(!PrivateProcessQueue::getTimerHeadler()->isActive())
        {
            PrivateProcessQueue::getInstance()->SLT_ExeAllProcess();
        }
        PrivateProcessQueue::getInstance()->push_back(process7z);
    }

    void setOutToConselo()
    {
        qDebug()<<__FUNCTION__;
        m_ProcessChannelMode = QProcess::ProcessChannelMode::ForwardedChannels;
    }

private:

    QProcess::ProcessChannelMode m_ProcessChannelMode;
    QString m_CurrApplictionDir;

    void check7zExecute()
    {
        if(!m_CurrApplictionDir.isEmpty()
                && ! __QRC_INFO.DllName.isEmpty()
                && ! __QRC_INFO.ExeName.isEmpty()
                && ! __QRC_INFO.FilePath.isEmpty()
                && ! __AppBkdFilePath.isEmpty())
        {
            QDir ToolDir = QDir(m_CurrApplictionDir+__AppBkdFilePath);
            if(!ToolDir.exists())
            {
                if(ToolDir.mkpath(ToolDir.path()))
                {
                    QString srcFile = __QRC_INFO.FilePath + __QRC_INFO.ExeName,
                            desFile = ToolDir.path() + __QRC_INFO.ExeName;

                    QFile file;
                    if(!file.exists(desFile))
                    {
                        if(!file.copy(srcFile,desFile))
                            qDebug()<<__FUNCTION__<<">>"<<QString("failed,copy qrc file %0>>%1").arg(srcFile,desFile);
                    }
                    srcFile = __QRC_INFO.FilePath + __QRC_INFO.DllName;
                    desFile = ToolDir.path() + __QRC_INFO.DllName;
                    if(!file.exists(desFile))
                    {
                        if(!file.copy(srcFile,desFile))
                            qDebug()<<__FUNCTION__<<">>"<<QString("failed,copy qrc file %0>>%1").arg(srcFile,desFile);
                    }
                }
            }
        }
    }
};

#endif // Q7ZIP_H

//7-Zip 18.06 (x64) : Copyright (c) 1999-2018 Igor Pavlov : 2018-12-30

//Usage: 7z <command> [<switches>...] <archive_name> [<file_names>...]

//<Commands>
//  a : Add files to archive
//  b : Benchmark
//  d : Delete files from archive
//  e : Extract files from archive (without using directory names)
//  h : Calculate hash values for files
//  i : Show information about supported formats
//  l : List contents of archive
//  rn : Rename files in archive
//  t : Test integrity of archive
//  u : Update files to archive
//  x : eXtract files with full paths

//<Switches>
//  -- : Stop switches parsing
//  @listfile : set path to listfile that contains file names
//  -ai[r[-|0]]{@listfile|!wildcard} : Include archives
//  -ax[r[-|0]]{@listfile|!wildcard} : eXclude archives
//  -ao{a|s|t|u} : set Overwrite mode
//  -an : disable archive_name field
//  -bb[0-3] : set output log level
//  -bd : disable progress indicator
//  -bs{o|e|p}{0|1|2} : set output stream for output/error/progress line
//  -bt : show execution time statistics
//  -i[r[-|0]]{@listfile|!wildcard} : Include filenames
//  -m{Parameters} : set compression Method
//    -mmt[N] : set number of CPU threads
//    -mx[N] : set compression level: -mx1 (fastest) ... -mx9 (ultra)
//  -o{Directory} : set Output directory
//  -p{Password} : set Password
//  -r[-|0] : Recurse subdirectories
//  -sa{a|e|s} : set Archive name mode
//  -scc{UTF-8|WIN|DOS} : set charset for for console input/output
//  -scs{UTF-8|UTF-16LE|UTF-16BE|WIN|DOS|{id}} : set charset for list files
//  -scrc[CRC32|CRC64|SHA1|SHA256|*] : set hash function for x, e, h commands
//  -sdel : delete files after compression
//  -seml[.] : send archive by email
//  -sfx[{name}] : Create SFX archive
//  -si[{name}] : read data from stdin
//  -slp : set Large Pages mode
//  -slt : show technical information for l (List) command
//  -snh : store hard links as links
//  -snl : store symbolic links as links
//  -sni : store NT security information
//  -sns[-] : store NTFS alternate streams
//  -so : write data to stdout
//  -spd : disable wildcard matching for file names
//  -spe : eliminate duplication of root folder for extract command
//  -spf : use fully qualified file paths
//  -ssc[-] : set sensitive case mode
//  -sse : stop archive creating, if it can't open some input file
//  -ssw : compress shared files
//  -stl : set archive timestamp from the most recently modified file
//  -stm{HexMask} : set CPU thread affinity mask (hexadecimal number)
//  -stx{Type} : exclude archive type
//  -t{Type} : Set type of archive
//  -u[-][p#][q#][r#][x#][y#][z#][!newArchiveName] : Update options
//  -v{Size}[b|k|m|g] : Create volumes
//  -w[{path}] : assign Work directory. Empty path means a temporary directory
//  -x[r[-|0]]{@listfile|!wildcard} : eXclude filenames
//  -y : assume Yes on all queries
