#include <QCoreApplication>
#include <QTest>
#include <QDebug>
#include "../src/db/DatabaseManager.h"

class TestDatabaseConnection : public QObject
{
    Q_OBJECT

private slots:
    void testConnection();
    void testTableCreation();
    void testDataOperations();

private:
    DatabaseManager* dbManager;
};

void TestDatabaseConnection::testConnection()
{
    dbManager = &DatabaseManager::getInstance();
    QVERIFY(dbManager->connectToDatabase());
    QVERIFY(dbManager->isConnected());
}

void TestDatabaseConnection::testTableCreation()
{
    QVERIFY(dbManager->initDatabase());
    QStringList tables = dbManager->getTableList();
    QVERIFY(tables.contains("users"));
    QVERIFY(tables.contains("study_progress"));
}

void TestDatabaseConnection::testDataOperations()
{
    // Test basic query execution
    QVERIFY(dbManager->executeQuery("SELECT 1"));
    
    // Test table structure
    auto query = dbManager->executeSelectQuery("SELECT COUNT(*) FROM users");
    QVERIFY(query.next());
}

QTEST_MAIN(TestDatabaseConnection)
#include "test_db_connection.moc"