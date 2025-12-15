#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>

#include "db/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "=== HTTP Proxy Learning System - Database Test ===";
    qDebug() << "Starting database connection test...";

    // Get DatabaseManager instance
    DatabaseManager& dbManager = DatabaseManager::getInstance();

    // Test database connection
    qDebug() << "\n1. Testing database connection...";
    if (!dbManager.connectToDatabase()) {
        qDebug() << "❌ Database connection failed:" << dbManager.getLastError();
        qDebug() << "\nPlease ensure PostgreSQL is running and database 'course_db' exists.";
        qDebug() << "You can create it with: createdb course_db";
        return 1;
    }

    qDebug() << "✅ Database connection successful!";

    // Initialize database schema
    qDebug() << "\n2. Initializing database schema...";
    if (!dbManager.initDatabase()) {
        qDebug() << "❌ Database initialization failed:" << dbManager.getLastError();
        return 1;
    }

    qDebug() << "✅ Database schema initialized successfully!";

    // List all tables
    qDebug() << "\n3. Listing database tables...";
    QStringList tables = dbManager.getTableList();
    if (tables.isEmpty()) {
        qDebug() << "❌ No tables found in database";
        return 1;
    }

    qDebug() << "✅ Found" << tables.size() << "tables:";
    for (const QString& table : tables) {
        qDebug() << "  -" << table;
    }

    // Test table structure
    qDebug() << "\n4. Testing table structure...";
    
    // Check users table
    QSqlQuery usersQuery = dbManager.executeSelectQuery("SELECT column_name, data_type FROM information_schema.columns WHERE table_name = 'users' ORDER BY ordinal_position");
    qDebug() << "\n📋 Users table structure:";
    while (usersQuery.next()) {
        QString columnName = usersQuery.value(0).toString();
        QString dataType = usersQuery.value(1).toString();
        qDebug() << "  -" << columnName << "(" << dataType << ")";
    }

    // Check study_progress table
    QSqlQuery progressQuery = dbManager.executeSelectQuery("SELECT column_name, data_type FROM information_schema.columns WHERE table_name = 'study_progress' ORDER BY ordinal_position");
    qDebug() << "\n📋 Study_progress table structure:";
    while (progressQuery.next()) {
        QString columnName = progressQuery.value(0).toString();
        QString dataType = progressQuery.value(1).toString();
        qDebug() << "  -" << columnName << "(" << dataType << ")";
    }

    // Test data insertion and retrieval
    qDebug() << "\n5. Testing data operations...";
    
    // Check if admin user exists
    QSqlQuery adminQuery = dbManager.executeSelectQuery("SELECT login, role FROM users WHERE login = 'admin'");
    if (adminQuery.next()) {
        QString login = adminQuery.value(0).toString();
        QString role = adminQuery.value(1).toString();
        qDebug() << "✅ Found admin user:" << login << "with role:" << role;
    } else {
        qDebug() << "ℹ️  No admin user found (this is normal if schema.sql wasn't executed)";
    }

    // Count total users
    QSqlQuery countQuery = dbManager.executeSelectQuery("SELECT COUNT(*) FROM users");
    if (countQuery.next()) {
        int userCount = countQuery.value(0).toInt();
        qDebug() << "📊 Total users in database:" << userCount;
    }

    qDebug() << "\n=== Database Test Completed Successfully! ===";
    qDebug() << "The database layer is ready for the HTTP Proxy Learning System.";

    return 0;
}