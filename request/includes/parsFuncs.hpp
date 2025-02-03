#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#define DATA_DIR "./data/"

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

std::string trim(std::string& str);
void toupper(std::string& str);
bool urlFormat(std::string& url);