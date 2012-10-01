#include <v8.h>
#include <node.h>

#ifndef MAIN_H
#define MAIN_H

void init(v8::Handle<v8::Object> target);
NODE_MODULE(nodebind, init)

#endif