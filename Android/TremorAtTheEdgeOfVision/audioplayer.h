#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QString>
#include <QTimer>

#include "audiochannel.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    //
    //
    //
    static AudioPlayer& shared() {
        static AudioPlayer instance;
        return instance;
    }

public slots:
    void reload();
    void play(const QString& filename);
    void stop(const QString& filename);
    void stopAll();
    void stopAllExcept(const QString& filename);
    bool isPlaying(const QString& filename);
    void updateChannels();
//signals:
private:
    //
    // singleton
    //
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();
    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;
    AudioPlayer(AudioPlayer&&) = delete;
    AudioPlayer& operator=(AudioPlayer&&) = delete;
    //
    //
    //
    QMap<QString,AudioChannel*>  m_channels;
    QTimer                      m_updateTimer;

};

#endif // AUDIOPLAYER_H
