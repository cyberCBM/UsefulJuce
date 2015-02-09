#ifndef __VIEW_LOG_WINDOW__
#define __VIEW_LOG_WINDOW__

#include "../JuceLibraryCode/JuceHeader.h"

class LogComponent : public Component, 
                    public TableListBoxModel, 
                    public ButtonListener,
                    public ComboBoxListener
{
public :
    LogComponent();
    virtual ~LogComponent();

    /** component interface */
    void		resized();
    void        paint (Graphics & g);
    /** ListBoxModel interface */
    int			getNumRows();
    void		paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected);
    void		paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);

    /** ButtonListener interface */
    void buttonClicked (Button* buttonThatWasClicked);

    /** ComboBoxListener interface */
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);

    /** Class method*/
    void AddInLogArray(const String & strMessage);
    void SaveLogFile(File & logFile, bool bAppend);
    void UpdateLogFilterArray(const String & strMessage = String::empty, const bool bClearAndUpdate = false);
protected:
    TableListBox *  m_pLogTableListBox;
    TextButton  *   m_pSaveTextButton;
    TextButton *    m_pClearTextButton;
    StringArray     m_logStringArray;
    StringArray     m_logFilterStringArray;
    String          sLogString;
    Label *         m_pFilterLabel;
    ComboBox *      m_pFilterCombobox;

    juce::ScopedPointer< juce::TextButton >     m_logDirBrowseBtn;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogComponent)
    
};

#endif  // __VIEW_LOG_WINDOW__
