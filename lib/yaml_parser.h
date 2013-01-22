#ifndef YAML_PARSER_H
#define YAML_PARSER_H

// Copyright (c) 2011 iRacing.com Motorsport Simulations, LLC.
// All rights reserved.

// super simple YAML parser
bool parseYaml(const char *data, const char* path, const char **val, int *len);

#endif //YAML_PARSER_H
