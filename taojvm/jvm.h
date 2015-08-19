//
//  constant_info.h
//  taojvm
//
//  Created by yintao on 15/8/19.
//  Copyright (c) 2015年 yintao. All rights reserved.
//
#ifndef JVM_H
#define JVM_H
typedef unsigned int u4;
typedef unsigned short u2;
typedef unsigned char u1;
#ifdef __x86_64__
typedef unsigned long u8;
#elif __i386__
typedef unsigned long long u8;
#endif

struct CpInfo {
    u1 tag;
    u1 info[];
}__attribute__ ((packed));

struct ConstantInfoSt {
    u2 index;
    CpInfo *base;
}__attribute__ ((packed));
struct AttributeInfo {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 *info;
}__attribute__ ((packed));
struct FieldInfo {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo *attributes;
}__attribute__ ((packed));
struct MethodInfo {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo *attributes;
}__attribute__ ((packed));
struct ClassFile {
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    CpInfo *constant_pool;
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;
    u2 fields_count;
    FieldInfo *fields;
    u2 methods_count;
    MethodInfo *methods;
    u2 attributes_count;
    AttributeInfo *attributes;
}__attribute__ ((packed));
struct File{
    const char *file_path;
    char *content;
    long size;
};
#define CLASS_READ_U4(s, p)                             \
        do {                                            \
                s = (((p >> 24) & 0x000000ff) |         \
                        ((p >> 8) & 0x0000ff00) |       \
                        ((p << 24) & 0xff000000) |      \
                        ((p << 8) & 0x00ff0000));       \
        } while (0);

#define CLASS_READ_U2(s, p)                             \
        do {                                            \
                s = (((p >> 8) & 0x00ff) |              \
                        ((p << 8) & 0xff00));           \
        } while (0);

#define CLASS_READ_U1(s, p)                             \
        do {                                            \
                s = p;                                  \
        } while (0);

#define CONSTANT_Class                                  7
#define CONSTANT_Fieldref                               9
#define CONSTANT_Methodref                              10
#define CONSTANT_InterfaceMethodref                     11
#define CONSTANT_String                                 8
#define CONSTANT_Integer                                3
#define CONSTANT_Float                                  4
#define CONSTANT_Long                                   5
#define CONSTANT_Double                                 6
#define CONSTANT_NameAndType                            12
#define CONSTANT_Utf8                                   1
#define CONSTANT_MethodHandle                           15
#define CONSTANT_MethodType                             16
#define CONSTANT_InvokeDynamic                          18

struct CONSTANT_Class_info {
    //u1 tag;
    u2 name_index;
}__attribute__ ((packed));

struct CONSTANT_Fieldref_info {
    //u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}__attribute__ ((packed));

struct CONSTANT_Methodref_info {
    //u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}__attribute__ ((packed));

struct CONSTANT_InterfaceMethodref_info {
    //u1 tag;
    u2 class_index;
    u2 name_and_type_inex;
}__attribute__ ((packed));

struct CONSTANT_String_info {
    //u1 tag;
    u2 string_index;
}__attribute__ ((packed));

struct CONSTANT_Integer_info {
    //u1 tag;
    u4 bytes;
}__attribute__ ((packed));

struct CONSTANT_Float_info {
    //u1 tag;
    u4 bytes;
}__attribute__ ((packed));

struct CONSTANT_Long_info {
    //u1 tag;
    u4 high_bytes;
    u4 low_bytes;
}__attribute__ ((packed));

struct CONSTANT_Double_info {
    //u1 tag;
    u4 high_bytes;
    u4 low_bytes;
}__attribute__ ((packed));

struct CONSTANT_NameAndType_info {
    //u1 tag;
    u2 name_index;
    u2 descriptor_index;
}__attribute__ ((packed));

struct CONSTANT_Utf8_info {
    //u1 tag;
    u2 length;
    u1 bytes[];
}__attribute__ ((packed));

struct CONSTANT_MethodHandle_info {
    //u1 tag;
    u1 reference_kind;
    u2 reference_index;
}__attribute__ ((packed));

struct CONSTANT_MethodType_info {
    //u1 tag;
    u2 descriptor_index;
}__attribute__ ((packed));

struct CONSTANT_InvokeDynamic_info {
    //u1 tag;
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
}__attribute__ ((packed));
#endif
