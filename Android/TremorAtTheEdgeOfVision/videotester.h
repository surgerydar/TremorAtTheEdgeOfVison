#ifndef VIDEOTESTER_H
#define VIDEOTESTER_H

#include <QObject>
#include "videotexture.h"

class VideoTester : public QObject
{
    Q_OBJECT
public:
    explicit VideoTester(QObject *parent = nullptr);

    VideoTester& shared() {
        static VideoTester instance;
        return instance;
    }
signals:

public slots:
    void open(const QUrl& url ) {
        m_texture.open(url);
    }
private:
    VideoTexture m_texture;
};

#endif // VIDEOTESTER_H
