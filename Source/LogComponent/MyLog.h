#ifndef __My_LOG__
#define __My_LOG__

#include <base/logger/Logger.h>

struct lua_State;

#ifdef JUCE_DEBUG
#define _DBG(message)                                                          \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::Debug)
#else
#define _DBG(message)
#endif

// macro used to log messages into any target logger
#define _MSG(message, logLevel, logTarget, module)                           \
if (MyLog::MyLog != nullptr)                                           \
    MyLog::MyLog->logMessage(message, logLevel, logTarget, module)

#define _WRN(message)                                                          \
if (MyLog::MyLog != nullptr)                                           \
    MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::Warn)
#define _ERR(message)                                                          \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::Error)
#define _ERRUI(message)                                                          \
if (MyLog::MyLog != nullptr)                                           \
    MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::ErrorWithUi)
#define _LUA(message)                                                          \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::Lua, "Script")
#define _LERR(message)                                                         \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::LuaError, "Script")
#define _LRUNERR(message)                                                      \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::LuaRuntimeError, "Script")
#define _TRANS(message)                                                        \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::Transaction)
#define _MIN(device, midiMessage)                                              \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(device, midiMessage, AuViD::API::LogLevel::MidiIn)
#define _MOUT(device, midiMessage)                                             \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(device, midiMessage, AuViD::API::LogLevel::MidiOut)
#define _MINSTR(message)                                                       \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::MidiIn)
#define _MOUTSTR(message)                                                      \
  if (MyLog::MyLog != nullptr)                                           \
  MyLog::MyLog->logMessage(message, AuViD::API::LogLevel::MidiOut)

class MyLog : public AsyncUpdater, public Logger, public DeletedAtShutdown {
public:
  
  struct MyLogMessage {
    String message;
    String module;
    AuViD::API::LogLevel level;
    Time time;
    AuViD::API::LogTarget logTarget;
  };

  class Listener {
  public:
    virtual ~Listener() {}
    virtual void messageLogged(MyLog::MyLogMessage message) = 0;
    virtual AuViD::API::LogTarget getLogTarget() = 0;
  };

  MyLog();
  ~MyLog();

  void addListener(Listener *l);
  void removeListener(Listener *l);
  void handleAsyncUpdate();

  void logIOMessage(const std::string &message);
  void logMessage(const String &message, const AuViD::API::LogLevel logLevel,
      const AuViD::API::LogTarget logTarget, const String & module = "");

  void logMessage(const String &message, const AuViD::API::LogLevel level, const String & module = "");
  void logMessage(const String &device, const MidiMessage &message,
      const AuViD::API::LogLevel level);
  void logMessage(const String &device, const MidiBuffer &buffer,
      const AuViD::API::LogLevel level);
  void logMessage(const String &message);
  void setLogToFile(const bool _logToFile);
  bool getLogMidiInput();
  bool getLogMidiOutput();
  const String formatMidiMessage(const MidiMessage &message);
  static AuViD::API::LogLevel stringToLevel(const String &level);
  static String levelToString(const AuViD::API::LogLevel &level);
  static String timeToString(const Time &time);
  static Colour getColourForMessage(const MyLogMessage &message);
  static Colour getColourForLevel(const AuViD::API::LogLevel logLevel);
  static String formatMessage(const MyLogMessage &m,
                              const bool includeLevel = true,
                              const bool includeTimestamp = true);
  void setMidiLogOptions(const int logOptions) {
    midiLogOptions.set(logOptions);
  }
  static MyLog *MyLog;

  static void wrapForLua(lua_State *L);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLog);

private:
  ReadWriteLock loggerLock;
  bool logToFile;
  FileLogger *fileLogger;
  Array<AuViD::API::LogLevel> levels;
  ListenerList<Listener> listeners;
  Array<MyLogMessage, CriticalSection> pendingQueue;
  MidiBuffer logBuffer;
  Atomic<int> midiLogOptions;
};


#endif // __My_LOG__
