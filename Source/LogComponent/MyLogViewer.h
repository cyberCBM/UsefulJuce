/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  1 Apr 2012 5:46:28pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MyLogVIEWER_MyLogVIEWER_DDEA711A__
#define __JUCER_HEADER_MyLogVIEWER_MyLogVIEWER_DDEA711A__

//[Headers]     -- You can add your own extra header files here --
#include "MyLog.h"

class MyLogViewer;
class DataIOLogViewer;

////////////////////////////////////////////////////////////////////////////////////////////////////
class LogHolderComponent : public Component {
public:
    LogHolderComponent();
    ~LogHolderComponent();
    void paint(Graphics &g);
    void resized();
    MyLogViewer& getCurrentLogViewer() {
        return (*(MyLogViewer*)logTabs->getCurrentContentComponent());
    }

    void changeLogTab(int tabIndex);

private:
    TabbedComponent* logTabs;
    MyLogViewer * detailedLogViewer;
    MyLogViewer * mainLogViewer;
    DataIOLogViewer* dataIoLogViewer;
};

class MyLogViewerTokeniser : public CodeTokeniser {
public:
  MyLogViewerTokeniser();
  ~MyLogViewerTokeniser();
  int readNextToken(CodeDocument::Iterator &source);
  CodeEditorComponent::ColourScheme getDefaultColourScheme();
  JUCE_LEAK_DETECTOR(MyLogViewerTokeniser);
};
////////////////////////////////////////////////////////////////////////////////////////////////////

//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/

class MyLogViewer : public Component,
                       public MyLog::Listener,
                       public ButtonListener {
public:
  //==============================================================================
  MyLogViewer(const String &name,
      int _logTarget, int _tabIndex, 
      LogHolderComponent & _ownerLogHolder);
  ~MyLogViewer();

  //==============================================================================
  //[UserMethods]     -- You can add your own custom methods in this section.
  enum ToolbarItems { _none, clearLog };
  void messageLogged(MyLog::MyLogMessage message);
  AuViD::API::LogTarget getLogTarget() { return (AuViD::API::LogTarget)m_logTarget; }
  String getContentName() { return ("Log viewer"); }
  void clearLogs();
  void saveLogs();

  /** ButtonListener interface */
  void buttonClicked(Button *button);
  //[/UserMethods]

  void paint(Graphics &g);
  void resized();

  //==============================================================================
  juce_UseDebuggingNewOperator

private :
    //[UserVariables]   -- You can add your own custom variables in this
    // section.
    CodeDocument m_logDocument;
    MyLogViewerTokeniser m_logViewerTokeniser;
    AuViD::API::LogTarget m_logTarget;

    ScopedPointer<ImageButton> m_saveImageButton;
    ScopedPointer<ImageButton> m_clearImageButton;
    LogHolderComponent & m_ownerLogHolder;
    int m_tabIndex;
    //[/UserVariables]

  //==============================================================================
    CodeEditorComponent *m_pLogEditor;
    JUCE_LEAK_DETECTOR(MyLogViewer)

  //==============================================================================
  // (prevent copy constructor and operator= being generated..)
    MyLogViewer(const MyLogViewer &);
    const MyLogViewer &operator=(const MyLogViewer &);
};

class DataIOLogViewer : public MyLogViewer, public SliderListener {
public:
    DataIOLogViewer(const String &name,
        int _logTarget, int _tabIndex,
        LogHolderComponent & _ownerLogHolder);

    /** Component interface */
    void resized();

    /** ButtonListener interface */
    void buttonClicked(Button *button);

    /** SliderListener interface */
    void sliderValueChanged(Slider* slider);

private:
	ToggleButtonLookAndFeel tgblnf;
	ScopedPointer<ToggleButton>   readRegisterToggleButton;
  ScopedPointer<ToggleButton>   intermediatePacketToggleButton;
  ScopedPointer<ToggleButton>   limitPayloadToggleButton;
  ScopedPointer<Slider>         payloadLimitSlider;

  JUCE_LEAK_DETECTOR(DataIOLogViewer)
};

#endif // __JUCER_HEADER_MyLogVIEWER_MyLogVIEWER_DDEA711A__
