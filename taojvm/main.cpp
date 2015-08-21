//
//  main.cpp
//  taojvm
//
//  Created by yintao on 15/8/16.
//  Copyright (c) 2015年 yintao. All rights reserved.
//

#include <iostream>
#include <fstream>
#include   <string.h>
#include   <string>
#include "jvm.h"
#include "class_loader.h"
using namespace std;
//const char* FILE_PATH = "D:\\git\\jvm\\taojvm\\Act.class";
//const char* FILE_PATH = "D:\\workspace\\untitled\\out\\production\\untitled\\Test.class";
//const char* FILE_PATH = "C:\\Users\\yintao\\Desktop\\deepjvm\\classlife\\ex8\\MyThread.class";
//const char* FILE_PATH = "/Users/yintao/Desktop/深入java虚拟机/applets/GettingLoaded/Act.class";
const char* FILE_PATH = "/Users/yintao/Documents/workspace/untitled/out/production/untitled/Main.class";
u1* fileContent;
File file;
ClassFile classFile;

//void read(ifstream ,char* ,int )；
int main(int argc, const char * argv[]) {
    file.file_path = FILE_PATH;
    readFile(file);
    parse(classFile,file);
        return 0;
    // insert code here...
    u4 magic = -1;
    u2 minorVersion  = -1;
    u2 majorVersion  = -1;
    u2 const_pool_count = -1;
    u1 tag = -1;
    u2 nameIndex = -1;
    std::ifstream fin(FILE_PATH, std::ios::binary);

    if(!fin.is_open()){
        return -1;
    }
    fin.close();
    std::cout << hex<<magic<<dec<<" minor:"<<minorVersion<<" major:"<<majorVersion<<endl;
    std::cout << " const_pool_count:"<<const_pool_count<<" tag:"<<hex<<tag+0<<endl;
    std::cout << "name_index:" <<nameIndex<<" :\u0777\u0777"<<endl;
    printf("%d",tag);
    if(magic == 0xbebafeca){
        std::cout<<"right";
    }
    std::cout<<sizeof(tag);
    ConstantInfoSt ci;
    ci.index =const_pool_count;
    ci.base = new CpInfo[const_pool_count-1];
    return 0;
}