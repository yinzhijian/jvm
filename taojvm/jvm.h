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
#endif
