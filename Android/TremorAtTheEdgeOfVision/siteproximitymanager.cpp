#include "siteproximitymanager.h"
#include "sitelist.h"
#include "locationmanager.h"
#include "audioplayer.h"

#include <QDebug>

const qreal activationDistance = 5.0;
const qreal deactivationDistance = 7.0;
const qreal activationDelay = 5.0;
const qreal offsiteDistance = 1000.0;
const int updateDelay = 1000 / 60;
const qreal activationDelayDecrement = 1.0 / 60.0;

const SiteList::Site nosite = {
    "",
    { 0.0, 0.0 },
    "",
    "",
    "",
    0.0
};

SiteProximityManager::SiteProximityManager(QObject *parent) : QObject(parent) {
    m_offsite = true;
    m_isLocated = false;
    m_activeSite = m_lastPlayedSite = nosite;

}
SiteProximityManager::~SiteProximityManager() {

}
void SiteProximityManager::timerEvent(QTimerEvent *) {
    if ( m_activating && m_activationTimer > 0 ) {
        m_activationTimer -= activationDelayDecrement;
        return;
    }
    //
    // find closest
    //
    SiteList::Site closest = nosite;
    qreal shortest = std::numeric_limits<qreal>::max();
    for ( auto& site : SiteList::shared().m_siteList ) {
        //auto distance = LocationManager::shared().distanceTo(site.coordinate);
        auto distance = LocationManager::shared().filteredDistanceTo(site.coordinate);
        if ( distance > 0.0 ) { // negative is an error
            if ( distance < shortest ) {
                shortest = distance;
                if ( shortest <= activationDistance ) {
                    closest = site;
                }
            }
        }
    }


    if ( LocationManager::shared().tracking() && shortest > -1.0 ) {
        if ( m_lastShortest >= 0 ) {
            qreal difference = fabs(shortest - m_lastShortest);
            if( difference > deactivationDistance * 2.0 ) {
                qDebug() << "SiteProximityManager::timerEvent : unreliable distance=" << shortest << " difference=" << difference;
                // reject large jumps
                //return;
                if ( difference > deactivationDistance * 4.0 ) {
                    m_lastShortest = shortest;
                }
                return;
            } else {
                //shortest = m_lastShortest + difference * 0.1;
            }
        }
        m_lastShortest = shortest;
        emit shortestChanged(shortest);

        //qDebug() << "SiteProximityManager::timerEvent : shortest=" << shortest;
        if ( !m_isLocated ) {
            m_isLocated = true;
            emit isLocatedChanged(true);
        }
        if ( shortest < offsiteDistance && m_offsite ) {
            m_offsite = false;
            emit offsiteChanged(m_offsite);
            AudioPlayer::shared().play("In_Blue_background_track_AAC.m4a");
        } else if ( shortest >= offsiteDistance && !m_offsite ) {
            m_offsite = true;
            emit offsiteChanged(m_offsite);
            AudioPlayer::shared().stopAll();
        }
        if ( closest.id == nosite.id ) {
            if ( m_activeSite.id != nosite.id && shortest > deactivationDistance ) {
                qDebug() << "SiteProximityManager::timerEvent : deactivating active site=" << m_activeSite.id;
                sitePlayed();
            } else if ( m_lastPlayedSite.id != nosite.id && shortest > deactivationDistance ) {
                qDebug() << "SiteProximityManager::timerEvent : deactivating last played site=" << m_lastPlayedSite.id;
                m_activated = false;
                m_lastPlayedSite = nosite;
            }
            return;
        } else if ( m_activated ) {
            return;
        }

        if ( !m_activating ) {
            qDebug() << "SiteProximityManager::timerEvent : activating : " << closest.notes;
            m_activating = true;
            m_activationTimer = activationDelay;
            AudioPlayer::shared().stopAllExcept("environmental_curtain_up_AAC.m4a");
            AudioPlayer::shared().play("environmental_curtain_up_AAC.m4a");
        } else if ( m_activeSite.id == nosite.id || closest.id != m_activeSite.id ) {
            m_activated = true;
            m_activating = false;
            m_activeSite = m_lastPlayedSite = closest;
            m_nowPlayingAudio = "";
            AudioPlayer::shared().stopAll();
            emit activeSiteChanged(m_activeSite);
            emit activeSequenceChanged(m_activeSite.sequence);
            emit nowPlayingAudioChanged(m_nowPlayingAudio);
        }
    }
}
void SiteProximityManager::start() {
    m_offsite = true;
    m_isLocated = false;
    m_lastShortest = -1;
    m_activated = false;
    m_lastPlayedSite = nosite;
    reset();
    m_timer.start(updateDelay,this);
}
void SiteProximityManager::stop() {
    m_timer.stop();
    AudioPlayer::shared().stopAll();
}
void SiteProximityManager::sitePlayed() {
    if ( m_activeSite.id != nosite.id ) {
        qDebug() << "SiteProximityManager::sitePlayed : " << m_activeSite.id;
        reset();
    } else {
        qDebug() << "SiteProximityManager::sitePlayed : no active site";
    }
}
//
//
//
void SiteProximityManager::reset() {
    QString audio = m_activeSite.outro;
    m_activating = false;
    m_activeSite = nosite;
    if ( !m_offsite ) {
        AudioPlayer::shared().play(audio.length() > 0 ? audio : "In_Blue_background_track_AAC.m4a");
    }

    emit activeSiteChanged(m_activeSite);
    emit activeSequenceChanged("");
    //emit nowPlayingAudioChanged(m_nowPlayingAudio);
}
