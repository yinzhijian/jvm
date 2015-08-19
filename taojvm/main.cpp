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
//const char* FILE_PATH = "D:\\git\\jvm\\taojvm\\Act.class";
const char* FILE_PATH = "/Users/yintao/Desktop/深入java虚拟机/applets/GettingLoaded/Act.class";
u1* fileContent;
File file;
ClassFile classFile;

void readFile(File &file){
    filebuf *pbuf;
    ifstream filestr;

    // 要读入整个文件，必须采用二进制打开
    filestr.open (file.file_path, ios::binary);
    // 获取filestr对应buffer对象的指针
    pbuf=filestr.rdbuf();

    // 调用buffer对象方法获取文件大小
    file.size=pbuf->pubseekoff (0,ios::end,ios::in);
    pbuf->pubseekpos (0,ios::in);

    // 分配内存空间
    file.content=new char[file.size];

    // 获取文件内容
    pbuf->sgetn (file.content,file.size);

    filestr.close();
}
void passConstantPool(ClassFile &classFile,char* &startIndex){
    classFile.constant_pool = new CpInfo[classFile.constant_pool_count-1];
    for(int i=0;i<classFile.constant_pool_count-1;i++){
        classFile.constant_pool[i].tag = startIndex[0];
        switch (startIndex[0]){
            case CONSTANT_Class:
            case CONSTANT_String:

                classFile.constant_pool[i].info;
        }
    }
}
void pass(ClassFile &classFile,File file){
    long i=0;
    char* startIndex = file.content;
    CLASS_READ_U4(classFile.magic,*(u4 *)startIndex);
    startIndex+=4;
    CLASS_READ_U2(classFile.minor_version,*(u2 *)startIndex);
    startIndex+=2;
    CLASS_READ_U2(classFile.major_version,*(u2 *)startIndex);
    startIndex+=2;
    CLASS_READ_U2(classFile.constant_pool_count,*(u2 *)startIndex);
    startIndex+=2;


}
//void read(ifstream ,char* ,int )；
int main(int argc, const char * argv[]) {
    file.file_path = FILE_PATH;
    readFile(file);
    pass(classFile,file);
    cout<<file.size;
    cout<<hex<<classFile.magic<<endl;
    cout<<dec<<" minor:"<<classFile.minor_version<<" major:"<<classFile.major_version<<" const_pool_count:"<<classFile.constant_pool_count;
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