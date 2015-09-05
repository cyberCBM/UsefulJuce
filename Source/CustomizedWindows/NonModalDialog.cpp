#include "NonModalDialog.h"

NonModalWindow::NonModalWindow(const String& name,
    const Colour& backgroundColour,
    int requiredButtons,
    bool addToDesktop,
    boost::function< void() > hideCallback)
    : DocumentWindow(name,
    backgroundColour,
    requiredButtons,
    addToDesktop)
    , m_hideCallback(hideCallback)
{
    setTitleBarTextCentred(false);
    addKeyListener(this);
    setSize(400, 200);
    setResizable(true, true);
    setResizeLimits(100, 100, 2000, 1200);
}

NonModalWindow::~NonModalWindow()
{
}

void NonModalWindow::closeButtonPressed()
{
    if (m_hideCallback) {
        m_hideCallback();
    }
    setVisible(false);
}

bool NonModalWindow::keyPressed(const KeyPress & key, Component * /*originatingComponent*/)
{
    if (key == KeyPress::escapeKey) {
        if (m_hideCallback) {
            m_hideCallback();
        }
        setVisible(false);
    }
    return true;
}