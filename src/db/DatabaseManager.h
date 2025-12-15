#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager& getInstance();
    
    bool connectToDatabase();
    bool initDatabase();
    bool isConnected() const;
    QStringList getTableList() const;
    QString getLastError() const;
    
    // Database operations
    bool executeQuery(const QString& query);
    QSqlQuery executeSelectQuery(const QString& query);
    
    // Prevent copying
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();
    
    bool createTables();
    bool loadSchemaFromFile();
    
    // Database connection parameters
    static const QString DB_HOSTNAME;
    static const QString DB_NAME;
    static const QString DB_USERNAME;
    static const QString DB_PASSWORD;
    static const int DB_PORT;
    
    QSqlDatabase m_database;
    QString m_lastError;
    bool m_connected;
};

#endif // DATABASEMANAGER_H