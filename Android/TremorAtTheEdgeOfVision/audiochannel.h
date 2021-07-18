#ifndef AUDIOCHANNEL_H
#define AUDIOCHANNEL_H

#include <QObject>
#include <QMediaPlayer>

class AudioChannel : public QObject
{
    Q_OBJECT
public:
    AudioChannel(QObject *parent = nullptr);
    void load( const QString& path, bool loop = true );
    void play();
    void stop();
    bool isPlaying();
    bool update();
//signals:

private:
    QMediaPlayer m_player;
    bool m_loop;
    qreal m_targetVolume;
};

#endif // AUDIOCHANNEL_H
