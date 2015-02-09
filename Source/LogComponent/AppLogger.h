/*
  ==============================================================================

  AppLogger.h
  Created: 9 Feb 2015 4:33:07pm
  Author:  cmodi

  ==============================================================================
  */

#ifndef APPLOGGER_H_INCLUDED
#define APPLOGGER_H_INCLUDED

#include "JuceHeader.h"
#include "LogComponent.h"

class LogComponent;

class AppLogger : public juce::Logger
{
public:
    AppLogger(LogComponent &_logComponent) : logComponent(_logComponent)
    {

    }

    ~AppLogger()
    {

    }

    void logMessage(const String & message)
    {
        String dataMessage = Time::getCurrentTime().toString(true, true, true) + "+" + message;
        logComponent.AddInLogArray(dataMessage);
    }


    juce_UseDebuggingNewOperator

private:
    LogComponent &logComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLogger)
};

#endif  // APPLOGGER_H_INCLUDED
