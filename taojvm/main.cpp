//
//  main.cpp
//  taojvm
//
//  Created by yintao on 15/8/16.
//  Copyright (c) 2015年 yintao. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "jvm.h"
using namespace std;
const char* FILE_PATH = "/Users/yintao/Desktop/深入java虚拟机/applets/GettingLoaded/Act.class";

void read(ifstream* fin,char* address,int size){
    for(int i =size-1;i>=0;i--){
        (*fin).read(&address[i],1);
    }
}
//void read(ifstream ,char* ,int )；
int main(int argc, const char * argv[]) {
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
    //fin.read((char*)&magic,sizeof(magic));
    read(&fin,(char*)&magic,sizeof(magic));
    read(&fin,(char*)&minorVersion,sizeof(minorVersion));
    read(&fin,(char*)&majorVersion,sizeof(majorVersion));
    read(&fin,(char*)&const_pool_count,sizeof(const_pool_count));
    read(&fin,(char*)&tag,sizeof(tag));
    read(&fin,(char*)&nameIndex,sizeof(nameIndex));
    fin.close();
    std::cout << hex<<magic<<dec<<" minor:"<<minorVersion<<" major:"<<majorVersion<<endl;
    std::cout << " const_pool_count:"<<const_pool_count<<" tag:"<<hex<<tag+0<<endl;
    std::cout << "name_index:" <<nameIndex<<" :\u0777\u0777"<<endl;
    printf("%d",tag);
    if(magic == 0xbebafeca){
        std::cout<<"right";
    }
    std::cout<<sizeof(tag);
    ConstantInfo ci;
    ci.index =const_pool_count;
    ci.base = new CpInfo[const_pool_count-1];
    return 0;
}