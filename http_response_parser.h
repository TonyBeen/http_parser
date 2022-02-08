/*************************************************************************
    > File Name: http_response_parser.h
    > Author: hsz
    > Brief:
    > Created Time: Sun 06 Feb 2022 11:08:08 PM CST
 ************************************************************************/

#ifndef __HTTPD_HTTP_RESPONSE_H__
#define __HTTPD_HTTP_RESPONSE_H__

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <error.h>

typedef void (*element_cb)(void *user_data, const char *at, size_t length);
typedef void (*field_cb)(void *user_data, const char *field, size_t flen, const char *value, size_t vlen);

typedef struct http_response_parser {
    const char *begin;      // 内容开始
    size_t      len;        // 内容长度
    int32_t     cs;         // 错误标志

    const char *field_begin;// 键开始
    size_t      field_len;  // 键长度
    const char *value_begin;// 值开始
    size_t      value_len;  // 值长度

    void *user_data;        // 用户数据

    element_cb http_version;    // http版本回调
    element_cb http_status;     // http状态码回调
    element_cb http_reason;     // http原因回调
    element_cb http_data;       // http响应数据回调
    field_cb   http_field;      // http响应的键值对回调

    const char *response_data;  // http响应数据起始位置
    size_t      response_len;   // http响应数据的长度
} http_response_parser;

void    http_response_parser_init(http_response_parser *parser, void *user_data);
void    http_response_parser_execute(http_response_parser *parser, const char *buffer, size_t len, size_t off);
bool    http_response_parser_finished(http_response_parser *parser);
bool    http_response_parser_has_error(http_response_parser *parser);

#endif // __HTTPD_HTTP_RESPONSE_H__
