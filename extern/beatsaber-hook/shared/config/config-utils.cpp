#ifndef CONFIG_DEFINED_H
#define CONFIG_DEFINED_H

#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "config-utils.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

// CONFIG

ConfigDocument Configuration::config;
bool readJson = false;

using namespace rapidjson;

// Loads the config for the given MOD_ID, if it doesn't exist, will leave it as an empty object.
void Configuration::Load() {
    if (readJson) {
        return;
    }
    // document = {};
    if (!direxists(CONFIG_PATH)) {
        mkdir(CONFIG_PATH, 0700);
    }
    std::string filename = getconfigpath();

    if (!fileexists(filename.c_str())) {
        writefile(filename.c_str(), "{}");
    }
    if (!parsejsonfile(config, filename)) {
        readJson = false;
    }
    if (!config.IsObject()) {
        config.SetObject();
    }
    readJson = true;
}

void Configuration::Reload() {
    std::string filename = getconfigpath();

    parsejsonfile(config, filename);
    if (!config.IsObject()) {
        config.SetObject();
    }
    readJson = true;
}

void Configuration::Write() {
    if (!direxists(CONFIG_PATH)) {
        mkdir(CONFIG_PATH, 0700);
    }
    if (!config.IsObject()) {
        config.SetObject();
    }
    std::string filename = getconfigpath();

    StringBuffer buf;
    PrettyWriter<StringBuffer> writer(buf);
    config.Accept(writer);
    writefile(filename.c_str(), buf.GetString());
}

bool parsejsonfile(ConfigDocument& doc, std::string filename) {
    if (!fileexists(filename.c_str())) {
        return false;
    }
    // FILE* fp = fopen(filename.c_str(), "r");

    std::ifstream is;
    is.open(filename.c_str());

    IStreamWrapper wrapper {is};
    
    return !doc.ParseStream(wrapper).HasParseError();
}

bool parsejson(ConfigDocument& doc, std::string_view js) {
    char temp[js.length()];
    memcpy(temp, js.data(), js.length());

    if (doc.ParseInsitu(temp).HasParseError()) {
        return false;
    }
    return true;
}

std::string getconfigpath() {
    return std::string(CONFIG_PATH) + MOD_ID + ".json";
}

#endif /* CONFIG_DEFINED_H */