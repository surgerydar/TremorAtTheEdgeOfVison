#include "audiochannel.h"
#include <QUrl>
#include <QDebug>
#include "systemutils.h"

AudioChannel::AudioChannel(QObject *parent) : QObject(parent) {
    m_loop = false;
    m_targetVolume = 0.0;
    m_player.setVolume(0);
    connect(&m_player,&QMediaPlayer::stateChanged,[=](QMediaPlayer::State state) {
        if ( state == QMediaPlayer::StoppedState && m_loop && m_targetVolume > 0.5 ) {
            m_player.play();
        }
    });
}

void AudioChannel::load( const QString& path, bool loop ) {
    QUrl url(path);
    m_loop = loop;
    m_targetVolume = 0.0;
    qDebug() << "AudioChannel::load : loading : " << url;
    m_player.setMedia(url);
}
void AudioChannel::play() {
    m_targetVolume = 1.0;
    m_player.play();
}
void AudioChannel::stop() {
    m_targetVolume = 0.0;
}
bool AudioChannel::isPlaying() {
    qreal currentVolume = qreal(m_player.volume()) / 100.0;
    return m_player.state() == QMediaPlayer::PlayingState && (currentVolume > m_targetVolume || currentVolume < m_targetVolume);
}
bool AudioChannel::update() { // returns true if player is done ie player is stopped or volume is 0.0
    //const qreal increment = 1.0 / ( 5000.0 / 10.0 );
    if ( isPlaying() ) {
        //qreal currentVolume = qreal(m_player.volume()) / 100.0;
        int currentVolume = m_player.volume();
        //qDebug() << "AudioChannel::update : currentVolume=" << currentVolume;
        if ( m_targetVolume > currentVolume / 100.0 ) {
            m_player.setVolume(currentVolume+1);
        } else if ( m_targetVolume < currentVolume / 100.0 ) {
            //qDebug() << "AudioChannel::update : currentVolume=" << ( currentVolume / 100.0 );
            m_player.setVolume(currentVolume-1);
        } else if ( m_targetVolume <= 0.0 ) {
            //qDebug() << "AudioChannel::update : stopping";

            m_player.stop();
            return true;
        }
        return false;
    }
    return true;
}
