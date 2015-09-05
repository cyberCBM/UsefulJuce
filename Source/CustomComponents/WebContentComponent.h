#ifndef __WEB_DOCUMENTATION_COMPONENT__
#define __WEB_DOCUMENTATION_COMPONENT__

#include "JuceHeader.h"
#include <PluginBaseClasses/IComponentView.h>
#include "Version/CollectAuStats.h" 

//==============================================================================
class WebContentComponent    : public juce::Component,
    private juce::TextEditor::Listener,
    private juce::ButtonListener,
    public AuViD::API::PluginBases::IComponentView
{
private:
    WebContentComponent(AuViD::API::PluginBases::ComponentDetails & cpd)
        : Component(String(cpd.mapDetails["name"])),
        goButton ("Go", "Go to URL"),
          backButton ("<<", "Back"),
          forwardButton (">>", "Forward"),
          urlString(String(cpd.mapDetails["detail"]))
    {
        setOpaque (true);
        COLLECT_STAT("Web Viewer -> " + urlString.toStdString());

        // Create an address box..
        addAndMakeVisible (addressTextBox);
        addressTextBox.setTextToShowWhenEmpty ("Enter a web address, e.g. http://www.audience.com", Colours::grey);
        addressTextBox.addListener (this);
        

        // create the actual browser component
        addAndMakeVisible (webView = new juce::WebBrowserComponent());

        // add some buttons..
        addAndMakeVisible (goButton);
        goButton.addListener (this);
        addAndMakeVisible (backButton);
        backButton.addListener (this);
        addAndMakeVisible (forwardButton);
        forwardButton.addListener (this);

        if (CTRLR_VIEW_MODE)
        {
            addressTextBox.setVisible(false);
            addressTextBox.setReadOnly(true);
            goButton.setVisible(false);
            backButton.setVisible(false);
            forwardButton.setVisible(false);
        }
        
        // send the browser to a start page..
        // webView->goToURL ("http://www.juce.com"); //No need to start with default.
    }
public:

    void onOpened(void){
        openURL(urlString);
    }
    void onClosed(void){}
    void onFocusChanged(bool bGained){}
    void onRefresh(bool force){}
    static  AuViD::API::PluginBases::IComponentView* Create(AuViD::API::PluginBases::ComponentDetails & componentDetails)
    {
        return new WebContentComponent(componentDetails);
    }

    void openURL(const String & url)
    {
        addressTextBox.setText(url);
        webView->goToURL (url); 
    }

    void paint (Graphics& g) 
    {
        g.fillAll (Colours::grey);
    }

    void resized() 
    {
        webView->setBounds (10, 45, getWidth() - 20, getHeight() - 55);
        goButton.setBounds (getWidth() - 45, 10, 35, 25);
        addressTextBox.setBounds (100, 10, getWidth() - 155, 25);
        backButton.setBounds (10, 10, 35, 25);
        forwardButton.setBounds (55, 10, 35, 25);
    }

private:
    ScopedPointer<juce::WebBrowserComponent> webView;
    String urlString;
    TextEditor addressTextBox;
    TextButton goButton, backButton, forwardButton;

    void textEditorTextChanged (TextEditor&)              {}
    void textEditorEscapeKeyPressed (TextEditor&)         {}
    void textEditorFocusLost (TextEditor&)                {}

    void textEditorReturnKeyPressed (TextEditor&) 
    {
        webView->goToURL (addressTextBox.getText());
    }

    void buttonClicked (Button* b) 
    {
        if (b == &backButton)
            webView->goBack();
        else if (b == &forwardButton)
            webView->goForward();
        else if (b == &goButton)
            webView->goToURL (addressTextBox.getText());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebContentComponent);
};

#endif