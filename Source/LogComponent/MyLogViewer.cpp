#include "stdafx.h"
//using storedSetting
#include "AuViD/Utility/SettingsConstants.h"

template <class Iterator> static int parseMessageType(Iterator &source) {
  source.skip();

  String level;
  AuViD::API::LogLevel ret = AuViD::API::LogLevel::Info;

  for (int i = 0; i < 8; i++) {
    level << source.nextChar();
  }

  if (source.peekNextChar() == ']') {
    ret = MyLog::stringToLevel(level);
  }

  source.skipToEndOfLine();
  return (ret);
}

MyLogViewerTokeniser::MyLogViewerTokeniser() {}
MyLogViewerTokeniser::~MyLogViewerTokeniser() {}

int MyLogViewerTokeniser::readNextToken(CodeDocument::Iterator &source) {
  int result = AuViD::API::LogLevel::Info;
  source.skipWhitespace();

  const juce_wchar firstChar = source.peekNextChar();

  switch (firstChar) {
  case '[':
    result = parseMessageType(source);
    break;

  default:
    source.skip();
    break;
  }

  return (result);
}

CodeEditorComponent::ColourScheme
MyLogViewerTokeniser::getDefaultColourScheme() {
  CodeEditorComponent::ColourScheme cs;

  for (int i = 0; i < AuViD::API::LogLevel::kLogLevels; i++) {
    cs.set(MyLog::levelToString((AuViD::API::LogLevel)i),
           MyLog::getColourForLevel((AuViD::API::LogLevel)i));
  }

  return cs;
}

MyLogViewer::MyLogViewer( const String &name,
                               int _logTarget, int _tabIndex, 
                               LogHolderComponent& _ownerLogHoder)
                               : m_pLogEditor(0), 
                               m_logTarget((AuViD::API::LogTarget)_logTarget),
                               m_tabIndex(_tabIndex), m_ownerLogHolder(_ownerLogHoder)
{
  addAndMakeVisible(
      m_pLogEditor = new CodeEditorComponent(m_logDocument, &m_logViewerTokeniser));
  m_pLogEditor->setName(L"logEditor");
  m_pLogEditor->setFont(
      Font("Consolas", 15, Font::bold));
  m_pLogEditor->setWantsKeyboardFocus(true);
  m_pLogEditor->setReadOnly(true);
  m_pLogEditor->setColour(CodeEditorComponent::backgroundColourId, 
      GetCOLOR(GUIColours::Log_backGroundColourId));
  m_pLogEditor->setColour(CodeEditorComponent::defaultTextColourId, 
      GetCOLOR(GUIColours::Log_TextColourId));
  CtrlrManager::getInstance()->getMyLog().addListener(this);

  m_saveImageButton = new ImageButton("Save");
  addAndMakeVisible(m_saveImageButton);
  m_saveImageButton->setImages(false, true, true, IMAGE(ico_file_png), 1.0f,
                            Colour(0x0), IMAGE(ico_file_png), 0.3f, Colour(0x0),
                            IMAGE(ico_file_png), 0.7f, Colour(0x0));
  m_saveImageButton->setTooltip(L"Save logs");
  m_saveImageButton->addListener(this);

  m_clearImageButton = new ImageButton("Clear");
  addAndMakeVisible(m_clearImageButton);
  m_clearImageButton->setImages(false, true, true, IMAGE(logClean_png), 1.0f,
                            Colour(0x0), IMAGE(logClean_png), 0.3f, Colour(0x0),
                            IMAGE(logClean_png), 0.7f, Colour(0x0));
  m_clearImageButton->setTooltip(L"Clear logs");
  m_clearImageButton->addListener(this);

  setSize(600, 400);
}

MyLogViewer::~MyLogViewer() {
  CtrlrManager::getInstance()->getMyLog().removeListener(this);
  deleteAndZero(m_pLogEditor);
  m_saveImageButton = nullptr;
  m_clearImageButton = nullptr;
}

void MyLogViewer::paint(Graphics &g) {
    g.fillAll(GetCOLOR(GUIColours::Log_main_backGroundColourId));
}

void MyLogViewer::resized() {
    m_saveImageButton->setBounds(getWidth() - 50, 2, 18, 18);
  m_clearImageButton->setBounds(getWidth() - 25, 2, 18, 18);
  m_pLogEditor->setBounds(0, 23, getWidth() - 0, getHeight() - 23);
}

void MyLogViewer::messageLogged(MyLog::MyLogMessage message) {
  if (message.level == AuViD::API::LogLevel::MidiIn || message.level == AuViD::API::LogLevel::MidiOut ||
      message.level == AuViD::API::LogLevel::Lua)
    return;

  m_pLogEditor->setReadOnly(false);
  m_pLogEditor->moveCaretToEnd(false);
  m_pLogEditor->insertTextAtCaret(MyLog::formatMessage(message) + "\n");
  //if (m_logTarget == AuViD::API::LogTarget::DataIO)
  //{
  //    logEditor->insertTextAtCaret("--------------------------------------------\n");
  //}
  m_pLogEditor->setReadOnly(true);
  m_ownerLogHolder.changeLogTab(m_tabIndex);
}

void MyLogViewer::buttonClicked(Button *button) {
    if (button == m_saveImageButton)
    {
        saveLogs();
    }
    else if (button == m_clearImageButton)
    {
        clearLogs();
    }
}

void MyLogViewer::clearLogs() {
  m_logDocument.replaceAllContent(String::empty);
}

void MyLogViewer::saveLogs() {
  // do save stuff
    std::string strRecentDir;
    if (CONFIG_MANAGER()->hasProperty(SettingsConstants::RECENT_OPENED_PANEL_DIR)) {
        CONFIG_MANAGER()->get< std::string >(
            SettingsConstants::RECENT_OPENED_PANEL_DIR,
            strRecentDir);
    }
    FileChooser fc("Choose Log file to save",
        File(strRecentDir), "*.txt", true);
    if (fc.browseForFileToSave(true)) {
        File chosenFile = fc.getResult();
        // this is the text they entered..
        if (chosenFile.exists()) {
            chosenFile.deleteFile();
        }
        chosenFile.appendText(m_logDocument.getAllContent());
        // To Do : get rid of it. Only one log file per App
        AU_LOG_INFO("Logs") << ("Log is saved to : " + chosenFile.getFullPathName() + " File");
    }
}

//////////////////////////////////////////////////////////////////////////

DataIOLogViewer::DataIOLogViewer(const String &name, int _logTarget, int _tabIndex,
    LogHolderComponent & _ownerLogHolder) :
    MyLogViewer(name, _logTarget, _tabIndex, _ownerLogHolder)
{
    addAndMakeVisible(readRegisterToggleButton = new ToggleButton("read Reg"));
    readRegisterToggleButton->setButtonText(TRANS("Read register request"));
	readRegisterToggleButton->setColour(ToggleButton::textColourId, Colours::white);
    readRegisterToggleButton->addListener(this);
    readRegisterToggleButton->setToggleState(false, dontSendNotification);
	readRegisterToggleButton->setLookAndFeel(&tgblnf);

    addAndMakeVisible(intermediatePacketToggleButton = new ToggleButton("Intermediate packets"));
    intermediatePacketToggleButton->setButtonText(TRANS("Intermediate packets"));
	intermediatePacketToggleButton->setColour(ToggleButton::textColourId, Colours::white);
    intermediatePacketToggleButton->addListener(this);
    intermediatePacketToggleButton->setToggleState(false, dontSendNotification);
	intermediatePacketToggleButton->setLookAndFeel(&tgblnf);

    addAndMakeVisible(limitPayloadToggleButton = new ToggleButton("Limit payloads size"));
    limitPayloadToggleButton->setButtonText(TRANS("Limit payloads size"));
	limitPayloadToggleButton->setColour(ToggleButton::textColourId, Colours::white);
    limitPayloadToggleButton->addListener(this);
    limitPayloadToggleButton->setToggleState(false, dontSendNotification);
	limitPayloadToggleButton->setLookAndFeel(&tgblnf);

    addAndMakeVisible(payloadLimitSlider = new Slider());
    payloadLimitSlider->setRange(0, 1020, 20);
    payloadLimitSlider->setValue(20.0, juce::dontSendNotification);
    payloadLimitSlider->setTooltip(translate("Set payload size limit", ""));
    payloadLimitSlider->setSliderStyle(Slider::SliderStyle::LinearBar);
	payloadLimitSlider->setColour(Slider::backgroundColourId, GetCOLOR(GUIColours::Log_Slider_backgroundColourId));
	payloadLimitSlider->setColour(Slider::thumbColourId, GetCOLOR(GUIColours::Log_Slider_thumbcolourId));
	payloadLimitSlider->setColour(Slider::textBoxTextColourId, GetCOLOR(GUIColours::Log_textBoxTextColourId));
    payloadLimitSlider->setEnabled(false);
    payloadLimitSlider->addListener(this);
}

void DataIOLogViewer::resized()
{
    readRegisterToggleButton->setBounds(1, 1, 150, 20);
    intermediatePacketToggleButton->setBounds(readRegisterToggleButton->getRight(), 1, 150, 20);
    limitPayloadToggleButton->setBounds(intermediatePacketToggleButton->getRight(), 1, 150, 20);
    payloadLimitSlider->setBounds(limitPayloadToggleButton->getRight(), 2, 150, 18);
    MyLogViewer::resized();
}

void DataIOLogViewer::buttonClicked(Button *button)
{
    if (button == readRegisterToggleButton)
    {
        AT_APP()->protocolProbe()->showRegisterReadPackets(button->getToggleState());
    }
    else if (button == intermediatePacketToggleButton)
    {
        AT_APP()->protocolProbe()->showIntermediatePackets(button->getToggleState());
    }
    else if (button == limitPayloadToggleButton)
    {
        payloadLimitSlider->setEnabled(button->getToggleState());
        AT_APP()->protocolProbe()->showLargePayloads(button->getToggleState());
        AT_APP()->protocolProbe()->setLargePayloadLimit(payloadLimitSlider->getValue());
    }
    else
    {
        MyLogViewer::buttonClicked(button);
    }
}


void DataIOLogViewer::sliderValueChanged(Slider *slider)
{
    if (payloadLimitSlider == slider)
    {
        AT_APP()->protocolProbe()->setLargePayloadLimit(slider->getValue());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogHolderComponent::LogHolderComponent()
{
    addAndMakeVisible(logTabs = new TabbedComponent(TabbedButtonBar::TabsAtTop));
	//logTabs->setColour(TabbedComponent::backgroundColourId, GetCOLOR(GUIColours::Log_backGroundColourId));
	logTabs->setColour(TabbedComponent::outlineColourId, GetCOLOR(GUIColours::Log_TabsOutlineColourId));
    logTabs->setTabBarDepth(24);
    mainLogViewer = new MyLogViewer("Main App Logs", AuViD::API::LogTarget::Command, logTabs->getNumTabs(), *this);
    logTabs->addTab("Command", GetCOLOR(GUIColours::Log_TabsbackGroundColourId), mainLogViewer, false);
    dataIoLogViewer = new DataIOLogViewer("Data I/O Logs", AuViD::API::LogTarget::DataIO, logTabs->getNumTabs(), *this);
    logTabs->addTab("Data I/O", GetCOLOR(GUIColours::Log_TabsbackGroundColourId), dataIoLogViewer, false);
    detailedLogViewer = new MyLogViewer("Detailed Logs",
        AuViD::API::LogTarget::Application /*| AuViD::API::LogTarget::Command*/, logTabs->getNumTabs(), *this);
    logTabs->addTab("Application", GetCOLOR(GUIColours::Log_TabsbackGroundColourId), detailedLogViewer, false);

    setSize(300, 700);
}
LogHolderComponent::~LogHolderComponent() {
    deleteAndZero(logTabs);
    deleteAndZero(detailedLogViewer);
    deleteAndZero(mainLogViewer);
    deleteAndZero(dataIoLogViewer);
}

void LogHolderComponent::paint(Graphics &g) {
    g.fillAll(GetCOLOR(GUIColours::App_BackgroundColourId));
}

void LogHolderComponent::resized() {
    logTabs->setBounds(0, 0, getWidth(), getHeight());
}

void LogHolderComponent::changeLogTab(int tabIndex)
{
    if ((tabIndex < logTabs->getNumTabs() || tabIndex > 0) && tabIndex != logTabs->getCurrentTabIndex())
    {
        logTabs->setCurrentTabIndex(tabIndex);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////