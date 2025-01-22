# Web Cервис на С++ для сокращения URL адреса

Для сборки проекта нужно установить vcpkg (https://learn.microsoft.com/ru-ru/vcpkg/get_started/get-started?pivots=shell-powershell) и cmake, vcpkg нужна для автоматического скачивания и установки библиотеки oat++ на которой реализован сервис

Для сборки использовать скрипт build.sh

Сервис представлен 2-ма ендпоинтами

POST /shorten

GET /{shortcode}

Для тестирования нужно запустить собранный сервер 

```bash
./build/cpp_url_short
```

Затем совершить запрос (можно использовать test_post.sh)
```
curl -X POST localhost:8000/shorten -H "Content-Type: application/json" -d '{"url":"https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution"}'
```
В результате должен вернутся сокращенный адрес в формате
```
{"shortUrl":"f8xZHd"}
```

Затем если ввести в браузере адрес 
```
localhost:8000/f8xZHd
```
должно произойти перенаправление на исходный длинный адрес из POST запроса

Для завершения сервиса использовать комбинацию клави Ctrl+C