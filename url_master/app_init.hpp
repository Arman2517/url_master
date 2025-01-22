#ifndef APP_INIT_HPP
#define APP_INIT_HPP

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/core/macro/component.hpp"

#include "db_init.hpp"


/**
 *  Class which creates and holds Application components and registers
 * components in oatpp::base::Environment Order of components initialization is
 * from top to bottom
 */
class AppComponents {
private:
  std::string m_host;
  ushort m_port;
public:
  AppComponents(const std::string& host, ushort port)
      : m_host(host),
        m_port(port) {}

  DatabaseComponent databaseComponent;

  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      serverConnectionProvider)([this] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
        {m_host, m_port, oatpp::network::Address::IP_4});
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                         httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  /**
   *  Create ConnectionHandler component which uses Router component to route
   * requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                         serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                    router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's
   * API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                         apiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

  
};

#endif