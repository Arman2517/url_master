#ifndef SQLITECLIENT_HPP
#define SQLITECLIENT_HPP

#include "oatpp/orm/SchemaMigration.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient) ///< Begin code-gen section

class SqliteClient : public oatpp::orm::DbClient {
public:

  SqliteClient(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {
    oatpp::orm::SchemaMigration migration(executor); 
    migration.addFile(1 /* version */, "initial_schema.sql" /* migration script */);
    migration.migrate(); //<-- This guy will throw on error.
  }
  
  QUERY(addUrlMap,
        "INSERT INTO UrlMap (shortUrl, longUrl) VALUES (:shortUrl, :longUrl);",
        PARAM(oatpp::String, shortUrl), 
        PARAM(oatpp::String, longUrl)
        ) 

  QUERY(getLongUrl, 
        "SELECT longUrl FROM UrlMap WHERE shortUrl=:shortUrl;", 
        PARAM(oatpp::String, shortUrl)) 
        
};

#include OATPP_CODEGEN_END(DbClient) ///< End code-gen section

#endif