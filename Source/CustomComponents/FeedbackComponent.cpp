#include "stdafx.h"
#include "FeedbackComponent.h"
#include "Poco/File.h"


const String FEEDBACK_URL = "ServerName";
const String FEEDBACK_IMAGE_PATH = "ServerLocation";

const String COMMENT        =       "Comment";
const String EMAILID        =       "EmailId";
const String FEEDBACKTYPE   =       "FeedbackType";
const String URATING        =       "UserRating";
const String IMAGENAME      =       "ImageName";
const String AUVIDVERSION   =       "AuvidVersion";

//==============================================================================
FeedbackComponent::FeedbackComponent (Image & image, FeedbackType _fType)
: fType(_fType), snapshotImage(image)
{
	String infolabeltext="We appreciate your feedback.";
	String buttontext = "Send a ";
    if (fType == smile)
	{
		infolabeltext += " What did you like?";
		buttontext += "Smile :)";
	}
    else if (fType == frown)
	{
		infolabeltext += " Is there something we can do better?";
		buttontext += "Frown :(";
	}

	addAndMakeVisible(commentsLabel = new Label("new label",infolabeltext));
	commentsLabel->setFont(Font(15.00f, Font::plain));
	commentsLabel->setJustificationType(Justification::centredLeft);
	commentsLabel->setEditable(false, false, false);
	commentsLabel->setColour(Label::textColourId, Colours::white);
	commentsLabel->setColour(Label::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible (commentsTextEditor = new TextEditor ("new text editor"));
    commentsTextEditor->setMultiLine (false);
    commentsTextEditor->setReturnKeyStartsNewLine (false);
    commentsTextEditor->setReadOnly (false);
    commentsTextEditor->setScrollbarsShown (true);
    commentsTextEditor->setCaretVisible (true);
    commentsTextEditor->setPopupMenuEnabled (true);
    commentsTextEditor->setText (String::empty);
	commentsTextEditor->setMultiLine(true);
	commentsTextEditor->addListener(this);
	commentsTextEditor->setColour(TextEditor::textColourId, Colours::white);
	commentsTextEditor->setColour(TextEditor::backgroundColourId, Colour(0xFF364760));
	commentsTextEditor->setTextToShowWhenEmpty("Write feedback to enable submit", Colours::lightgrey);

	addAndMakeVisible(submitTextButton = new TextButton("Submit"));
	submitTextButton->addListener(this);
	submitTextButton->setColour(TextButton::buttonColourId, Colour(0xFF2C91D8));
	submitTextButton->setColour(TextButton::buttonOnColourId, Colour(0xFF2C91D8));
	submitTextButton->setColour(TextButton::textColourOnId, Colour(0xFFFFFFFF));
	submitTextButton->setColour(TextButton::textColourOffId, Colour(0xFFFFFFFF));
	submitTextButton->setEnabled(false);
	submitTextButton->setLookAndFeel(&tblnf);

	addAndMakeVisible (emailidTextEditor = new TextEditor ("new text editor"));
    emailidTextEditor->setMultiLine (false);
    emailidTextEditor->setReturnKeyStartsNewLine (false);
    emailidTextEditor->setReadOnly (false);
    emailidTextEditor->setScrollbarsShown (true);
    emailidTextEditor->setCaretVisible (true);
    emailidTextEditor->setPopupMenuEnabled (true);
	emailidTextEditor->setColour(TextEditor::textColourId, Colours::white);
	emailidTextEditor->setColour(TextEditor::backgroundColourId, Colour(0xFF364760));
    
    std::string uname;
    char buffer[1024];
    unsigned long bufferSize = 1024;
    memset(buffer, 0x00, 1024);
    if (GetUserNameA(buffer, &bufferSize) == TRUE) {
        uname = std::string(buffer, bufferSize);
    }
    userName = String(uname) + "@audience.com";
    emailidTextEditor->setText(userName);

    addAndMakeVisible (snapshotToggleButton = new ToggleButton ("new toggle button"));
    snapshotToggleButton->setButtonText (TRANS("Include Screenshot"));
    snapshotToggleButton->addListener (this);
	snapshotToggleButton->setColour(ToggleButton::textColourId,Colours::white);
    snapshotToggleButton->setToggleState(true, false);
	snapshotToggleButton->setLookAndFeel(&tbnlnf);
	
    addAndMakeVisible(emailIDtoggleButton = new ToggleButton("new toggle button"));
	emailIDtoggleButton->setButtonText(TRANS("Include Email Address"));
	emailIDtoggleButton->addListener(this);
	emailIDtoggleButton->setLookAndFeel(&tbnlnf);
	emailIDtoggleButton->setColour(ToggleButton::textColourId,Colours::white);
    emailIDtoggleButton->setToggleState(true, false);
	
    addAndMakeVisible(imageGroup=new GroupComponent());
	imageGroup->addAndMakeVisible(imageComponent=new ImageComponent());
    imageComponent->setImage(snapshotImage);
	imageGroup->setText("");
	//imageGroup->setLookAndFeel(&lookandfeel);
	imageComponent->setEnabled(false);
	
    addAndMakeVisible(ratingLabel = new Label("new label", "RATE US"));
	ratingLabel->setFont(Font(15.00f, Font::bold));
	ratingLabel->setJustificationType(Justification::centredLeft);
	ratingLabel->setEditable(false, false, false);
	ratingLabel->setColour(Label::textColourId, Colours::white);
	ratingLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	
    addAndMakeVisible(ratingComponent = new RatingComponent(5));
    
    setSize (490, 590);
}

FeedbackComponent::~FeedbackComponent()
{
	commentsTextEditor = nullptr;
    submitTextButton = nullptr;
    emailidTextEditor = nullptr;
    snapshotToggleButton = nullptr;
}


void FeedbackComponent::paint (Graphics& g)
{
	g.fillAll(Colour(0xFF263446));
}

void FeedbackComponent::resized()
{
	commentsLabel->setBounds(20, 16, 450, 24);
	commentsTextEditor->setBounds(20, 40, 450, 120);
    snapshotToggleButton->setBounds (20, 170, 350, 25);
	imageGroup->setBounds(20, 195, 450, 250);
	imageComponent->setBounds(10,10,imageGroup->getWidth()-20,imageGroup->getHeight()-20);
	emailidTextEditor->setBounds(225, 465, 245, 25);
	emailIDtoggleButton->setBounds(20,465,200,30);
	submitTextButton->setBounds(325, 550, 150, 25);
	ratingComponent->setBounds(200,505,150,30);
	ratingLabel->setBounds(215,520,200,30);
}

void FeedbackComponent::buttonClicked (Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == submitTextButton)
    {
        // need to send following data to mpt server
        String imagename = "AuViD" + Time::getCurrentTime().toString(true, true, true, true) + ".png";
        imagename = imagename.replace(":", "_");
        copyFile(imagename);

        StringArray arrayOfdataToSend;
        arrayOfdataToSend.add(commentsTextEditor->getText());
        arrayOfdataToSend.add(emailidTextEditor->getText());
        arrayOfdataToSend.add(String(fType));
        arrayOfdataToSend.add(String(ratingComponent->getratting()));
        arrayOfdataToSend.add(imagename);
        String auvidVersion = GetManager()->getInstanceName();
        auvidVersion += STR_PRODUCT_VERSION;
        arrayOfdataToSend.add(auvidVersion);
        sendData(arrayOfdataToSend);
        
        // close 
        if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>())
			dw->exitModalState(1);
		
    }
    else if (buttonThatWasClicked == snapshotToggleButton)
    {
        imageComponent->setEnabled(snapshotToggleButton->getToggleState());
    }
	else if (buttonThatWasClicked == emailIDtoggleButton)
	{
        if (emailIDtoggleButton->getToggleState())
        {
            emailidTextEditor->setText(userName);
        }
        else
        {
            emailidTextEditor->setText("Anonymous");
        }
        
	}
}

void FeedbackComponent::sendData(const StringArray& dataInArray)
{
    auto sendTask = [dataInArray]()-> void
    {
        try {
            juce::URL mptUrl(FEEDBACK_URL);
            mptUrl = mptUrl.withParameter("opcode", "insert_stat")
                .withParameter(COMMENT, dataInArray[0])
                .withParameter(EMAILID, dataInArray[1])
                .withParameter(FEEDBACKTYPE, dataInArray[2])
                .withParameter(URATING, dataInArray[3])
                .withParameter(IMAGENAME, dataInArray[4])
                .withParameter(AUVIDVERSION, dataInArray[5]);
            juce::InputStream *stream = mptUrl.createInputStream(true);
            if (stream != nullptr)
            {
                juce::String response = stream->readString();
                delete stream;
            }
        }
        catch (...) {
            //Die silently
        }
    };
    ASYNC_EXEC()->addTask(sendTask);
}

void FeedbackComponent::copyFile(const String& sourceFileName)
{
    juce::Image image = snapshotImage;
    auto copyTask = [sourceFileName, image]()-> void
    {
        // Create Image first on local directory
        // AuViD + CurrentTime + Format
        juce::String sourceFullPath = juce::File::getSpecialLocation(juce::File::tempDirectory).getFullPathName() +
            File::separatorString + sourceFileName;
        juce::File imageFile(sourceFullPath);
        // Scoped so that FileOutputStream flush all data and write buffer to File
        {
            FileOutputStream op(imageFile);
            juce::PNGImageFormat pngimage;
            pngimage.writeImageToStream(image, op);
        }

        // need to copy this local file to NW share
        String destFilePath = FEEDBACK_IMAGE_PATH + sourceFileName;

        Poco::File sourceFile(sourceFullPath.toStdString());
        sourceFile.copyTo(destFilePath.toStdString());
        // Delete the file when copy is done
        imageFile.deleteFile();
    };
    ASYNC_EXEC()->addTask(copyTask);
}

void FeedbackComponent::textEditorTextChanged(TextEditor &txt)
{
	if (&txt == commentsTextEditor)
	{
        submitTextButton->setEnabled(commentsTextEditor->getText().isNotEmpty());
	}
}

