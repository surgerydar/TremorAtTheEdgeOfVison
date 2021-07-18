#include "audioplayer.h"
#include "systemutils.h"
#include <QDebug>

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent) {
    reload();
    connect(&m_updateTimer, &QTimer::timeout, this, QOverload<>::of(&AudioPlayer::updateChannels));
}

AudioPlayer::~AudioPlayer() {
    for ( auto& channel : m_channels ) {
        channel->deleteLater();
    }
}

void AudioPlayer::reload() {
    const QStringList files = {
                "In_Blue_background_track_AAC.m4a",
                "environmental_curtain_up_AAC.m4a",
                "Scene_07_Guitar_walk_home_AAC.m4a"
            };
    for ( auto& file : files ) {
        QString filePath = SystemUtils::shared().documentPath(file);
        qDebug() << "AudioPlayer::AudioPlayer : loading : " << filePath;
        if ( SystemUtils::shared().fileExists(filePath) ) {
            if ( !m_channels.contains(file) ) {
                m_channels[file] = new AudioChannel;
            }
            m_channels[file]->load("file://" + filePath,!(file=="environmental_curtain_up_AAC.m4a"));
        } else {
            qDebug() << "AudioPlayer::AudioPlayer : unable to load file : " << filePath;
        }
    }

}

void AudioPlayer::play(const QString& filename) {
    qDebug() << "AudioPlayer::play " << filename;
    if ( m_channels.contains(filename) && !m_channels[filename]->isPlaying() ) {
        qDebug() << "AudioPlayer::play : playing : " << filename;
        m_channels[filename]->play();
        if ( !m_updateTimer.isActive() ) {
            //m_updateTimer.start(10);
            m_updateTimer.start(5000/100);
        }
    }
}
void AudioPlayer::stop(const QString& filename) {
    qDebug() << "AudioPlayer::stop " << filename;
    if ( m_channels.contains(filename) /*&& m_channels[filename]->isPlaying()*/ ) {
        m_channels[filename]->stop();
        if ( !m_updateTimer.isActive() ) {
            m_updateTimer.start(5000/100);
        }
    }
}
void AudioPlayer::stopAll() {
    qDebug() << "AudioPlayer::stopAll ";
    for ( QMap<QString, AudioChannel*>::iterator it = m_channels.begin(); it != m_channels.end(); it++ ) {
        //qDebug() << "AudioPlayer::stopAll : stopping : " << it.key();
        it.value()->stop();
    }
    if ( !m_updateTimer.isActive() ) {
        m_updateTimer.start(5000/100);
    }
}
void AudioPlayer::stopAllExcept(const QString& filename) {
    for ( QMap<QString, AudioChannel*>::iterator it = m_channels.begin(); it != m_channels.end(); it++ ) {
        if ( it.key() != filename ) {
            //qDebug() << "AudioPlayer::stopAllExcept : stopping : " << it.key();
            it.value()->stop();
        }
    }
    if ( !m_updateTimer.isActive() ) {
        m_updateTimer.start(5000/100);
    }
}
bool AudioPlayer::isPlaying(const QString& filename) {
    return m_channels.contains(filename) && m_channels[filename]->isPlaying();
}
void AudioPlayer::updateChannels() {
    bool active = false;
    for ( QMap<QString, AudioChannel*>::iterator it = m_channels.begin(); it != m_channels.end(); it++ ) {
        //qDebug() << "AudioPlayer::updateChannels : updating : " << it.key();
        if ( !it.value()->update() ) {
            active = true;
        }
    }
    if ( !active ) {
        m_updateTimer.stop();
    }
}

