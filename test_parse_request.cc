/*************************************************************************
    > File Name: test_parse_request.cc
    > Author: hsz
    > Brief:
    > Created Time: Tue 08 Feb 2022 02:51:23 PM CST
 ************************************************************************/

#include "http_request_parser.h"
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

void on_request_method(void *data, const char *at, size_t length)
{
    char buf[1024] = {0};
    memcpy(buf, at, length);
    buf[length] = '\0';
    printf("%s() %s\n", __func__, buf);
}

void on_request_uri(void *data, const char *at, size_t length)
{
    char buf[1024] = {0};
    memcpy(buf, at, length);
    buf[length] = '\0';
    printf("%s() %s\n", __func__, buf);
}

void on_request_fragment(void *data, const char *at, size_t length)
{
    char buf[1024] = {0};
    memcpy(buf, at, length);
    buf[length] = '\0';
    printf("%s() %s\n", __func__, buf);
}

void on_request_path(void *data, const char *at, size_t length)
{
    char buf[1024] = {0};
    memcpy(buf, at, length);
    buf[length] = '\0';
    printf("%s() %s\n", __func__, buf);
}

void on_request_query(void *data, const char *at, size_t length)
{
    char buf[1024] = {0};
    memcpy(buf, at, length);
    buf[length] = '\0';
    printf("%s() %s\n", __func__, buf);
}

void on_request_version(void *data, const char *at, size_t length)
{
    char buf[1024] = {0};
    memcpy(buf, at, length);
    buf[length] = '\0';
    printf("%s() %s\n", __func__, buf);
}

void on_request_header_done(void *data, const char *at, size_t length)
{
    char buf[1024] = {0};
    memcpy(buf, at, length);
    buf[length] = '\0';
    printf("%s() %s\n", __func__, buf);
}

void on_request_http_field(void *data, const char *field, size_t flen, const char *value, size_t vlen)
{
    std::string _filed(field, flen);
    std::string _value(value, vlen);

    printf("%s() [%s: %s]\n", __func__, _filed.c_str(), _value.c_str());
}

int main(int argc, char **argv)
{
    const char *request = 
        "GET /templets/new/script/jquery.snippet.js#fragment HTTP/1.1\r\n"
        "Host: c.biancheng.net\r\n"
        "Proxy-Connection: keep-alive\r\n"
        "Pragma: no-cache\r\n"
        "Cache-Control: no-cache\r\n"
        "Accept: text/javascript, application/javascript, application/ecmascript, application/x-ecmascript, */*; q=0.01\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36\r\n"
        "X-Requested-With: XMLHttpRequest\r\n"
        "Referer: http://c.biancheng.net/view/7918.html\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n";

    http_request_parser parser;
    http_request_parser_init(&parser);
    parser.request_method = on_request_method;
    parser.request_uri = on_request_uri;
    parser.fragment = on_request_fragment;
    parser.request_path = on_request_path;
    parser.query_string = on_request_query;
    parser.http_version = on_request_version;
    parser.header_done = on_request_header_done;
    parser.http_field = on_request_http_field;
    parser.data = nullptr;

    http_request_parser_execute(&parser, request, strlen(request), 0);
    return 0;
}