#include "stdafx.h"
#include "MyLog.h"

MyLog *MyLog::MyLog = nullptr;

MyLog::MyLog()
    : logToFile(false), fileLogger(nullptr) {
  MyLog::MyLog = this;

}

MyLog::~MyLog() {
  MyLog = nullptr;
  if (fileLogger) {
    Logger::setCurrentLogger(0);
    deleteAndZero(fileLogger);
  } else {
    Logger::setCurrentLogger(0);
  }
}

void MyLog::setLogToFile(const bool _logToFile) {
  logToFile = _logToFile;
  if (logToFile) {
    if (!fileLogger) {
        String fullLogFilePath = APPDATAPATH + APP_NAME + String("_") + STR_PRODUCT_VERSION + ".logs";
        Logger::writeToLog("AuViDLog::setLogToFile: " + fullLogFilePath);
        AU_LOG_INFO("Auvid") << ("LogToFile started : " + fullLogFilePath);
        fileLogger =
            new FileLogger(File(fullLogFilePath),
            "AuViD debug log");
        Logger::setCurrentLogger(fileLogger);
    }

  } else {
    if (fileLogger) {
      Logger::setCurrentLogger(this);
      AU_LOG_INFO("Auvid") << "LogToFile Stopped: ";
      deleteAndZero(fileLogger);
    } else {
      Logger::setCurrentLogger(this);
    }
  }
}

void MyLog::logMessage(const String &message) { logMessage(message, AuViD::API::LogLevel::Info); }

void MyLog::logIOMessage(const std::string &message)
{
    logMessage(String(message), AuViD::API::LogLevel::Info, AuViD::API::LogTarget::DataIO);
}

void MyLog::logMessage(const String &message, const AuViD::API::LogLevel level,
    const AuViD::API::LogTarget target, const String & module)
{
    // Enable it if want logs on VS/Other Output
    // Logger::outputDebugString (message);
    MyLogMessage m;
    m.level = level;
    m.module = module.trim();
    m.message = message.trim();
    m.time = Time::getCurrentTime();
    m.logTarget = target;

    if (fileLogger) {
        fileLogger->logMessage(formatMessage(m).trim());
    }

    pendingQueue.add(m);
    triggerAsyncUpdate();
}


void MyLog::logMessage(const String &message, const AuViD::API::LogLevel level, const String & module) {
  // Logger::outputDebugString (message);
  MyLogMessage m;
  m.level = level;
  m.module = module.trim();
  m.message = message.trim();
  m.time = Time::getCurrentTime();
  m.logTarget = AuViD::API::LogTarget::Application;

  if (fileLogger) {
    fileLogger->logMessage(formatMessage(m).trim());
  }

  pendingQueue.add(m);
  triggerAsyncUpdate();
}

void MyLog::logMessage(const String &device, const MidiMessage &message,
    const AuViD::API::LogLevel level) {
    if (getBitOption(midiLogOptions.get(), midiLogInput) && level == AuViD::API::LogLevel::MidiIn) {
    logMessage((getBitOption(midiLogOptions.get(), midiLogDevice)
                    ? (" Dev:[" + device + "]")
                    : "") +
                   formatMidiMessage(message),
               level);
  }

  if (getBitOption(midiLogOptions.get(), midiLogOutput) && level == AuViD::API::LogLevel::MidiOut) {
    logMessage((getBitOption(midiLogOptions.get(), midiLogDevice)
                    ? (" Dev:[" + device + "]")
                    : "") +
                   formatMidiMessage(message),
               level);
  }
}

void MyLog::logMessage(const String &device, const MidiBuffer &buffer,
    const AuViD::API::LogLevel level) {
  MidiBuffer::Iterator i(buffer);
  MidiMessage m;
  int t;
  while (i.getNextEvent(m, t)) {
    logMessage(device, m, level);
  }
}

void MyLog::handleAsyncUpdate() {
    for (int i = 0; i < pendingQueue.size(); i++) {
        auto msg = pendingQueue[i];
        if (msg.level == AuViD::API::LogLevel::ErrorWithUi) {
            WARN(formatMessage(pendingQueue[i], true, true));
        }

        for (int i = 0; i < listeners.size(); i++) {
            Listener * listener = listeners.getListeners()[i];
            if (listener)
            {
                if ((msg.logTarget & listener->getLogTarget()) == msg.logTarget) {
                    listener->messageLogged(msg);
                }
            }
        }
    }
    pendingQueue.clear();
}

Colour MyLog::getColourForLevel(const AuViD::API::LogLevel logLevel) {
  switch (logLevel) {
  case AuViD::API::LogLevel::Info:
    return Colours::black;
  case AuViD::API::LogLevel::Warn:
    return Colours::darkblue;
  case AuViD::API::LogLevel::LuaRuntimeError:
  case AuViD::API::LogLevel::LuaError:
  case AuViD::API::LogLevel::Error:
  case AuViD::API::LogLevel::ErrorWithUi:
      return Colours::red;
  case AuViD::API::LogLevel::Debug:
    return Colours::darkgrey;
  case AuViD::API::LogLevel::Lua:
    return Colours::blue;
  case AuViD::API::LogLevel::MidiIn:
    return Colours::steelblue;
  case AuViD::API::LogLevel::MidiOut:
    return Colours::darkseagreen;
  case AuViD::API::LogLevel::Transaction:
    return (Colours::darkmagenta);
  case AuViD::API::LogLevel::kLogLevels:
  default:
    return Colours::black;
  }

}

Colour MyLog::getColourForMessage(const MyLogMessage &message) {
  return (getColourForLevel(message.level));
}

String MyLog::levelToString(const AuViD::API::LogLevel &level) {
  switch (level) {
  case AuViD::API::LogLevel::Info:
    return ("[INFO    ]");
  case AuViD::API::LogLevel::Warn:
    return ("[WARN    ]");
  case AuViD::API::LogLevel::Error:
    return ("[ERROR   ]");
  case AuViD::API::LogLevel::Debug:
    return ("[DEBUG   ]");
  case AuViD::API::LogLevel::Lua:
    return ("[LUA     ]");
  case AuViD::API::LogLevel::MidiIn:
    return ("[MIDI IN ]");
  case AuViD::API::LogLevel::MidiOut:
    return ("[MIDI OUT]");
  case AuViD::API::LogLevel::LuaError:
    return ("[LUA ERR ]");
  case AuViD::API::LogLevel::LuaRuntimeError:
    return ("[LUA RERR]");
  case AuViD::API::LogLevel::Transaction:
    return ("[TRANS   ]");
  case AuViD::API::LogLevel::kLogLevels:
    return ("[INFO    ]");
  }
  return ("[INFO   ]");
}

AuViD::API::LogLevel MyLog::stringToLevel(const String &_level) {
  const String level = _level.trim();
  if (level == "INFO")
      return (AuViD::API::LogLevel::Info);
  if (level == "WARN")
      return (AuViD::API::LogLevel::Warn);
  if (level == "ERROR")
      return (AuViD::API::LogLevel::Error);
  if (level == "DEBUG")
      return (AuViD::API::LogLevel::Debug);
  if (level == "LUA")
      return (AuViD::API::LogLevel::Lua);
  if (level == "MIDI IN")
      return (AuViD::API::LogLevel::MidiIn);
  if (level == "MIDI OUT")
      return (AuViD::API::LogLevel::MidiOut);
  if (level == "LUA ERR")
      return (AuViD::API::LogLevel::LuaError);
  if (level == "LUA RERR")
      return (AuViD::API::LogLevel::LuaRuntimeError);
  if (level == "TRANS")
      return (AuViD::API::LogLevel::Transaction);

  return (AuViD::API::LogLevel::kLogLevels);
}

// only showing [hour:min:sec]
String MyLog::timeToString(const Time &time) {
    return (time.formatted("[%H:%M:%S] ")); /* +
          String::formatted("%06d]", time.getMilliseconds()));*/
}

String MyLog::formatMessage(const MyLogMessage &m,
                               const bool includeLevel,
                               const bool includeTimestamp) {
  String ret;

  if (includeLevel)
    ret << levelToString(m.level);

  // include module
  ret << "[ " << m.module << " ]";

  if (includeTimestamp)
    ret << timeToString(m.time);

  ret << "= ";

  if (m.message.endsWith("\n")) {
    ret << m.message.substring(1);
  } else {
    ret << m.message;
  }

  return (ret);
}

const String MyLog::formatMidiMessage(const MidiMessage &message) {
  return (getMidiMessageAsLogString(
      message, getBitOption(midiLogOptions.get(), midiLogName),
      getBitOption(midiLogOptions.get(), midiLogChannel),
      getBitOption(midiLogOptions.get(), midiLogNumber),
      getBitOption(midiLogOptions.get(), midiLogValue),
      getBitOption(midiLogOptions.get(), midiLogTimestamp),
      getBitOption(midiLogOptions.get(), midiLogRawData),
      getBitOption(midiLogOptions.get(), midiLogRawDecimal),
      getBitOption(midiLogOptions.get(), midiLogDataSize)));
}

void MyLog::addListener(Listener *l) { listeners.add(l); }

void MyLog::removeListener(Listener *l) { listeners.remove(l); }

void MyLog::wrapForLua(lua_State *L)
{
    using namespace luabind;
    module(L)
        [
            class_<MyLog>("Logs")
            .enum_("MyLogTypes")
            [
                value("Info", 0),
                value("Warn", 1),
                value("Error", 2),
                value("Debug", 3),
                value("Lua", 4),
                value("MidiIn", 5),
                value("MidiOut", 6),
                value("LuaError", 7),
                value("LuaRuntimeError", 8),
                value("Transaction", 9),
                value("kLogLevels", 10),
                value("ErrorWithUi", 11)
            ]
            .enum_("MyLogTargets")
                [
                    value("Application", 1),
                    value("Command", 2),
                    value("DataIO", 4)
                ]
        ];
}
