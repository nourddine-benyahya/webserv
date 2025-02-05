#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>


#define DATA_DIR "./data/"
#define CGI_DIR "./data/cgi-files"
#define PHP_CGI "./cgi-bin/php-cgi"

enum MethodType {
    GET,
    HEAD,
    OPTIONS,
    TRACE,
    PUT,
    DELETE,
    POST,
    PATCH,
    CONNECT,
};

enum dataType
{
    TEXT,
    XML,
    HTML,
    JSON,
    JAVA_SCRIPT,
    OCTET_STREAM,
    URLENCODED,
    FORM_DATA,
    NONE,
};

enum Err
{
    FILE_NOT_FOUND,
    SCRIPT_EXEC_ERROR,
};

std::string trim(std::string& str);
bool fileExists(const char *path);
void toupper(std::string& str);
bool urlFormat(std::string& url);