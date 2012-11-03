#ifndef DEVICE_H
#define DEVICE_H

#include <QProcess>
#include <QDebug>
#include <QWSServer>

class Device
{
public:
    static Device& instance()
    {
        static Device instance;
        return instance;
    }

    typedef enum MODEL {
        UNKNOWN = 0,
        EMULATOR, // qvfb qws environment
        K2,       // Kindle 2
        KDX,      // Kindle DX(G)
        K3,       // Kindle Keyboard
        K4NT,     // Kindle Non-Touch Silver 2011
        K4NTB,    // Kindle Non-Touch Black 2012
        KT,       // Kindle Touch
        KPW       // Kindle PaperWhite
    } Model;

    static Model getModel() {
        return m_model;
    }

    static int getWidth() {
        return WIDTH[m_model];
    }

    static int getHeight() {
        return HEIGHT[m_model];
    }

    static bool hasKeyboard() {
        return KEYBOARD[m_model];
    }

    static bool hasFiveWay() {
        return FIVE_WAY[m_model];
    }

    static bool isTouch() {
        return !FIVE_WAY[m_model] && !KEYBOARD[m_model];
    }

    static bool hasLight() {
        return m_model == KPW;
    }

    static bool isEmulator() { return m_model == EMULATOR; }

    static void suspendFramework(bool fast = false) {
        qDebug("- framework");
        if (!isTouch()) {
            // this pause lets CVM handle painting before stopping, or screensaver may not draw
            // on next resume when device is turned off
            sleep(1);
            QProcess::execute("killall -STOP cvm");
        } else {
            QProcess::execute(QString("/bin/sh ./ktsuspend.sh %1").arg(fast ? 1 : 0));
        }
        QWSServer::instance()->enablePainting(true);
    }

    static void resumeFramework(bool fast = false) {
        qDebug("+ framework");
        QWSServer::instance()->enablePainting(false);
        if (!isTouch()) {
            QProcess::execute("killall -CONT cvm");
        } else {
            QProcess::execute(QString("/bin/sh ./ktresume.sh %1").arg(fast ? 1 : 0));
        }
    }

private:
    static const int WIDTH[], HEIGHT[];
    static const bool KEYBOARD[];
    static const bool FIVE_WAY[];

    static MODEL m_model;

    Device();
    Device(Device const&);
    void operator=(Device const&);
};

#endif // DEVICE_H
