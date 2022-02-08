#include "http_response_parser.h"

#include <assert.h>

%%{
    machine http_response_parser;

    action mark {
        parser->begin = fpc;
    }

    action done {
        printf("parser done\n");
        // fbreak会使变量p++;
    }

    action http_version {
        if (parser->http_version) {
            parser->len = fpc - parser->begin;
            parser->http_version(parser->user_data, parser->begin, parser->len);
        }
        // temp.clear();
        // len = fpc - begin;
        // temp.append(begin, len);
        // cout << "http_version() [" << temp << "]" << endl;
    }

    action http_status {
        if (parser->http_status) {
            parser->len = fpc - parser->begin;
            parser->http_status(parser->user_data, parser->begin, parser->len);
        }
    }

    action http_status_reason {
        if (parser->http_reason) {
            parser->len = fpc - parser->begin;
            parser->http_reason(parser->user_data, parser->begin, parser->len);
        }
    }

    action write_field {
        parser->field_begin = parser->begin;
        parser->field_len = fpc - parser->begin;
    }

    action write_value {
        parser->value_begin = parser->begin;
        parser->value_len = fpc - parser->begin;
    }

    action write_field_value {
        if (parser->http_field) {
            parser->http_field(parser->user_data, parser->field_begin,
                parser->field_len, parser->value_begin, parser->value_len);
        }
    }

    action http_set_data {
        parser->response_data = parser->begin;
        parser->response_len  = fpc - parser->begin;
        if (parser->http_data) {
            parser->http_data(parser->user_data, parser->response_data, parser->response_len);
        }
    }

    CRLF = ("\r\n" | "\n");
    HTTP_CTL = (0 - 31) | 127 ;
    HTTP_separator = ( "(" | ")" | "<" | ">" | "@"
                     | "," | ";" | ":" | "\\" | "\""
                     | "/" | "[" | "]" | "?" | "="
                     | "{" | "}" | " " | "\t" ) ;
    lws = CRLF? (" " | "\t")+ ;

    # for Response_Line
    http_number = ( "1." ("0" | "1") ) ;
    HTTP_Version = ("HTTP/" http_number) >mark %http_version;
    http_status = (digit{3}) >mark %http_status;
    reason = (alpha | " " | "-");
    http_status_desc = reason* >mark %http_status_reason;

    Response_Line = (HTTP_Version " " http_status " " http_status_desc CRLF);

    # for message_header
    content = ((any -- HTTP_CTL) | lws);
    token = ascii -- ( HTTP_CTL | HTTP_separator ) ;
    field_name = ( token )+ >mark %write_field;
    field_value = content* >mark %write_value;

    message_header = field_name ":" lws* field_value :> CRLF %write_field_value;

    data_content = ascii -- (HTTP_CTL);
    http_data = data_content* >mark %http_set_data;

    main := Response_Line ( message_header )* ( CRLF ) (http_data) %done;
    write data;
}%%

void http_response_parser_init(http_response_parser *parser, void *user_data)
{
    int cs = 0;
    %% write init;
    if (parser) {
        parser->begin       = nullptr;
        parser->len         = 0;
        parser->user_data   = user_data;
        parser->cs          = cs;

        parser->http_version = nullptr;
        parser->http_status  = nullptr;
        parser->http_reason  = nullptr;
        parser->http_field   = nullptr;

        parser->response_data = nullptr;
        parser->response_len  = 0;
    }
}

void http_response_parser_execute(http_response_parser *parser, const char *buffer, size_t len, size_t off)
{
    if (parser == nullptr) {
        return;
    }
    
    if (buffer == nullptr) {
        parser->cs = -EINVAL;
    }

    const char *p, *pe, *eof;
    int cs = parser->cs;

    assert(off <= len && "offset past end of buffer");

    p   = buffer + off;
    pe  = buffer + len;
    eof = pe;

    assert(pe - p == (int)(len - off) && "pointers aren't same distance");
    %% write exec;
    assert(p <= pe && "Buffer overflow after parsing.");

    if (!http_response_parser_has_error(parser)) {
        parser->cs = cs;
    }
}

bool http_response_parser_finished(http_response_parser *parser)
{
    return parser->cs >= http_response_parser_first_final;
}

bool http_response_parser_has_error(http_response_parser *parser)
{
    return parser->cs == http_response_parser_error;
}