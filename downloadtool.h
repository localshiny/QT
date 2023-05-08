

#pragma once

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QPointer>
#include <QApplication>

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <memory>

#define DOWNLOAD_DEBUG

class DownloadTool : public QObject
{
    Q_OBJECT

public:

    explicit DownloadTool(const QString& downloadUrl, const QString& savePath, QObject* parent = nullptr);
    ~DownloadTool();

    void startDownload();
    void cancelDownload();

Q_SIGNALS:
    void sigProgress(qint64 bytesRead, qint64 totalBytes, qreal progress);
    void processFinishSignal(int);

private Q_SLOTS:
    void httpFinished();
    void httpReadyRead();

    void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);  

private:
    void startRequest(const QUrl& requestedUrl);
    std::unique_ptr<QFile> openFileForWrite(const QString& fileName);

private:
    QString m_downloadUrl;
    QString m_savePath;

    const QString defaultFileName = "tmp";

    QUrl url;
    QNetworkAccessManager qnam;
    QPointer<QNetworkReply> reply;
    std::unique_ptr<QFile> file;
    bool httpRequestAborted;
};

