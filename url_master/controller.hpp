#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <unordered_map>
#include <random>
#include <string>
#include <fstream>
#include "dto.hpp"
#include "sqliteclient.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class UrlShortenerController : public oatpp::web::server::api::ApiController {
private:
    
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
    std::string alphabet;
    const std::shared_ptr<SqliteClient>& m_dbClient;

    std::string generateShortCode() {
        std::string code;
        for (int i = 0; i < 6; ++i) {
            code += alphabet[dis(gen)];
        }
        return code;
    }
    

public:
    UrlShortenerController(
        
        const std::shared_ptr<SqliteClient>& dbClient,
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,   objectMapper)
    ) : oatpp::web::server::api::ApiController(objectMapper)
        , gen(rd())
        , m_dbClient(dbClient)
    {
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        dis = std::uniform_int_distribution<>(0, alphabet.length() - 1);

    }

    ENDPOINT("POST", "/shorten", shortenUrl,
             BODY_DTO(Object<ShortenUrlDto>, dto)) {
        auto longUrl = dto->url;
        std::string shortCode;
        OATPP_LOGI(
            "dbclinet", "Insering into db:  %s, %s",
             shortCode.c_str(), longUrl->c_str());
        
        std::shared_ptr<oatpp::orm::QueryResult> queryResult = nullptr;
        size_t maxTries = 100;
        do {
            shortCode = generateShortCode();
            queryResult = m_dbClient->addUrlMap(shortCode, longUrl);
            maxTries--;
        } while ((!queryResult->isSuccess()) && (maxTries > 0));
        
        auto response = ShortenedUrlDto::createShared();
        if(!queryResult->isSuccess()) {
            auto message = queryResult->getErrorMessage();
            OATPP_LOGD(
                "add url map query failed",
                "Error, message=%s", message->c_str()
            );
            response->shortUrl = "Error";
            return createDtoResponse(Status::CODE_400, response);
        }

        response->shortUrl = shortCode;
        return createDtoResponse(Status::CODE_200, response);
    }

    ENDPOINT("GET", "/{shortCode}", redirectUrl, PATH(String, shortCode)) {

      auto response = createResponse(Status::CODE_302, "");
      
      auto queryResult = m_dbClient->getLongUrl(shortCode);
      
      /* Check if the operation was successful */
      if (!queryResult->isSuccess()) {
        auto message = queryResult->getErrorMessage();
        OATPP_LOGD("get long url query failed", "Error, message=%s",
                   message->c_str());
      } else {
        auto dataset =
            queryResult->fetch<oatpp::Vector<oatpp::Fields<oatpp::String>>>();
        if (!dataset->empty()) {
            response->putHeader("Location", (dataset->at(0))["longUrl"]);
            return response;
        }
      }

      return createResponse(Status::CODE_404, "URL not found");
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif