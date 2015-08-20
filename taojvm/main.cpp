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
using namespace std;
//const char* FILE_PATH = "D:\\git\\jvm\\taojvm\\Act.class";
//const char* FILE_PATH = "D:\\workspace\\untitled\\out\\production\\untitled\\Test.class";
//const char* FILE_PATH = "C:\\Users\\yintao\\Desktop\\deepjvm\\classlife\\ex8\\MyThread.class";
//const char* FILE_PATH = "/Users/yintao/Desktop/深入java虚拟机/applets/GettingLoaded/Act.class";
const char* FILE_PATH = "/Users/yintao/Documents/workspace/untitled/out/production/untitled/Main.class";
u1* fileContent;
File file;
ClassFile classFile;
const char* constantTranslate(CLASS_FILE_CONSTANT_TAG class_file_constant){
    switch (class_file_constant){
        case CONSTANT_Class:
            return "CONSTANT_Class_Info";
        case CONSTANT_String:
            return "CONSTANT_String_Info";
        case CONSTANT_MethodType:
            return "CONSTANT_MethodType_Info";
        case CONSTANT_Fieldref:
            return "CONSTANT_Fieldref_Info";
        case CONSTANT_Methodref:
            return "CONSTANT_Methodref_Info";
        case CONSTANT_NameAndType:
            return "CONSTANT_NameAndType_Info";
        case CONSTANT_InterfaceMethodref:
            return "CONSTANT_InterfaceMethodref_Info";
        case CONSTANT_InvokeDynamic:
            return "CONSTANT_InvokeDynamic_Info";
        case CONSTANT_Integer:
            return "CONSTANT_Integer_Info";
        case CONSTANT_Float:
            return "CONSTANT_Float_Info";
        case CONSTANT_Long:
            return "CONSTANT_Long_Info";
        case CONSTANT_Double:
            return "CONSTANT_Double_Info";
        case CONSTANT_Utf8:
            return "CONSTANT_Utf8_Info";
        case CONSTANT_MethodHandle:
            return "CONSTANT_MethodHandle_Info";
        default:
            return "unknow constant";
    }
}
const char* accessFlagsTranslate(u2 accessFlags){
    string out;
    out.clear();
    if(accessFlags & ACC_PUBLIC){
        out+="ACC_PUBLIC ";
    }
    if(accessFlags & ACC_FINAL){
        out+="ACC_FINAL ";
    }
    if(accessFlags & ACC_SUPER){
        out+="ACC_SUPER ";
    }
    if(accessFlags & ACC_INTERFACE){
        out+="ACC_INTERFACE ";
    }
    if(accessFlags & ACC_ABSTRACT){
        out+="ACC_ABSTRACT ";
    }
    if(accessFlags & ACC_SYNTHETIC){
        out+="ACC_SYNTHETIC ";
    }
    if(accessFlags & ACC_ANNOTATION){
        out+="ACC_ANNOTATION ";
    }
    if(accessFlags & ACC_ENUM){
        out+="ACC_ENUM ";
    }
    return out.c_str();
}
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
void parseConstantPool(ClassFile &classFile,char* &startIndex){

    classFile.constant_pool = new CpInfo[classFile.constant_pool_count-1];
    u2 index;
    for(int i=0;i<classFile.constant_pool_count-1;i++){
        classFile.constant_pool[i].tag =*(u1 *)startIndex;
        startIndex+=1;
        switch (classFile.constant_pool[i].tag ){
            case CONSTANT_Class:
            case CONSTANT_String:
            case CONSTANT_MethodType:
                CONSTANT_String_info constant_string_info;
                CLASS_READ_U2(constant_string_info.string_index,*(u2 *)startIndex);
                printf("#%d = %s string_index:%d\n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),constant_string_info.string_index);
                startIndex+=2;
                classFile.constant_pool[i].info = (u1 *)&constant_string_info;
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_NameAndType:
            case CONSTANT_InterfaceMethodref:
                CONSTANT_Fieldref_info constant_fieldref_info;
                CLASS_READ_U2(constant_fieldref_info.class_index,*(u2 *)startIndex);
                startIndex+=2;
                CLASS_READ_U2(constant_fieldref_info.name_and_type_index,*(u2 *)startIndex);
                startIndex+=2;
                printf("#%d = %s class_index:%d name_and_type_index:%d\n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),constant_fieldref_info.class_index,constant_fieldref_info.name_and_type_index);
                classFile.constant_pool[i].info = (u1 *)&constant_fieldref_info;
                break;
            case CONSTANT_InvokeDynamic:
                CONSTANT_InvokeDynamic_info constant_invokeDynamic_info;
                CLASS_READ_U2(constant_invokeDynamic_info.bootstrap_method_attr_index,*(u2 *)startIndex);
                startIndex+=2;
                CLASS_READ_U2(constant_invokeDynamic_info.name_and_type_index,*(u2 *)startIndex);
                startIndex+=2;
                printf("#%d = %s bootstrap_method_attr_index:%d name_and_type_index:%d\n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),constant_invokeDynamic_info.bootstrap_method_attr_index,constant_invokeDynamic_info.name_and_type_index);
                classFile.constant_pool[i].info = (u1 *)&constant_invokeDynamic_info;
                break;
            case CONSTANT_Integer:
                CONSTANT_Integer_info constant_integer_info;
                CLASS_READ_U4(constant_integer_info.bytes,*(u4 *)startIndex);
                startIndex+=4;
                printf("#%d = %s bytes:%d \n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),constant_integer_info.bytes);
                classFile.constant_pool[i].info = (u1 *)&constant_integer_info;
                break;
            case CONSTANT_Float:
                CONSTANT_Float_info constant_float_info;
                CLASS_READ_U4(constant_float_info.bytes,*(u4 *)startIndex);
                startIndex+=4;
                printf("#%d = %s bytes:%f \n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),*(float*)(&constant_float_info.bytes));
                classFile.constant_pool[i].info = (u1 *)&constant_integer_info;
                break;
            case CONSTANT_Long:
                CONSTANT_Long_info constant_long_info;
                CLASS_READ_U4(constant_long_info.low_bytes,*(u4 *)startIndex);
                startIndex+=4;
                CLASS_READ_U4(constant_long_info.high_bytes,*(u4 *)startIndex);
                startIndex+=4;
                printf("#%d = %s value %ld\n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),*(long*)(&constant_long_info.high_bytes));
                classFile.constant_pool[i].info = (u1 *)&constant_long_info;
                i++;//由于LONG跟Double占两个entries
                break;
            case CONSTANT_Double:
                CONSTANT_Double_info constant_double_info;
                CLASS_READ_U4(constant_double_info.low_bytes,*(u4 *)startIndex);
                startIndex+=4;
                CLASS_READ_U4(constant_double_info.high_bytes,*(u4 *)startIndex);
                startIndex+=4;
                printf("#%d = %s value %f\n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),*(double*)(&constant_double_info.high_bytes));
                classFile.constant_pool[i].info = (u1 *)&constant_double_info;
                i++;//由于LONG跟Double占两个entries
                break;
            case CONSTANT_Utf8:
                CONSTANT_Utf8_info constant_utf8_info;
                CLASS_READ_U2(constant_utf8_info.length,*(u2 *)startIndex);
                startIndex+=2;
                constant_utf8_info.bytes = new u1[constant_utf8_info.length+1];
                constant_utf8_info.bytes[constant_utf8_info.length] = '\0';
                memcpy(constant_utf8_info.bytes, startIndex, constant_utf8_info.length);
                startIndex+=constant_utf8_info.length;
                printf("#%d = %s len %d\t%s\n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),constant_utf8_info.length,constant_utf8_info.bytes);
                classFile.constant_pool[i].info = (u1 *)&constant_utf8_info;
                break;
            case CONSTANT_MethodHandle:
                CONSTANT_MethodHandle_info constant_methodHandle_info;
                constant_methodHandle_info.reference_kind = *(u1 *)startIndex;
                startIndex+=2;
                CLASS_READ_U2(constant_methodHandle_info.reference_index,*(u2 *)startIndex);
                startIndex+=2;
                printf("#%d = %s reference_kind：%d reference_index：%d\n",i+1,constantTranslate((CLASS_FILE_CONSTANT_TAG)classFile.constant_pool[i].tag),constant_methodHandle_info.reference_kind,constant_methodHandle_info.reference_index);
                classFile.constant_pool[i].info = (u1 *)&constant_methodHandle_info;
                break;
            default:
                cout<<"#"<<i+1<<" = unknow tag:"<<classFile.constant_pool[i].tag+0<<endl;
                break;
        }
    }
}
void paresInterface(ClassFile &classFile,char* &startIndex){
    CLASS_READ_U2(classFile.interfaces_count,*(u2 *)startIndex);
    startIndex+=2;
    cout<<"interfaces_count:"<<classFile.interfaces_count<<endl;
    cout<<"interfaces:";
    if(classFile.interfaces_count>0){
        u2* interfaces = new u2[classFile.interfaces_count];
        for(int i=0;i<classFile.interfaces_count;i++){
            CLASS_READ_U2(interfaces[i],*(u2 *)startIndex);
            startIndex+=2;
            cout<<interfaces[i]<<" ";
        }
        classFile.interfaces= interfaces;
        cout<<endl;
    }else{
        classFile.interfaces = NULL;
        cout<<"null"<<endl;
    }
}
void parseAttribute(AttributeInfo *attributes ,u2 &attributes_count,char* &startIndex){
    CLASS_READ_U2(attributes_count,*(u2 *)startIndex);
    startIndex+=2;
    cout<<"attributes_count:"<<attributes_count<<endl;
    attributes = new AttributeInfo[attributes_count];
    for(int i=0;i<attributes_count;i++ ){
        CLASS_READ_U2(attributes[i].attribute_name_index,*(u2 *)startIndex);
        cout<<"#"<<i<< " = attribute_name_index:"<<attributes[i].attribute_name_index<<endl;
        startIndex+=2;
        CLASS_READ_U4(attributes[i].attribute_length,*(u4 *)startIndex);
        cout<<"#"<<i<< " = attribute_length:"<<attributes[i].attribute_length<<endl;
        startIndex+=4;
        attributes[i].info = (u1 *)startIndex;
        startIndex+=attributes[i].attribute_length;
    }

}
void parseField(ClassFile &classFile,char* &startIndex){
    CLASS_READ_U2(classFile.fields_count,*(u2 *)startIndex);
    startIndex+=2;
    cout<<"fields_count:"<<classFile.fields_count<<endl;
    cout<<"field_info:";
    if(classFile.fields_count>0){
        FieldInfo* fieldInfo = new FieldInfo[classFile.fields_count];
        for(int i=0;i<classFile.fields_count;i++){
            cout<<"#"<<i<<" = ";
            CLASS_READ_U2(fieldInfo[i].access_flags,*(u2 *)startIndex);
            startIndex+=2;
            printf("access_flags: %s",accessFlagsTranslate(fieldInfo[i].access_flags));
            //cout<<"access_flags:"<<accessFlagsTranslate(fieldInfo[i].access_flags)<<" ";
            CLASS_READ_U2(fieldInfo[i].name_index,*(u2 *)startIndex);
            startIndex+=2;
            cout<<"name_index:"<<fieldInfo[i].name_index<<"";
            CLASS_READ_U2(fieldInfo[i].descriptor_index,*(u2 *)startIndex);
            startIndex+=2;
            cout<<"descriptor_index:"<<fieldInfo[i].descriptor_index<<endl;
            parseAttribute(fieldInfo[i].attributes,fieldInfo[i].attributes_count,startIndex);

        }
        classFile.fields= fieldInfo;
        cout<<endl;
    }else{
        classFile.fields = NULL;
        cout<<"null"<<endl;
    }
}
void parseMethod(ClassFile &classFile,char* &startIndex){
    CLASS_READ_U2(classFile.methods_count,*(u2 *)startIndex);
    startIndex+=2;
    cout<<"methods_count:"<<classFile.methods_count<<endl;
    cout<<"method_info:";
    if(classFile.methods_count>0){
        MethodInfo* methodInfo = new MethodInfo[classFile.methods_count];
        for(int i=0;i<classFile.methods_count;i++){
            cout<<"#"<<i<<" = ";
            CLASS_READ_U2(methodInfo[i].access_flags,*(u2 *)startIndex);
            startIndex+=2;
            printf("access_flags: %s",accessFlagsTranslate(methodInfo[i].access_flags));
            //cout<<"access_flags:"<<accessFlagsTranslate(methodInfo[i].access_flags)<<" ";
            CLASS_READ_U2(methodInfo[i].name_index,*(u2 *)startIndex);
            startIndex+=2;
            cout<<"name_index:"<<methodInfo[i].name_index<<"";
            CLASS_READ_U2(methodInfo[i].descriptor_index,*(u2 *)startIndex);
            startIndex+=2;
            cout<<"descriptor_index:"<<methodInfo[i].descriptor_index<<endl;
            parseAttribute(methodInfo[i].attributes,methodInfo[i].attributes_count,startIndex);

        }
        classFile.methods= methodInfo;
        cout<<endl;
    }else{
        classFile.methods = NULL;
        cout<<"null"<<endl;
    }
}
void parse(ClassFile &classFile,File file){
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
    cout<<"file size:"<<file.size<<endl;
    cout<<hex<<"magic:"<<classFile.magic<<endl;
    cout<<dec<<" minor:"<<classFile.minor_version<<" major:"<<classFile.major_version<<" const_pool_count:"<<classFile.constant_pool_count<<endl;

    parseConstantPool(classFile,startIndex);
    CLASS_READ_U2(classFile.access_flags,*(u2 *)startIndex);
    startIndex+=2;
    printf("access_flags: %s",accessFlagsTranslate(classFile.access_flags));
    CLASS_READ_U2(classFile.this_class,*(u2 *)startIndex);
    startIndex+=2;
    cout<<"this_class:"<<classFile.this_class<<endl;
    CLASS_READ_U2(classFile.super_class,*(u2 *)startIndex);
    startIndex+=2;
    cout<<"super_class:"<<classFile.super_class<<endl;
    paresInterface(classFile,startIndex);
    parseField(classFile,startIndex);
    parseMethod(classFile,startIndex);
    parseAttribute(classFile.attributes,classFile.attributes_count,startIndex);
}
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