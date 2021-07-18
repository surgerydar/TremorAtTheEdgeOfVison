#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>

class Downloader : public QObject
{
    Q_OBJECT
public:
    //
    //
    //
    static Downloader& shared() {
        static Downloader instance;
        return instance;
    }

private:
    //
    // singleton
    //
    explicit Downloader(QObject *parent = nullptr);
    ~Downloader() = default;
    Downloader(const Downloader&) = delete;
    Downloader& operator=(const Downloader&) = delete;
    Downloader(Downloader&&) = delete;
    Downloader& operator=(Downloader&&) = delete;
    //
    //
    //
    QNetworkAccessManager m_manager;

signals:
    void done( const QString& source, const QString& destination );
    void error( const QString& source, const QString& destination, const QString& message );
    void progress( const QString& source, const QString& destination, quint64 total, quint64 current, const QString& message );

public slots:
    void download( const QString& url, const QString& filename );

private slots:

};

#endif // DOWNLOADER_H
