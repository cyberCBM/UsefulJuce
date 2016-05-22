#pragma once

/* Use this class for the UI clients to execute a background job
asynchronously, while showing a non blocking progress bar in the UI.
The requirement for the client is to pass in a function that takes a 
callback function (a function that takes an integer as parameter) parameter,
which will be filled in by this class to update the progress. It is the 
responsibility of the client to pass in the appropriate progress value, as the
callback function does not have any bounds checking.
This class is also exposed to Lua, so that scripts can use this functionality 
as well.
*/
#include <cstdint>

class ICancellableTask;

/*Type of the function a client must provide to this class. */
using functionWithProgressCallback = std::function<void(std::function<void(uint32_t)>)>; 

/* A usage of the code:
#include <chrono>
#include <thread>
uint32_t p = 0;
auto myFunc = [&p] (std::function<void(int)> pFun)
{
while (p < 100)
{
std::this_thread::sleep_for(std::chrono::milliseconds(350));
p += 10;
pFun(p);
}
};
m_progComp->setFunctionToExecuteInBG(myFunc);
*/
class ProgressBarComponent : public juce::Component, public juce::ButtonListener
{
public:
    ProgressBarComponent(uint32_t progMin, uint32_t progMax, bool bCancelButtonNeeded  = false);
    ~ProgressBarComponent() = default;

    void setFunctionToExecuteInBG(functionWithProgressCallback func);
    void setFunctionToExecuteInBG(luabind::object const func, luabind::object const progFun);

    void setProgressValue(uint32_t progress);

    void resized();
	void showPercantageDisplay(bool show);
	void setTextToDisplay(const String & str);
	void setBackGroundColour(const Colour color);
	void setForeGroundColour(const Colour color);

    void buttonClicked(juce::Button*);

    int showModal(Component* componentToCenterAround, String & title, const int width, const int height);
private:
    uint32_t                            m_progMin;
    uint32_t                            m_progMax;
    double                                m_currProg;
    std::unique_ptr<juce::TextButton> 		m_cancelBtn;
    std::unique_ptr<juce::ProgressBar>  		m_progressBar;
    std::shared_ptr<Au::ICancellableTask> m_task;
    bool                                    m_bIsModal;

    void closeDialog(int returnState);
};



