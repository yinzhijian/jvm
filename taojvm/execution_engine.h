//
// Created by yintao on 2015/8/22.
//
#include <map>
#include <iostream>
#include "jvm.h"
#include "runtime.h"

using namespace std;
#ifndef TAOJVM_EXECUTION_ENGINE_H
#define TAOJVM_EXECUTION_ENGINE_H
#define OPCODE_SYMBOL_LEN 10

class ExecutionEngine;

typedef void (ExecutionEngine::*PtrEEFunc)(u4, u2, const char *);

struct ByteCode {
    u4 opcode_val;//操作数的值

    u2 opcode_len;
    const char *symbol;
    PtrEEFunc func;

    ByteCode() { };

    ByteCode(u4 opcode_val,
             u2 opcode_len,
             const char symbol[],
             PtrEEFunc func) {
        this->opcode_val = opcode_val;
        this->opcode_len = opcode_len;
        this->symbol = symbol;
        this->func = func;
    };
};

class ExecutionEngine {
private:
    map<u2, ByteCode> m_byte_code_hash;
    Runtime *m_runtime;

    void initByteCode() {
        m_byte_code_hash[2] = ByteCode(-1, 1, "iconst_m1", &ExecutionEngine::execIConstI);
        m_byte_code_hash[3] = ByteCode(0, 1, "iconst_0", &ExecutionEngine::execIConstI);
        m_byte_code_hash[4] = ByteCode(1, 1, "iconst_1", &ExecutionEngine::execIConstI);
        m_byte_code_hash[5] = ByteCode(2, 1, "iconst_2", &ExecutionEngine::execIConstI);
        m_byte_code_hash[6] = ByteCode(3, 1, "iconst_3", &ExecutionEngine::execIConstI);
        m_byte_code_hash[7] = ByteCode(4, 1, "iconst_4", &ExecutionEngine::execIConstI);
        m_byte_code_hash[8] = ByteCode(5, 1, "iconst_5", &ExecutionEngine::execIConstI);

        m_byte_code_hash[0x1a] = ByteCode(0, 1, "iload_0", &ExecutionEngine::execILoadN);
        m_byte_code_hash[0x1b] = ByteCode(1, 1, "iload_1", &ExecutionEngine::execILoadN);
        m_byte_code_hash[0x1c] = ByteCode(2, 1, "iload_2", &ExecutionEngine::execILoadN);
        m_byte_code_hash[0x1d] = ByteCode(3, 1, "iload_3", &ExecutionEngine::execILoadN);

        m_byte_code_hash[0x3b] = ByteCode(0, 1, "istore_0", &ExecutionEngine::execIStoreN);
        m_byte_code_hash[0x3c] = ByteCode(1, 1, "istore_1", &ExecutionEngine::execIStoreN);
        m_byte_code_hash[0x3d] = ByteCode(2, 1, "istore_2", &ExecutionEngine::execIStoreN);
        m_byte_code_hash[0x3e] = ByteCode(3, 1, "istore_3", &ExecutionEngine::execIStoreN);

        m_byte_code_hash[0x60] = ByteCode(0, 1, "iadd", &ExecutionEngine::execIAdd);
        m_byte_code_hash[0x64] = ByteCode(0, 1, "isub", &ExecutionEngine::execISub);

        m_byte_code_hash[0xac] = ByteCode(0, 1, "ireturn", &ExecutionEngine::execIReturn);
        m_byte_code_hash[0xb1] = ByteCode(0, 1, "return", &ExecutionEngine::execReturn);
        m_byte_code_hash[0xb8] = ByteCode(0, 3, "invokestatic", &ExecutionEngine::execInvokeStatic);
    }

    void printCurrentInfo() {
        printf("current pc=%d\n", m_runtime->pc);
        printf("local =");
        for (int localIndex = 0; localIndex < m_runtime->currentStackFrame->max_locals; localIndex++) {
            printf("%x\t", m_runtime->currentStackFrame->local_var_table[localIndex]);
        }
        printf("stack =");
        for (int stackIndex = 0; stackIndex < m_runtime->currentStackFrame->max_stack; stackIndex++) {
            printf("%x\t", m_runtime->currentStackFrame->operand_stack[stackIndex]);
        }
        printf("\n");
    }

    void eraseStackAndVarTable() {
        for (int localIndex = 0; localIndex < m_runtime->currentStackFrame->max_locals; localIndex++) {
            m_runtime->currentStackFrame->local_var_table[localIndex] = 0;
        }
        for (int stackIndex = 0; stackIndex < m_runtime->currentStackFrame->max_stack; stackIndex++) {
            m_runtime->currentStackFrame->operand_stack[stackIndex] = 0;
        }
    }

    u4 popStack() {
        u4 value = m_runtime->currentStackFrame->operand_stack[--m_runtime->currentStackFrame->offset];
        m_runtime->currentStackFrame->operand_stack[m_runtime->currentStackFrame->offset] = (u4) NULL;
        return value;
    }

    void pushStack(u4 val) {
        m_runtime->currentStackFrame->operand_stack[m_runtime->currentStackFrame->offset++] = val;
    }

public:
    void execIConstI(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;
        pushStack(val);
        m_runtime->pc += len;
    }

    void execIStoreN(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;

        m_runtime->currentStackFrame->local_var_table[val] = m_runtime->currentStackFrame->operand_stack[--m_runtime->currentStackFrame->offset];
        m_runtime->currentStackFrame->operand_stack[m_runtime->currentStackFrame->offset] = (u4) NULL;
        m_runtime->pc += len;
    }

    void execILoadN(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;
        m_runtime->currentStackFrame->operand_stack[m_runtime->currentStackFrame->offset++] = m_runtime->currentStackFrame->local_var_table[val];
        m_runtime->pc += len;
    }

    void execIAdd(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;
        u4 value1 = popStack();
        u4 value2 = popStack();
        pushStack(value1 + value2);
        m_runtime->pc += len;
    }

    void execISub(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;
        u4 value1 = popStack();
        u4 value2 = popStack();
        pushStack(value2 - value1);
        m_runtime->pc += len;
    }

    void execIReturn(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;
        m_runtime->currentMethodDeep--;
        if (m_runtime->currentStackFrame->prev_stack != 0) {
            u4 val = popStack();
            this->eraseStackAndVarTable();
            m_runtime->pc = m_runtime->currentStackFrame->prev_stack->return_addr;
            m_runtime->currentMethod = m_runtime->currentStackFrame->prev_stack->method;
            m_runtime->currentStackFrame = m_runtime->currentStackFrame->prev_stack;
            m_runtime->currentStackFrame->prev_stack = 0;
            pushStack(val);
        }
    }

    void execReturn(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;
        m_runtime->currentMethodDeep--;
        if (m_runtime->currentStackFrame->prev_stack != 0) {
            this->eraseStackAndVarTable();
            m_runtime->pc = m_runtime->currentStackFrame->prev_stack->return_addr;
            m_runtime->currentMethod = m_runtime->currentStackFrame->prev_stack->method;
            m_runtime->currentStackFrame = m_runtime->currentStackFrame->prev_stack;

        }
    }

    void execInvokeStatic(u4 val, u2 len, const char *symbol) {
        cout << len << " execed! " << symbol << endl;
        u2 indexbyte1 = m_runtime->currentMethod->code[m_runtime->pc + 1];
        u2 indexbyte2 = m_runtime->currentMethod->code[m_runtime->pc + 2];
        u2 methodRefIndex = ((indexbyte1 << 8) | indexbyte2);
        //找到类跟方法
        u2 classIndex = ((CONSTANT_Methodref_info *) (m_runtime->currentClassFile->constant_pool[methodRefIndex].info))->class_index;
        u2 nameAndTypeIndex = ((CONSTANT_Methodref_info *) (m_runtime->currentClassFile->constant_pool[methodRefIndex].info))->name_and_type_index;
        char *className = (char *) ((CONSTANT_Utf8_info *) m_runtime->currentClassFile->constant_pool[classIndex].info)->bytes;
        u2 methodNameIndex = ((CONSTANT_NameAndType_info *) m_runtime->currentClassFile->constant_pool[nameAndTypeIndex].info)->name_index;
        u2 methodDesciptorIndex = ((CONSTANT_NameAndType_info *) m_runtime->currentClassFile->constant_pool[nameAndTypeIndex].info)->descriptor_index;

        for (u2 i = 0; i < m_runtime->currentClassFile->methods_count; i++) {
            if (m_runtime->currentClassFile->methods[i].poolIndex == methodNameIndex) {
                unsigned long size = m_runtime->currentClassFile->methods[i].arguments.size();
                for (u2 j = 0; j < size; j++) {
                    u4 arg = popStack();
                    m_runtime->currentClassFile->methods[i].jvmStackFrame->local_var_table[size - j - 1] = arg;
                }
                m_runtime->currentStackFrame->return_addr = (m_runtime->pc) + len;
                m_runtime->currentClassFile->methods[i].jvmStackFrame->prev_stack = m_runtime->currentStackFrame;
                m_runtime->currentMethod = &m_runtime->currentClassFile->methods[i];
                m_runtime->currentMethodDeep++;
                m_runtime->currentStackFrame = m_runtime->currentClassFile->methods[i].jvmStackFrame;
                m_runtime->pc = 0;
                return;
            }
        }
        printf("not found the method in pool #%d", methodNameIndex);
        exit(-1);
    }

    ExecutionEngine(Runtime *runtime) {
        initByteCode();
        m_runtime = runtime;
        printCurrentInfo();
        do {
            u1 code = m_runtime->currentMethod->code[m_runtime->pc];
            if (m_byte_code_hash.find(code) == m_byte_code_hash.end()) {
                printf("code %x is not support now", code);
                break;
            }
            (this->*m_byte_code_hash[code].func)(m_byte_code_hash[code].opcode_val, m_byte_code_hash[code].opcode_len,
                                                 m_byte_code_hash[code].symbol);
            printCurrentInfo();
        } while (m_runtime->currentMethodDeep > 0);
        printf("exec end!");
        //cout << m_byte_code_hash[4].opcode<<endl;
    };
};

#endif //TAOJVM_EXECUTION_ENGINE_H
