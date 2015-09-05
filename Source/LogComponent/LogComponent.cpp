#include "LogComponent.h"
#ifndef _WIN32
    #include <Shlobj.h>
#endif // !_WIN32

LogComponent::LogComponent() :
    m_logDirBrowseBtn(nullptr)
{
    m_pLogTableListBox = new TableListBox("RSL Generator", this);
    m_pLogTableListBox->setColour (ListBox::outlineColourId, Colours::black);
    m_pLogTableListBox->setOutlineThickness (1);
    m_pLogTableListBox->addMouseListener(this, true);
    m_pLogTableListBox->setAutoSizeMenuOptionShown(false);
    m_pLogTableListBox->setMultipleSelectionEnabled(true);
    addAndMakeVisible( m_pLogTableListBox );
    m_pLogTableListBox->setColour(TableListBox::backgroundColourId, Colours::white);
    m_pLogTableListBox->setColour(TableListBox::textColourId, Colours::black);
    m_pLogTableListBox->getHeader().addColumn( "Time", 1, 175, 150, 200, TableHeaderComponent::visible | TableHeaderComponent::resizable);
    m_pLogTableListBox->getHeader().addColumn( "Type", 2, 200, 150, 300, TableHeaderComponent::visible | TableHeaderComponent::resizable);
    m_pLogTableListBox->getHeader().addColumn( "Description", 3, 900, 900, 1000, TableHeaderComponent::visible | TableHeaderComponent::resizable);

    addAndMakeVisible (m_pSaveTextButton = new TextButton("Save") );
    m_pSaveTextButton->setTooltip(translate("LOG_Save_TextButton_Tooltip",""));
    m_pSaveTextButton->setEnabled(false);
    m_pSaveTextButton->addListener(this);

    addAndMakeVisible (m_pClearTextButton = new TextButton("Clear") );
    m_pClearTextButton->setTooltip(translate("LOG_Clear_TextButton_Tooltip",""));
    m_pClearTextButton->setEnabled(false);
    m_pClearTextButton->addListener(this);

    addAndMakeVisible( m_pFilterLabel = new juce::Label( "m_logComboLabel", TRANS( "Select Log Filter:" )));
    m_pFilterLabel->setColour( juce::Label::textColourId, Colours::lightsteelblue );

    addAndMakeVisible( m_pFilterCombobox = new juce::ComboBox( "m_pFilterCombobox" ));
    m_pFilterCombobox->addListener(this);
    m_pFilterCombobox->addItem("All Logs", 1);
    m_pFilterCombobox->addItem("Informational logs", 2);
    m_pFilterCombobox->addItem("Errors logs", 3);
    m_pFilterCombobox->addItem("Warnings logs", 4);
    m_pFilterCombobox->setSelectedItemIndex(0, dontSendNotification);

    //// Log related 

    m_logDirBrowseBtn = new juce::TextButton(TRANS( "Open Directory" ));
    m_logDirBrowseBtn->setTooltip(translate("Open_TextButton_Tooltip",""));
    m_logDirBrowseBtn->addListener( this );
    addAndMakeVisible( m_logDirBrowseBtn );
}

LogComponent::~LogComponent()
{
    m_logStringArray.clear();
    m_logDirBrowseBtn= nullptr;
    deleteAllChildren();
}

void LogComponent::resized()
{
    m_pLogTableListBox->setBounds(1, 1, getWidth() - 2, getHeight() - 25);
    
    m_pFilterLabel->setBounds(proportionOfWidth(0.0f), getHeight()-23, 120, 22);
    m_pFilterCombobox->setBounds(m_pFilterLabel->getRight(), getHeight()-21, 150, 18);
    m_logDirBrowseBtn->setBounds(m_pFilterCombobox->getRight() + 2, getHeight()-23, 120, 22 );
    m_pSaveTextButton->setBounds(m_logDirBrowseBtn->getRight(), getHeight()-23, 100, 22);
    m_pClearTextButton->setBounds(m_pSaveTextButton->getRight(), getHeight()-23, 100, 22);
}

void LogComponent::paint (Graphics & g)
{
    // backGround Filling
    g.fillAll (Colour(0XFF003055));
}

int LogComponent::getNumRows()
{
    return m_logFilterStringArray.size();
}

void LogComponent::paintRowBackground (Graphics & g, int /*rowNumber*/, int /*width*/, int /*height*/, bool rowIsSelected)
{
    if(rowIsSelected)
        g.fillAll(Colours::lightgrey);
}

void LogComponent::paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
    StringArray logDataArr;
    String str = m_logFilterStringArray[rowNumber];
    logDataArr.addTokens(m_logFilterStringArray[rowNumber] , "+", String::empty);

    g.setColour(Colours::black);
    if(str.containsIgnoreCase("Error"))
        g.setColour(Colours::red);
    else if (str.containsIgnoreCase("Warning"))
        g.setColour(Colours::blue);
    
    jassert(columnId >= 1 && columnId <= 3);
    if (columnId >=1 && columnId <= 3)
        g.drawText(logDataArr[columnId-1], 0 , 0, width, height, Justification::left, false);
}

void LogComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == m_pFilterCombobox)
    {
        UpdateLogFilterArray( String::empty, true);
    }
}

void LogComponent::AddInLogArray(const String & strMessage)
{
    String logMessage="\t"+strMessage;
    sLogString += logMessage + "\n";
    if (!m_pClearTextButton->isEnabled())
    {
        m_pClearTextButton->setEnabled(true);
        m_pSaveTextButton->setEnabled(true);
    }
    m_logStringArray.add(logMessage);
    UpdateLogFilterArray(logMessage);
}

void LogComponent::UpdateLogFilterArray(const String & strMessage , const bool bClearAndUpdate)
{
    const String error = "Error";
    const String warning = "Warning";

    if (bClearAndUpdate)
    {
        m_logFilterStringArray.clear();
    }
    switch (m_pFilterCombobox->getSelectedItemIndex())
    {
    case 0:
        if (strMessage.isNotEmpty())
            m_logFilterStringArray.add(strMessage);
        else
            m_logFilterStringArray = m_logStringArray;
        break;
    case 1: // Informational case
        if (strMessage.isNotEmpty())
        {
            if (!strMessage.containsIgnoreCase(error) && 
                !strMessage.containsIgnoreCase(warning))
                    m_logFilterStringArray.add(strMessage);
        }
        else
        {
            m_logFilterStringArray = m_logStringArray;
            for (int i = 0 ; i < m_logFilterStringArray.size(); i++)
            {
                if (m_logFilterStringArray[i].containsIgnoreCase(error) || 
                    m_logFilterStringArray[i].containsIgnoreCase(warning))
                {
                        m_logFilterStringArray.remove(i);
                        i--;
                }
            }
        }
        break;
    case 2: // Error case
        if (strMessage.isNotEmpty())
        {
            if (strMessage.containsIgnoreCase(error) )
                m_logFilterStringArray.add(strMessage);
        }
        else
        {
            for (int i = 0 ; i < m_logStringArray.size(); i++)
            {
                if (m_logStringArray[i].containsIgnoreCase(error) )
                    m_logFilterStringArray.add(m_logStringArray[i]);
            }
        }
        break;
    case 3: // warning case
        if (strMessage.isNotEmpty())
        {
            if (strMessage.containsIgnoreCase(warning) )
                m_logFilterStringArray.add(strMessage);
        }
        else
        {
            for (int i = 0 ; i < m_logStringArray.size(); i++)
            {
                if (m_logStringArray[i].containsIgnoreCase(warning) )
                    m_logFilterStringArray.add(m_logStringArray[i]);
            }
        }
        break;
    default:
        jassertfalse;
    }
    m_pLogTableListBox->scrollToEnsureRowIsOnscreen(m_logFilterStringArray.size());
    m_pLogTableListBox->updateContent();
}

void LogComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if(buttonThatWasClicked == m_pSaveTextButton)
    {
        if(m_logStringArray.size() > 0)
        {
            juce::File OpenDir(File::getSpecialLocation(File::userDesktopDirectory).getFullPathName() );
            FileChooser fc ("Choose Log file to save", OpenDir, "*.txt", true);
            if (fc.browseForFileToSave (true))
            {
                File chosenFile = fc.getResult();
                SaveLogFile(chosenFile, false);
            }
        }
    }
    else if (buttonThatWasClicked == m_pClearTextButton)
    {
        sLogString = String::empty;
        m_pClearTextButton->setEnabled(false);
        m_pSaveTextButton->setEnabled(false);
        m_logStringArray.clear();
        UpdateLogFilterArray(String::empty, true);
    }
    else if( buttonThatWasClicked == m_logDirBrowseBtn ) 
    {
        /*ITEMIDLIST *pIDL = ILCreateFromPath("Log directory path".getCharPointer());
        if(NULL != pIDL)
        {
            SHOpenFolderAndSelectItems(pIDL, 0, 0, 0) ;
            ILFree(pIDL) ;
        }
		else
		{
			juce::String strPath = "Log directory path";
			ITEMIDLIST *pIDTempL = ILCreateFromPathW(strPath.toWideCharPointer());
			if(NULL != pIDTempL)
			{
				SHOpenFolderAndSelectItems(pIDTempL, 0, 0, 0) ;
				ILFree(pIDTempL) ;
			}
		}*/
    }
}

void LogComponent::SaveLogFile(File & logFile, bool bAppend)
{
    if(logFile.exists() && !bAppend)
    {
        logFile.deleteFile();
    }
    logFile.appendText(sLogString);
}

// ------------------------------------------------------------


