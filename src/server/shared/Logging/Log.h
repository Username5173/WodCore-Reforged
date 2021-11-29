/*
* Copyright (C) 2008-2020 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
* Copyright (C) 2021 WodCore Reforged
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#ifndef TRINITYCORE_LOG_H
#define TRINITYCORE_LOG_H

#include "Common.h"
#include "Define.h"
#include "Appender.h"
#include "LogWorker.h"
#include "Logger.h"
#include "LogOperation.h"
#include "Dynamic/UnorderedMap.h"

#include <string>
#include <ace/Singleton.h>

#include <cstdarg>
#include <cstdio>

class Log
{
    friend class ACE_Singleton<Log, ACE_Thread_Mutex>;

    typedef UNORDERED_MAP<uint8, Logger> LoggerMap;

    private:
        Log();
        ~Log();

    public:
        void LoadFromConfig();
        void Close();
        bool ShouldLog(LogFilterType type, LogLevel level) const;
        bool SetLogLevel(std::string const& name, char const* level, bool isLogger = true);

        inline void outTrace(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_TRACE))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_TRACE, str, ap);

            va_end(ap);
        }
        inline void outDebug(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_DEBUG))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_DEBUG, str, ap);

            va_end(ap);
        }
        inline void outInfo(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_INFO))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_INFO, str, ap);

            va_end(ap);
        }
        inline void outWarn(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_WARN))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_WARN, str, ap);

            va_end(ap);
        }
        inline void outError(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_ERROR))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_ERROR, str, ap);

            va_end(ap);
        }
        inline void outFatal(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_FATAL))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_FATAL, str, ap);

            va_end(ap);
        }
        inline void outCharDump(char const* str, uint32 account_id, uint32 guid, char const* name)
        {
            if (!str || !ShouldLog(LOG_FILTER_PLAYER_DUMP, LOG_LEVEL_INFO))
                return;

            std::ostringstream ss;
            ss << "== START DUMP == (account: " << account_id << " guid: " << guid << " name: " << name
                << ")\n" << str << "\n== END DUMP ==\n";

            LogMessage* msg = new LogMessage(LOG_LEVEL_INFO, LOG_FILTER_PLAYER_DUMP, ss.str());
            ss.clear();
            ss << guid << '_' << name;

            msg->param1 = ss.str();

            write(msg);
        }

        /// No filters
        void outArena(const char * str, ...)               ATTR_PRINTF(2, 3);
        void outCommand(uint32 gm_account_id, std::string gm_account_name,
                        uint32 gm_character_id, std::string gm_character_name,
                        uint32 sc_account_id, std::string sc_account_name,
                        uint32 sc_character_id, std::string sc_character_name,
                        const char * str, ...)              ATTR_PRINTF(10, 11);
        void outGmChat(uint32 message_type,
                       uint32 from_account_id, std::string from_account_name,
                       uint32 from_character_id, std::string from_character_name,
                       uint32 to_account_id, std::string to_account_name,
                       uint32 to_character_id, std::string to_character_name,
                       const char * str);
        void outAshran(const char* str, ...);
        
        static std::string GetTimestampStr();
        
        void SetRealmId(uint32 id);
        uint32 GetRealmID() const { return 1; } //hackfix, set realm as ID 1, need more work on howto identifi player's realm id

    private:
        void vlog(LogFilterType f, LogLevel level, char const* str, va_list argptr);
        void write(LogMessage* msg);

        inline Logger* GetLoggerByType(LogFilterType filter);
        Appender* GetAppenderByName(std::string const& name);
        uint8 NextAppenderId();
        void CreateAppenderFromConfig(const char* name);
        void CreateLoggerFromConfig(const char* name);
        void ReadAppendersFromConfig();
        void ReadLoggersFromConfig();

        AppenderMap appenders;
        LoggerMap loggers;
        uint8 AppenderId;
        LogLevel lowestLogLevel;

        std::string m_logsDir;
        std::string m_logsTimestamp;

        LogWorker* worker;

        FILE* ashranLog;

        /// Slack
        bool        m_SlackEnable;
        std::string m_SlackApiUrl;
        std::string m_SlackAppName;

        uint8 m_LogLevelTypeByFilterCache[MAX_LOG_FILTER];
        uint8 m_LogTypePresentCache[MAX_LOG_FILTER];
};

inline bool Log::ShouldLog(LogFilterType type, LogLevel level) const
{
    LoggerMap::const_iterator it = loggers.find(uint8(type));
    if (it != loggers.end())
    {
        LogLevel logLevel = it->second.getLogLevel();
        return logLevel != LOG_LEVEL_DISABLED && logLevel <= level;
    }

    if (type != LOG_FILTER_GENERAL)
        return ShouldLog(LOG_FILTER_GENERAL, level);
    else
        return false;
}

#define sLog ACE_Singleton<Log, ACE_Thread_Mutex>::instance()

#if COMPILER != COMPILER_MICROSOFT
#define TC_LOG_MESSAGE_BODY(level__, call__, filterType__, ...)     \
        do {                                                        \
            if (sLog->ShouldLog(filterType__, level__))             \
                sLog->call__(filterType__, __VA_ARGS__);            \
        } while (0)
#else
#define TC_LOG_MESSAGE_BODY(level__, call__, filterType__, ...)     \
        __pragma(warning(push))                                     \
        __pragma(warning(disable:4127))                             \
        do {                                                        \
            if (sLog->ShouldLog(filterType__, level__))             \
                sLog->call__(filterType__, __VA_ARGS__);            \
        } while (0)                                                 \
        __pragma(warning(pop))
#endif

#define TC_LOG_TRACE(filterType__, ...) \
    TC_LOG_MESSAGE_BODY(LOG_LEVEL_TRACE, outTrace, filterType__, __VA_ARGS__)

#define TC_LOG_DEBUG(filterType__, ...) \
    TC_LOG_MESSAGE_BODY(LOG_LEVEL_DEBUG, outDebug, filterType__, __VA_ARGS__)

#define TC_LOG_INFO(filterType__, ...)  \
    TC_LOG_MESSAGE_BODY(LOG_LEVEL_INFO, outInfo, filterType__, __VA_ARGS__)

#define TC_LOG_WARN(filterType__, ...)  \
    TC_LOG_MESSAGE_BODY(LOG_LEVEL_WARN, outWarn, filterType__, __VA_ARGS__)

#define TC_LOG_ERROR(filterType__, ...) \
    TC_LOG_MESSAGE_BODY(LOG_LEVEL_ERROR, outError, filterType__, __VA_ARGS__)

#define TC_LOG_FATAL(filterType__, ...) \
    TC_LOG_MESSAGE_BODY(LOG_LEVEL_FATAL, outFatal, filterType__, __VA_ARGS__)

/*
// Returns default logger if the requested logger is not found
inline Logger* Log::GetLoggerByType(LogFilterType filter)
{
    return m_LogTypePresentCache[filter] ? &loggers[filter] : &(loggers[0]);
}

inline bool Log::ShouldLog(LogFilterType type, LogLevel level)
{
    // Don't even look for a logger if the LogLevel is lower than lowest log levels across all loggers
    if (level < lowestLogLevel)
        return false;

    if (m_LogTypePresentCache[type])
    {
        LogLevel loggerLevel = (LogLevel)m_LogLevelTypeByFilterCache[type];
        return  loggerLevel != LOG_LEVEL_DISABLED && loggerLevel <= level;
    }

    if (type != LOG_FILTER_GENERAL)
        return ShouldLog(LOG_FILTER_GENERAL, level);

    return false;
}
*/

#endif
