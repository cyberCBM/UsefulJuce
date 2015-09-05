#ifndef NON_MODAL_DIALOG
#define NON_MODAL_DIALOG

#include <boost/function.hpp>
#include "JuceHeader.h"


/** This class is used to show internal command window and codec view
    which is used to send API commands to connected device
*/
class NonModalWindow : public DocumentWindow,
                       private KeyListener
{
public:
    NonModalWindow(const String& name,
        const Colour& backgroundColour,
        int requiredButtons,
        bool addToDesktop = true,
        boost::function< void() > hideCallback = 0);
    virtual ~NonModalWindow();    
    void closeButtonPressed();    
    bool keyPressed(const KeyPress & key, Component * /*originatingComponent*/);
private:
    boost::function< void() > m_hideCallback;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonModalWindow)
};

#endif //NON_MODAL_DIALOG