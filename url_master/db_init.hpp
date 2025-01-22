#ifndef DB_INIT_HPP
#define DB_INIT_HPP

#include "oatpp-sqlite/orm.hpp"
#include "sqliteclient.hpp"

#ifndef DATABASE_FILE
#define DATABASE_FILE "urlMapDb.sqlite"
#endif
class DatabaseComponent {
public:
    
  /**
   * Create database connection provider component
   */
  OATPP_CREATE_COMPONENT(
    std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
     dbConnectionProvider)([] {

    /* Create database-specific ConnectionProvider */
    auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(DATABASE_FILE);

    /* Create database-specific ConnectionPool */
    return oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
                                                       10 /* max-connections */,
                                                       std::chrono::seconds(5) /* connection TTL */);

  }());

  /**
   * Create database client
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<SqliteClient>, sqLiteClient)([] {

    /* Get database ConnectionProvider component */
    OATPP_COMPONENT(
        std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
         connectionProvider);

    /* Create database-specific Executor */
    auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

    /* Create MyClient database client */
    return std::make_shared<SqliteClient>(executor);

  }());

};

#endif