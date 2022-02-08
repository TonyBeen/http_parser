#include "http_response_parser.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void on_http_version(void *data, const char *begin, size_t len)
{
    string temp;
    temp.append(begin, len);
    cout << __func__ << "() [" << temp << "]" << endl;
}

void on_http_status(void *data, const char *begin, size_t len)
{
    string temp;
    temp.append(begin, len);
    cout << __func__ << "() [" << temp << "]" << endl;
}

void on_http_reason(void *data, const char *begin, size_t len)
{
    string temp;
    temp.append(begin, len);
    cout << __func__ << "() [" << temp << "]" << endl;
}

void on_http_data(void *data, const char *begin, size_t len)
{
    string temp;
    temp.append(begin, len);
    cout << __func__ << "() [" << temp << "]" << endl;
}

void on_http_field(void *data, const char *filed, size_t flen, const char *value, size_t vlen)
{
    string temp;
    temp.append(filed, flen);
    temp.append(": ");
    temp.append(value, vlen);
    cout << __func__ << "() [" << temp << "]" << endl;
}

int main(int argc, char **argv)
{
    const char *str = 
        "HTTP/1.0 207 Multi-Status\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 5299\r\n"
        "Content-Type: text/html\r\n"
        "Server: eular/httpd v1.0\r\n\r\n"
        "{\"code\":2,\"message\":\"/home.html\"}\0";
    
    const char *str2 = 
        "HTTP/1.1 200 OK\r\n"
        "Server: nginx\r\n"
        "Date: Tue, 08 Feb 2022 14:51:22 GMT\r\n"
        "Content-Type: application/json; charset=utf-8\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Connection: keep-alive\r\n"
        "Set-Cookie: PHPSESSID=123nras8ubbobo71be1mpdnjhc; path=/; HttpOnly\r\n"
        "Expires: Thu, 19 Nov 1981 08:52:00 GMT\r\n"
        "Cache-Control: no-store, no-cache, must-revalidate\r\n"
        "Pragma: no-cache\r\n\r\n"
        "{\"code\":200,\"message\":\"success\",\"data\":{\"country\":\"中国\",\"province\":\"天津\",\"city\":\"天津\",\"service\":\"电信\"}}";

    string temp = str;

    http_response_parser parser;
    http_response_parser_init(&parser, nullptr);
    parser.http_field = on_http_field;
    parser.http_status = on_http_status;
    parser.http_reason = on_http_reason;
    parser.http_version = on_http_version;
    parser.http_data = on_http_data;

    printf("strlen: %zu\n", strlen(str));

    http_response_parser_execute(&parser, temp.c_str(), temp.length(), 0);
    if (http_response_parser_has_error(&parser)) {
        printf("has error\n");
    }

    temp = str2;
    http_response_parser_init(&parser, nullptr);
    parser.http_field = on_http_field;
    parser.http_status = on_http_status;
    parser.http_reason = on_http_reason;
    parser.http_version = on_http_version;
    parser.http_data = on_http_data;
    http_response_parser_execute(&parser, temp.c_str(), temp.length(), 0);
    if (http_response_parser_has_error(&parser)) {
        printf("has error\n");
    }

    return 0;
}