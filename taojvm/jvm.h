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

struct CpInfo{
    u1 tag;
    u1 info[];
}__attribute__ ((packed));

struct ConstantInfo{
    u2 index;
    CpInfo *base;
}__attribute__ ((packed));
#endif
