CREATE TABLE UrlMap (
    id          INTEGER PRIMARY KEY,
    shortUrl    VARCHAR UNIQUE,
    longUrl     VARCHAR
);

INSERT INTO UrlMap
(shortUrl, longUrl) VALUES ("shortUrl", "longUrl");