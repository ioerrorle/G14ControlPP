#ifndef KBDLISTENERTHREAD_H
#define KBDLISTENERTHREAD_H

#include <QThread>
#include <windows.h>

class KbdListenerThread : public QThread
{
    Q_OBJECT
public:
    explicit KbdListenerThread(QObject *parent = nullptr);
    ~KbdListenerThread();
    bool init(HANDLE kbdHandle, QString &error);
    void quit();

protected:
    void run() override;

private:
    HANDLE m_kbdHandle = NULL;
    HANDLE m_kbdEventHandle = NULL;

    HANDLE m_quitEventHandle = NULL;

    void emitKeyPress(const unsigned char *cc, int nCount);

signals:
    void keyPressed(const uchar &keycode);
    void kbdError(const QString &error);

};

#endif // KBDLISTENERTHREAD_H
