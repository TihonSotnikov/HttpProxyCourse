#include "db/DatabaseManager.h"

// Database connection constants
const QString DatabaseManager::DB_HOSTNAME = "localhost";
const QString DatabaseManager::DB_NAME = "course_db";
const QString DatabaseManager::DB_USERNAME = "postgres";
const QString DatabaseManager::DB_PASSWORD = "";
const int DatabaseManager::DB_PORT = 5432;

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent), m_connected(false)
{
    m_database = QSqlDatabase::addDatabase("QPSQL");
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connectToDatabase()
{
    if (m_connected && m_database.isOpen()) {
        return true;
    }

    m_database.setHostName(DB_HOSTNAME);
    m_database.setPort(DB_PORT);
    m_database.setDatabaseName(DB_NAME);
    m_database.setUserName(DB_USERNAME);
    m_database.setPassword(DB_PASSWORD);

    if (!m_database.open()) {
        m_lastError = QString("Failed to connect to database: %1").arg(m_database.lastError().text());
        qDebug() << m_lastError;
        m_connected = false;
        return false;
    }

    m_connected = true;
    m_lastError.clear();
    qDebug() << "Successfully connected to PostgreSQL database:" << DB_NAME;
    return true;
}

bool DatabaseManager::initDatabase()
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }

    // Try to load schema from file first
    if (loadSchemaFromFile()) {
        qDebug() << "Database schema loaded from file successfully";
        return true;
    }

    // Fallback to hardcoded schema
    qDebug() << "Loading schema from file failed, using hardcoded schema";
    return createTables();
}

bool DatabaseManager::loadSchemaFromFile()
{
    QString schemaPath = QCoreApplication::applicationDirPath() + "/../data/schema.sql";
    QFile schemaFile(schemaPath);
    
    if (!schemaFile.exists()) {
        // Try alternative path
        schemaPath = "data/schema.sql";
        schemaFile.setFileName(schemaPath);
    }
    
    if (!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QString("Cannot open schema file: %1").arg(schemaPath);
        qDebug() << m_lastError;
        return false;
    }

    QTextStream in(&schemaFile);
    QString schemaContent = in.readAll();
    schemaFile.close();

    // Split by semicolon and execute each statement
    QStringList statements = schemaContent.split(';', Qt::SkipEmptyParts);
    
    for (const QString& statement : statements) {
        QString trimmedStatement = statement.trimmed();
        if (trimmedStatement.isEmpty() || trimmedStatement.startsWith("--")) {
            continue;
        }

        QSqlQuery query(m_database);
        if (!query.exec(trimmedStatement)) {
            m_lastError = QString("Failed to execute schema statement: %1").arg(query.lastError().text());
            qDebug() << m_lastError;
            qDebug() << "Statement:" << trimmedStatement;
            return false;
        }
    }

    return true;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(m_database);

    // Create users table
    QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id SERIAL PRIMARY KEY,
            login TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            role TEXT NOT NULL DEFAULT 'student',
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createUsersTable)) {
        m_lastError = QString("Failed to create users table: %1").arg(query.lastError().text());
        qDebug() << m_lastError;
        return false;
    }

    // Create study_progress table
    QString createProgressTable = R"(
        CREATE TABLE IF NOT EXISTS study_progress (
            user_id INTEGER NOT NULL,
            chapter_id INTEGER NOT NULL,
            status TEXT NOT NULL DEFAULT 'not_started',
            last_score INTEGER DEFAULT 0,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (user_id, chapter_id),
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";

    if (!query.exec(createProgressTable)) {
        m_lastError = QString("Failed to create study_progress table: %1").arg(query.lastError().text());
        qDebug() << m_lastError;
        return false;
    }

    // Create indexes
    query.exec("CREATE INDEX IF NOT EXISTS idx_users_login ON users(login)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_study_progress_user_id ON study_progress(user_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_study_progress_chapter_id ON study_progress(chapter_id)");

    qDebug() << "Database tables created successfully";
    return true;
}

bool DatabaseManager::isConnected() const
{
    return m_connected && m_database.isOpen();
}

QStringList DatabaseManager::getTableList() const
{
    if (!isConnected()) {
        return QStringList();
    }

    return m_database.tables();
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}

bool DatabaseManager::executeQuery(const QString& query)
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }

    QSqlQuery sqlQuery(m_database);
    if (!sqlQuery.exec(query)) {
        m_lastError = QString("Query execution failed: %1").arg(sqlQuery.lastError().text());
        return false;
    }

    return true;
}

QSqlQuery DatabaseManager::executeSelectQuery(const QString& query)
{
    QSqlQuery sqlQuery(m_database);
    if (isConnected()) {
        sqlQuery.exec(query);
    }
    return sqlQuery;
}