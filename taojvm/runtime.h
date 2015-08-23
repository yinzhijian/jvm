//
// Created by yintao on 2015/8/22.
//
#include "jvm.h"
#include <map>
#include <string>

using namespace std;
#ifndef TAOJVM_RUNTIME_H
#define TAOJVM_RUNTIME_H

class Runtime {
public:
    u2 pc;
    Method *currentMethod;
    JvmStackFrame *currentStackFrame;
    map<string, ClassFile> classFiles;
    ClassFile *currentClassFile;
    u2 currentMethodDeep;

};
#endif //TAOJVM_RUNTIME_H
