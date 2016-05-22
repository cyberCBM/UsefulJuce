#include "stdafx.h"
#include "ProgressBarComponent.h"
ProgressBarComponent::ProgressBarComponent(uint32_t progMin, uint32_t progMax, bool bCancelButtonNeeded)
    : m_progMin(progMin)
    , m_progMax(progMax)
    , m_currProg(0)
    , m_cancelBtn(bCancelButtonNeeded ? std::make_unique<juce::TextButton>(): nullptr)
    , m_progressBar(std::make_unique<juce::ProgressBar>(m_currProg))
    , m_bIsModal(false)
{
    addAndMakeVisible(m_progressBar.get());
    if (m_cancelBtn)
    {
        auto& tblnf = AUVID_CORE().GetLookAndFeelInterface()->GetTextButtonLookAndFeel();
        addAndMakeVisible(m_cancelBtn.get());
        m_cancelBtn->setButtonText("Cancel");
        m_cancelBtn->setColour(TextButton::buttonColourId, Colour(0xFF2C91D8));
        m_cancelBtn->setColour(TextButton::buttonOnColourId, Colour(0xFF2C91D8));
        m_cancelBtn->setColour(TextButton::textColourOnId, Colour(0xFFFFFFFF));
        m_cancelBtn->setColour(TextButton::textColourOffId, Colour(0xFFFFFFFF));
        m_cancelBtn->setLookAndFeel(&tblnf);
        m_cancelBtn->addListener(this);
    }
  
}

void ProgressBarComponent::setFunctionToExecuteInBG(functionWithProgressCallback func)
{
    auto progFun = [this] (uint32_t progress)
    {
        auto messageProgFun = [this, progress]()
        {
            setProgressValue(progress);
        };
        //make sure the progress reporting function
        //is executed on message thread.
        MessageManager::callAsync(messageProgFun);
    };

    auto task = [progFun, func] ()
    {
        func(progFun);
    };
    
    // add task to async executioner or background thread
}

void ProgressBarComponent::resized()
{
    auto bounds = getLocalBounds();
    if (m_cancelBtn)
    {
        m_progressBar->setBounds(bounds.getX(), bounds.getY(), bounds.getWidth() / 2, bounds.getHeight());
        m_cancelBtn->setBounds(m_progressBar->getRight() + 5, bounds.getY(), bounds.getWidth() / 2 - 10, bounds.getHeight());
    }
    else
        m_progressBar->setBounds(bounds);
}

void ProgressBarComponent::setProgressValue(uint32_t progress)
{
    m_currProg = static_cast<double>(progress - m_progMin) / (m_progMax - m_progMin);
}


void ProgressBarComponent::setFunctionToExecuteInBG(luabind::object const  func, luabind::object const progFun)
{
    auto task = [=] ()
    {
        return luabind::call_function<bool>(func, progFun);
    };

    auto after = [=]()
    {
        MessageManager::callAsync([=]()
        {
            closeDialog(1);
        });
    };

    m_task = std::make_shared<Au::BasicBackgroundTask>(task, nullptr, after); 
	// add task to async executioner or background thread which should be cancleable 
    
}


void ProgressBarComponent::showPercantageDisplay(bool show)
{
	m_progressBar->setPercentageDisplay(show);
}

void ProgressBarComponent::setTextToDisplay(const String & str)
{
	m_progressBar->setTextToDisplay(str);
}

void ProgressBarComponent::setBackGroundColour(const Colour color)
{
	m_progressBar->setColour(ProgressBar::backgroundColourId, color);
	m_progressBar->repaint();
}

void ProgressBarComponent::setForeGroundColour(const Colour color)
{
	m_progressBar->setColour(ProgressBar::foregroundColourId, color);
	m_progressBar->repaint();
}

void ProgressBarComponent::buttonClicked(juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == m_cancelBtn.get())
    {
        if (m_task)
            m_task->cancelTask();
        closeDialog(0);
    }
}

void ProgressBarComponent::closeDialog(int returnState)
{
    if (m_bIsModal)
    {
        if (juce::DialogWindow* dw = findParentComponentOfClass<juce::DialogWindow>())
            dw->exitModalState(returnState);
    }
    else
    {
        setVisible(false);
    }
}

int ProgressBarComponent::showModal(Component* componentToCenterAround, String & title, const int width, const int height)
{
    m_bIsModal = true;
    setSize(width, height);
   return  juce::DialogWindow::showModalDialog(title, this, componentToCenterAround, juce::Colours::red, false);
}

