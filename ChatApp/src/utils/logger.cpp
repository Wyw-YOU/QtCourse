// src/utils/logger.cpp
#include "logger.h"
#include <QDir>
#include <QCoreApplication>
#include "global.h"

Logger* Logger::m_instance = nullptr;

Logger::Logger(QObject* parent)
    : QObject(parent)
    , m_logFile(nullptr)
    , m_textStream(nullptr)
    , m_initialized(false)
{
}

Logger::~Logger()
{
    uninit();
}

Logger* Logger::instance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    if (!m_instance) {
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::init(const QString& logDir)
{
    QMutexLocker locker(&m_mutex);

    if (m_initialized) {
        return;
    }

    // 确定日志目录
    if (logDir.isEmpty()) {
        m_logDir = Utils::getDataPath() + "/logs";
    } else {
        m_logDir = logDir;
    }

    // 创建日志目录
    QDir dir(m_logDir);
    if (!dir.exists()) {
        dir.mkpath(m_logDir);
    }

    // 打开日志文件
    QString logFilePath = m_logDir + "/" + getLogFileName();
    m_logFile = new QFile(logFilePath);

    if (m_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_textStream = new QTextStream(m_logFile);
        m_textStream->setCodec("UTF-8");
        m_initialized = true;

        info("Logger", QString("Log system initialized. Log file: %1").arg(logFilePath));
    } else {
        qWarning() << "Failed to open log file:" << logFilePath;
        delete m_logFile;
        m_logFile = nullptr;
    }
}

void Logger::uninit()
{
    QMutexLocker locker(&m_mutex);

    if (m_textStream) {
        m_textStream->flush();
        delete m_textStream;
        m_textStream = nullptr;
    }

    if (m_logFile) {
        m_logFile->close();
        delete m_logFile;
        m_logFile = nullptr;
    }

    m_initialized = false;
}

void Logger::log(LogLevel level, const QString& module, const QString& message)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) {
        return;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString levelStr = levelToString(level);
    QString logMessage = QString("[%1] [%2] [%3] %4")
                             .arg(timestamp)
                             .arg(levelStr)
                             .arg(module)
                             .arg(message);

    // 写入文件
    if (m_textStream) {
        *m_textStream << logMessage << "\n";
        m_textStream->flush();
    }

    // 输出到控制台
#if defined(QT_DEBUG)
    QTextStream(stdout) << logMessage << "\n";
#endif

    // 发送信号
    emit newLogMessage(logMessage);
}

void Logger::debug(const QString& module, const QString& message)
{
    log(Debug, module, message);
}

void Logger::info(const QString& module, const QString& message)
{
    log(Info, module, message);
}

void Logger::warning(const QString& module, const QString& message)
{
    log(Warning, module, message);
}

void Logger::error(const QString& module, const QString& message)
{
    log(Error, module, message);
}

void Logger::fatal(const QString& module, const QString& message)
{
    log(Fatal, module, message);
}

QString Logger::getLogFileName() const
{
    return QString("%1_%2.log")
    .arg(APP_NAME)
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
}

QString Logger::levelToString(LogLevel level) const
{
    switch (level) {
    case Debug: return "DEBUG";
    case Info: return "INFO";
    case Warning: return "WARNING";
    case Error: return "ERROR";
    case Fatal: return "FATAL";
    default: return "UNKNOWN";
    }
}

// 全局工具函数实现
namespace Utils {
QString getCurrentTimeString()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

QString formatFileSize(qint64 bytes)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;

    if (bytes >= GB) {
        return QString("%1 GB").arg(QString::number(bytes * 1.0 / GB, 'f', 2));
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(QString::number(bytes * 1.0 / MB, 'f', 2));
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(QString::number(bytes * 1.0 / KB, 'f', 2));
    } else {
        return QString("%1 B").arg(bytes);
    }
}

QString getDataPath()
{
    QString path;
#ifdef Q_OS_WIN
    path = QDir::homePath() + "/AppData/Local/" + COMPANY_NAME + "/" + APP_NAME;
#else
    path = QDir::homePath() + "/." + COMPANY_NAME + "/" + APP_NAME;
#endif

    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(path);
    }
    return path;
}

QString getAvatarPath(int userId)
{
    QString path = getDataPath() + "/avatars";
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(path);
    }
    return path + QString("/avatar_%1.png").arg(userId);
}
}
