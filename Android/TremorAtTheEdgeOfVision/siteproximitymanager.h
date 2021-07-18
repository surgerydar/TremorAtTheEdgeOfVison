#ifndef SITEPROXIMITYMANAGER_H
#define SITEPROXIMITYMANAGER_H

#include <QObject>
#include <QBasicTimer>

#include "sitelist.h"
class SiteProximityManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool offsite READ offsite NOTIFY offsiteChanged)
    Q_PROPERTY(bool isLocated READ isLocated NOTIFY isLocatedChanged)
    Q_PROPERTY(bool denied READ denied NOTIFY deniedChanged)
    Q_PROPERTY(QString nowPlayingAudio READ nowPlayingAudio WRITE setNowPlayingAudio NOTIFY nowPlayingAudioChanged)
    Q_PROPERTY(QString activeSequence READ activeSequence NOTIFY activeSequenceChanged)
    Q_PROPERTY(qreal activeHorizontalOffset READ activeHorizontalOffset NOTIFY activeHorizontalOffsetChanged)
    Q_PROPERTY(qreal shortest READ shortest NOTIFY shortestChanged)
public:
    //
    //
    //
    static SiteProximityManager& shared() {
        static SiteProximityManager instance;
        return instance;
    }
    //
    //
    //
    bool offsite() const {
        return m_offsite;
    }
    bool isLocated() const {
        return m_isLocated;
    }
    bool denied() const {
        return m_denied;
    }
    QString nowPlayingAudio() const {
        return m_nowPlayingAudio;
    }
    void setNowPlayingAudio( const QString& nowPlaying ) {
        m_nowPlayingAudio = nowPlaying;
    }
    QString activeSequence() {
        return m_activeSite.sequence;
    }
    qreal activeHorizontalOffset() {
        return m_activeSite.horizontalOffset;
    }
    qreal shortest() {
        return m_lastShortest;
    }
    //
    //
    //
    void timerEvent(QTimerEvent *) override;

public slots:
    void start();
    void stop();
    void sitePlayed();

signals:
    void activeSequenceChanged(const QString& sequence);
    void activeHorizontalOffsetChanged(const qreal horizontalOffset);
    void activeSiteChanged(const SiteList::Site& activeSite);
    void offsiteChanged(const bool offsite);
    void isLocatedChanged(const bool isLocated);
    void deniedChanged(const bool denied);
    void nowPlayingAudioChanged(const QString& filename);
    void shortestChanged(const qreal shortest);

private:
    //
    // singleton
    //
    explicit SiteProximityManager(QObject *parent = nullptr);
    ~SiteProximityManager();
    SiteProximityManager(const SiteProximityManager&) = delete;
    SiteProximityManager& operator=(const SiteProximityManager&) = delete;
    SiteProximityManager(SiteProximityManager&&) = delete;
    SiteProximityManager& operator=(SiteProximityManager&&) = delete;
    //
    //
    //
    void reset();
    //
    //
    //
    QBasicTimer         m_timer;
    bool                m_offsite = false;
    bool                m_isLocated = false;
    bool                m_denied = false;
    SiteList::Site      m_activeSite;
    SiteList::Site      m_lastPlayedSite;
    QString             m_nowPlayingAudio;
    qreal               m_lastShortest;
    bool                m_activated;
    bool                m_activating;
    qreal               m_activationTimer;
};

#endif // SITEPROXIMITYMANAGER_H
