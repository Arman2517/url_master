#include <atomic>
#include <thread>

#include "controller.hpp"
#include "dto.hpp"
#include "app_init.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

std::atomic<bool> isRunning(true);

void signalHandler(int signum) {
    isRunning = false;
    OATPP_LOGI("Server", "Termination signal received: %d", signum);
}

void run(
     const std::string& host, ushort port) {

  AppComponents components(host, port);

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Inject SqliteClient database client */
  OATPP_COMPONENT(std::shared_ptr<SqliteClient>, dbClient);

  /* Create UrlShortenerController and add all of its endpoints to router */
  auto myController = 
    std::make_shared<UrlShortenerController>(dbClient);
  router->addController(myController);

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                  connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                  connectionProvider);
  
  /* Create server which takes provided TCP connections and passes them to HTTP
   * connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  std::thread oatppThread([&server] {
    /* Run server */
    server.run();
  });
  
  /* Print info about server port */
  OATPP_LOGI("UrlShortener", "Server running on port %s",
             connectionProvider->getProperty("port").getData());

// waiting loop
  while(isRunning) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // OATPP_LOGI("UrlShortener", "Waiting for SIGTERM or SIGINT...");
  }
  OATPP_LOGI("UrlShortener", "Server is shutting down...");
  /* Finally, stop the ConnectionHandler and wait until all running connections are closed */
  connectionHandler->stop();

  /* stop db connection pool */
  OATPP_COMPONENT(
    std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
    dbConnectionProvider);
  dbConnectionProvider->stop();

  /* Now, check if server is still running and stop it if needed */
  if (server.getStatus() == oatpp::network::Server::STATUS_RUNNING) {
    server.stop();
  }

  /* Before returning, check if the server-thread has already stopped or if we need to wait for the server to stop */
  if(oatppThread.joinable()) {

    /* We need to wait until the thread is done */
    oatppThread.join();
  }

}

int main(int argc, char* argv[]) {
    oatpp::base::Environment::init();

    const char* mapFileName = argc > 1 ? argv[1] : "urlMap.txt";
    const char* host = argc > 2 ? argv[2] : "localhost";
    ushort port = argc > 3 ? std::stoul(argv[3]) : 8000;
    
    // Register signal handler for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    run( host, port);

    oatpp::base::Environment::destroy();
    return 0;
}
