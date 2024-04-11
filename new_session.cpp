#include "new_session.h"

int NewSession::id = 0;

NewSession::NewSession(QProgressBar *progress, QLCDNumber *lcd, QTimer *timer, Log* collection){
    progressBar = progress;
    lcdNumber = lcd;
    waitTimer = timer;

    log = collection;
    updateLCDTime();

}


bool NewSession::getPlaying() const
{
    return playing;
}

void NewSession::startSession(time_t start_time)
{
    complete = false;
    playing = true;
    // Update start_time
    this->start_time = start_time;
    // Set secondsRemaining to 5min
    secondsRemaining = 5*60;
}

void NewSession::pauseSession()
{
    playing = false;
    waitTimer->start(1000*10);
}

void NewSession::resumeSession()
{
    if (complete){
        startSession(start_time);
    }
    waitTimer->stop();
    playing = true;
}

void NewSession::stopSession()
{
    if(waitTimer->isActive()) {
        waitTimer->stop();
    }
    playing = false;
    complete = true;
}

void NewSession::timeout()
{
    playing = false;
    complete = false;
    qInfo("time OUTT!!!");
}



void NewSession::endSession()
{
    // End the session and return session log
    complete = true;
    log->addSession(id, end_time, 5.0, 8.0);
    id++;
}

void NewSession::secondUpdates()
{
    // Update values
    if (playing) {
        secondsRemaining--;
        progress = 100-std::round(float(secondsRemaining)/300*100);
    }
    updateLCDTime();
    updateProgressBar();;
}

void NewSession::updateLCDTime()
{
    int minutes = secondsRemaining / 60;
    int seconds = secondsRemaining % 60;

    // Format minutes and seconds into "MM:SS" format
    QString displayText = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

    // Update lcdNumber
    lcdNumber->display(displayText);
}

void NewSession::updateProgressBar()
{
    if (progressBar->value() != progress)
    {
        emit progressBar->setValue(progress);
    }
}

bool NewSession::getComplete() const
{
    return complete;
}


