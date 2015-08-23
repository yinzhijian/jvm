//
//  main.cpp
//  taojvm
//
//  Created by yintao on 15/8/16.
//  Copyright (c) 2015年 yintao. All rights reserved.
//

#include <iostream>
#include "jvm.h"
#include "class_loader.h"
#include "execution_engine.h"

using namespace std;
//const char* FILE_PATH = "D:\\git\\jvm\\taojvm\\Act.class";
//const char* FILE_PATH = "D:\\workspace\\untitled\\out\\production\\untitled\\Main.class";
//const char* FILE_PATH = "C:\\Users\\yintao\\Desktop\\deepjvm\\classlife\\ex8\\MyThread.class";
//const char* FILE_PATH = "/Users/yintao/Desktop/深入java虚拟机/applets/GettingLoaded/Act.class";
const char* FILE_PATH = "/Users/yintao/Documents/workspace/untitled/out/production/untitled/Main.class";
u1* fileContent;
File file;
ClassFile classFile;

//void read(ifstream ,char* ,int )；
int main(int argc, const char * argv[]) {
    ClassLoader *classLoader = new ClassLoader(FILE_PATH);
    printf("qwe");
    Runtime *runtime = new Runtime;
    ClassFile *classFile = classLoader->getClassFile();
    printf("rty");
    // u2 thisClassIndex = *(u2 *)(classFile->constant_pool[(classFile->this_class)].info);
    // printf("uio this%d,%d",classFile->this_class,thisClassIndex);
    // printf("tag:%d",classFile->constant_pool[thisClassIndex].tag);
    // printf("name %s",(const char *)((CONSTANT_Utf8_info *)(classFile->constant_pool[thisClassIndex].info))->bytes);
    //string name((const char *)((CONSTANT_Utf8_info *)(classFile->constant_pool[thisClassIndex].info))->bytes);
    printf("asdf");
    runtime->classFiles["Main"] = *classFile;
    printf("ghj");
    runtime->currentClassFile = classFile;
    runtime->currentMethod = NULL;
    printf("kl;");
    for (u2 mIndex = 0; mIndex < classFile->methods_count; mIndex++) {
        if (!strcmp((const char *) classFile->methods[mIndex].name, "main")) {
            runtime->currentMethod = &classFile->methods[mIndex];
            cout << "found main method!";
            break;
        }
    }
    if (runtime->currentMethod == NULL) {
        cerr << "not found main method!";
        return -1;
    }
    runtime->pc = 0;
    runtime->currentStackFrame = runtime->currentMethod->jvmStackFrame;
    runtime->currentMethodDeep = 1;
    new ExecutionEngine(runtime);
    return 0;
}