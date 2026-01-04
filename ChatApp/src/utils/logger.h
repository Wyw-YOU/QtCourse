// src/utils/logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum LogLevel {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
        Fatal = 4
    };

    static Logger* instance();

    void init(const QString& logDir = QString());
    void uninit();

    void log(LogLevel level, const QString& module, const QString& message);

    // 快捷方法
    void debug(const QString& module, const QString& message);
    void info(const QString& module, const QString& message);
    void warning(const QString& module, const QString& message);
    void error(const QString& module, const QString& message);
    void fatal(const QString& module, const QString& message);

signals:
    void newLogMessage(const QString& message);

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger();

    QString getLogFileName() const;
    QString levelToString(LogLevel level) const;

    static Logger* m_instance;
    QFile* m_logFile;
    QTextStream* m_textStream;
    QMutex m_mutex;
    QString m_logDir;
    bool m_initialized;
};

// 宏定义，便于使用
#define LOG_DEBUG(module, message) Logger::instance()->debug(module, message)
#define LOG_INFO(module, message) Logger::instance()->info(module, message)
#define LOG_WARNING(module, message) Logger::instance()->warning(module, message)
#define LOG_ERROR(module, message) Logger::instance()->error(module, message)
#define LOG_FATAL(module, message) Logger::instance()->fatal(module, message)

#endif // LOGGER_H
