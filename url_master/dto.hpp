#ifndef DTO_HPP
#define DTO_HPP

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class ShortenUrlDto : public oatpp::DTO {
    DTO_INIT(ShortenUrlDto, DTO)
    DTO_FIELD(String, url);
};

class ShortenedUrlDto : public oatpp::DTO {
    DTO_INIT(ShortenedUrlDto, DTO)
    DTO_FIELD(String, shortUrl);
};

#include OATPP_CODEGEN_END(DTO)

#endif