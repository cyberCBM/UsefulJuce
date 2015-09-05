#ifndef __FeedbackComponent__H__
#define __FeedbackComponent__H__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
//[/Headers]
#include"RatingComponent.h"

enum FeedbackType
{
	smile,
	frown,
	bug
};

class FeedbackComponent  : public Component,
                      public ButtonListener,
					  public TextEditor::Listener
{
public:
    FeedbackComponent(Image & image, FeedbackType _fType);
    ~FeedbackComponent();
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
	void textEditorTextChanged(TextEditor&);
    void copyFile(const String& sourceFileName);
    void sendData(const StringArray& dataInArray);
private:

    ScopedPointer<TextEditor>       commentsTextEditor;
	ScopedPointer<Label>            commentsLabel;
    ScopedPointer<TextButton>       submitTextButton;
	ScopedPointer<Label>            emailLabel;
    ScopedPointer<TextEditor>       emailidTextEditor;
	ScopedPointer<GroupComponent>   imageGroup;
    ScopedPointer<ToggleButton>     snapshotToggleButton;
	ScopedPointer<ToggleButton>     emailIDtoggleButton;
	ScopedPointer<ImageComponent>   imageComponent;
	ScopedPointer<Label>            ratingLabel;
	ScopedPointer<RatingComponent>  ratingComponent;
	Image                           snapshotImage;
    FeedbackType                    fType;
    String                          userName;
	ToggleButtonLookAndFeel tbnlnf;
	TextButtonLookAndFeel tblnf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeedbackComponent)
};

#endif   // __FeedbackComponent__H__
