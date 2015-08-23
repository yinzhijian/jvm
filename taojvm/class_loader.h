//
// Created by yintao on 15/8/21.
//
#include "jvm.h"
#include <iostream>
#include <fstream>
#include   <string.h>
#include   <string>

using namespace std;
#ifndef TAOJVM_CLASS_LOADER_H
#define TAOJVM_CLASS_LOADER_H

class ClassLoader {
private:
    ClassFile *m_class_file;
    File *m_file;

    u2 parseAttribute(AttributeInfo *&attributes, char *&startIndex);

    void parseCodeAttribute(CodeAttribute *&codeAttribute, char *&startIndex);

    const char *constantTranslate(CLASS_FILE_CONSTANT_TAG class_file_constant) {
        switch (class_file_constant) {
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

    const char *accessFlagsTranslate(u2 accessFlags) {
        string out;
        out.clear();
        if (accessFlags & ACC_PUBLIC) {
            out += "ACC_PUBLIC ";
        }
        if (accessFlags & ACC_FINAL) {
            out += "ACC_FINAL ";
        }
        if (accessFlags & ACC_SUPER) {
            out += "ACC_SUPER ";
        }
        if (accessFlags & ACC_INTERFACE) {
            out += "ACC_INTERFACE ";
        }
        if (accessFlags & ACC_ABSTRACT) {
            out += "ACC_ABSTRACT ";
        }
        if (accessFlags & ACC_SYNTHETIC) {
            out += "ACC_SYNTHETIC ";
        }
        if (accessFlags & ACC_ANNOTATION) {
            out += "ACC_ANNOTATION ";
        }
        if (accessFlags & ACC_ENUM) {
            out += "ACC_ENUM ";
        }
        return out.c_str();
    }

    void readFile(File &file) {
        filebuf *pbuf;
        ifstream filestr;
        // 要读入整个文件，必须采用二进制打开
        filestr.open(file.file_path, ios::binary);
        // 获取filestr对应buffer对象的指针
        pbuf = filestr.rdbuf();

        // 调用buffer对象方法获取文件大小
        file.size = pbuf->pubseekoff(0, ios::end, ios::in);
        pbuf->pubseekpos(0, ios::in);

        // 分配内存空间
        file.content = new char[file.size];

        // 获取文件内容
        pbuf->sgetn(file.content, file.size);

        filestr.close();
    }

    void parseConstantPool(ClassFile &classFile, char *&startIndex) {

        classFile.constant_pool = new CpInfo[classFile.constant_pool_count];
        u2 index;
        for (int i = 1; i < classFile.constant_pool_count; i++) {
            classFile.constant_pool[i].tag = *(u1 *) startIndex;
            startIndex += 1;
            switch (classFile.constant_pool[i].tag) {
                case CONSTANT_Class:
                case CONSTANT_String:
                case CONSTANT_MethodType:
                    CONSTANT_String_info constant_string_info;
                    CLASS_READ_U2(constant_string_info.string_index, *(u2 *) startIndex);
                    printf("#%d = %s string_index:%d\n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           constant_string_info.string_index);
                    startIndex += 2;
                    classFile.constant_pool[i].info = (u1 *) &constant_string_info;
                    break;
                case CONSTANT_Fieldref:
                case CONSTANT_Methodref:
                case CONSTANT_NameAndType:
                case CONSTANT_InterfaceMethodref: {
                    CONSTANT_Fieldref_info *constant_fieldref_info = new CONSTANT_Fieldref_info;
                    CLASS_READ_U2(constant_fieldref_info->class_index, *(u2 *) startIndex);
                    startIndex += 2;
                    CLASS_READ_U2(constant_fieldref_info->name_and_type_index, *(u2 *) startIndex);
                    startIndex += 2;
                    printf("#%d = %s class_index:%d name_and_type_index:%d\n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           constant_fieldref_info->class_index, constant_fieldref_info->name_and_type_index);
                    classFile.constant_pool[i].info = (u1 *) constant_fieldref_info;
                    printf("Test %d",
                           ((CONSTANT_Methodref_info *) classFile.constant_pool[i].info)->name_and_type_index);
                }
                    break;
                case CONSTANT_InvokeDynamic:
                    CONSTANT_InvokeDynamic_info constant_invokeDynamic_info;
                    CLASS_READ_U2(constant_invokeDynamic_info.bootstrap_method_attr_index, *(u2 *) startIndex);
                    startIndex += 2;
                    CLASS_READ_U2(constant_invokeDynamic_info.name_and_type_index, *(u2 *) startIndex);
                    startIndex += 2;
                    printf("#%d = %s bootstrap_method_attr_index:%d name_and_type_index:%d\n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           constant_invokeDynamic_info.bootstrap_method_attr_index,
                           constant_invokeDynamic_info.name_and_type_index);
                    classFile.constant_pool[i].info = (u1 *) &constant_invokeDynamic_info;
                    break;
                case CONSTANT_Integer:
                    CONSTANT_Integer_info constant_integer_info;
                    CLASS_READ_U4(constant_integer_info.bytes, *(u4 *) startIndex);
                    startIndex += 4;
                    printf("#%d = %s bytes:%d \n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           constant_integer_info.bytes);
                    classFile.constant_pool[i].info = (u1 *) &constant_integer_info;
                    break;
                case CONSTANT_Float:
                    CONSTANT_Float_info constant_float_info;
                    CLASS_READ_U4(constant_float_info.bytes, *(u4 *) startIndex);
                    startIndex += 4;
                    printf("#%d = %s bytes:%f \n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           *(float *) (&constant_float_info.bytes));
                    classFile.constant_pool[i].info = (u1 *) &constant_integer_info;
                    break;
                case CONSTANT_Long:
                    CONSTANT_Long_info constant_long_info;
                    CLASS_READ_U4(constant_long_info.low_bytes, *(u4 *) startIndex);
                    startIndex += 4;
                    CLASS_READ_U4(constant_long_info.high_bytes, *(u4 *) startIndex);
                    startIndex += 4;
                    printf("#%d = %s value %ld\n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           *(long *) (&constant_long_info.high_bytes));
                    classFile.constant_pool[i].info = (u1 *) &constant_long_info;
                    i++;//由于LONG跟Double占两个entries
                    break;
                case CONSTANT_Double: {
                    CONSTANT_Double_info *constant_double_info = new CONSTANT_Double_info;
                    CLASS_READ_U4(constant_double_info->low_bytes, *(u4 *) startIndex);
                    startIndex += 4;
                    CLASS_READ_U4(constant_double_info->high_bytes, *(u4 *) startIndex);
                    startIndex += 4;
                    printf("#%d = %s value %f\n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           *(double *) (&constant_double_info->high_bytes));
                    classFile.constant_pool[i].info = (u1 *) constant_double_info;
                    i++;//由于LONG跟Double占两个entries
                }
                    break;
                case CONSTANT_Utf8: {
                    CONSTANT_Utf8_info *constant_utf8_info = new CONSTANT_Utf8_info;
                    CLASS_READ_U2(constant_utf8_info->length, *(u2 *) startIndex);
                    startIndex += 2;
                    constant_utf8_info->bytes = new u1[constant_utf8_info->length + 1];
                    constant_utf8_info->bytes[constant_utf8_info->length] = '\0';
                    memcpy(constant_utf8_info->bytes, startIndex, constant_utf8_info->length);
                    startIndex += constant_utf8_info->length;
                    printf("#%d = %s len %d\t%s\n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           constant_utf8_info->length, constant_utf8_info->bytes);
                    classFile.constant_pool[i].info = (u1 *) constant_utf8_info;
                }
                    break;
                case CONSTANT_MethodHandle: {
                    CONSTANT_MethodHandle_info *constant_methodHandle_info = new CONSTANT_MethodHandle_info;
                    constant_methodHandle_info->reference_kind = *(u1 *) startIndex;
                    startIndex += 2;
                    CLASS_READ_U2(constant_methodHandle_info->reference_index, *(u2 *) startIndex);
                    startIndex += 2;
                    printf("#%d = %s reference_kind：%d reference_index：%d\n", i,
                           constantTranslate((CLASS_FILE_CONSTANT_TAG) classFile.constant_pool[i].tag),
                           constant_methodHandle_info->reference_kind, constant_methodHandle_info->reference_index);
                    classFile.constant_pool[i].info = (u1 *) constant_methodHandle_info;
                }
                    break;
                default:
                    cout << "#" << i << " = unknow tag:" << classFile.constant_pool[i].tag + 0 << endl;
                    break;
            }
        }
    }

    void paresInterface(ClassFile &classFile, char *&startIndex) {
        CLASS_READ_U2(classFile.interfaces_count, *(u2 *) startIndex);
        startIndex += 2;
        cout << "interfaces_count:" << classFile.interfaces_count << endl;
        cout << "interfaces:";
        if (classFile.interfaces_count > 0) {
            u2 *interfaces = new u2[classFile.interfaces_count];
            for (int i = 0; i < classFile.interfaces_count; i++) {
                CLASS_READ_U2(interfaces[i], *(u2 *) startIndex);
                startIndex += 2;
                cout << interfaces[i] << " ";
            }
            classFile.interfaces = interfaces;
            cout << endl;
        } else {
            classFile.interfaces = NULL;
            cout << "null" << endl;
        }
    }


    void parseField(ClassFile &classFile, char *&startIndex) {
        CLASS_READ_U2(classFile.fields_count, *(u2 *) startIndex);
        startIndex += 2;
        cout << "fields_count:" << classFile.fields_count << endl;
        cout << "field_info:";
        if (classFile.fields_count > 0) {
            FieldInfo *fieldInfo = new FieldInfo[classFile.fields_count];
            for (int i = 0; i < classFile.fields_count; i++) {
                cout << "#" << i << " = ";
                CLASS_READ_U2(fieldInfo[i].access_flags, *(u2 *) startIndex);
                startIndex += 2;
                printf("access_flags: %s", accessFlagsTranslate(fieldInfo[i].access_flags));
                //cout<<"access_flags:"<<accessFlagsTranslate(fieldInfo[i].access_flags)<<" ";
                CLASS_READ_U2(fieldInfo[i].name_index, *(u2 *) startIndex);
                startIndex += 2;
                cout << "name_index:" << fieldInfo[i].name_index << "";
                CLASS_READ_U2(fieldInfo[i].descriptor_index, *(u2 *) startIndex);
                startIndex += 2;
                cout << "descriptor_index:" << fieldInfo[i].descriptor_index << endl;
                fieldInfo[i].attributes_count = parseAttribute((AttributeInfo *&) fieldInfo[i].attributes, startIndex);

            }
            classFile.fields = fieldInfo;
            cout << endl;
        } else {
            classFile.fields = NULL;
            cout << "null" << endl;
        }
    }

    void parseDescriptor(const u1 *descriptor, vector<string> &arguments, string &returnType) {
        bool isObj = false;
        string temp;
        for (int i = 0; descriptor[i] != '\0'; i++) {
            if (descriptor[i] == '(') {
                continue;
            } else if (descriptor[i] == ')') {
                returnType = string((const char *) &descriptor[i + 1]);
                break;
            } else {
                if (descriptor[i] == 'L') {
                    isObj = true;
                } else if (descriptor[i] == ';') {
                    isObj = false;
                } else if (descriptor[i] == '[') {
                    temp += descriptor[i];
                    continue;
                }
                if (isObj == true) {
                    temp += descriptor[i];
                } else {
                    temp += descriptor[i] + '\0';
                    arguments.push_back(temp);
                    temp = string();
                }
            }
        }
        printf("desc %s\n", descriptor);
        for (vector<string>::iterator it = arguments.begin(); it != arguments.end(); ++it) {

            printf("%s\t", it->c_str());
        }
        printf("return %s\n", returnType.c_str());
    }

    void parseMethod(ClassFile &classFile, char *&startIndex) {
        CLASS_READ_U2(classFile.methods_count, *(u2 *) startIndex);
        startIndex += 2;
        cout << "methods_count:" << classFile.methods_count << endl;
        cout << "method_info:";

        if (classFile.methods_count > 0) {
            classFile.methods = new Method[classFile.methods_count];
            MethodInfo *methodInfo = new MethodInfo[classFile.methods_count];
            for (int i = 0; i < classFile.methods_count; i++) {
                cout << "#" << i << " = ";
                CLASS_READ_U2(methodInfo[i].access_flags, *(u2 *) startIndex);
                startIndex += 2;
                printf("access_flags: %s", accessFlagsTranslate(methodInfo[i].access_flags));
                //cout<<"access_flags:"<<accessFlagsTranslate(methodInfo[i].access_flags)<<" ";
                CLASS_READ_U2(methodInfo[i].name_index, *(u2 *) startIndex);
                startIndex += 2;
                cout << "name_index:" << methodInfo[i].name_index << "";
                CLASS_READ_U2(methodInfo[i].descriptor_index, *(u2 *) startIndex);
                startIndex += 2;
                cout << "descriptor_index:" << methodInfo[i].descriptor_index << endl;
                methodInfo[i].attributes_count = parseAttribute((AttributeInfo *&) methodInfo[i].attributes,
                                                                startIndex);
                classFile.methods[i].poolIndex = methodInfo[i].name_index;
                classFile.methods[i].name = ((CONSTANT_Utf8_info *) classFile.constant_pool[methodInfo[i].name_index].info)->bytes;
                classFile.methods[i].descriptor = ((CONSTANT_Utf8_info *) classFile.constant_pool[methodInfo[i].descriptor_index].info)->bytes;
                parseDescriptor(classFile.methods[i].descriptor, classFile.methods[i].arguments,
                                classFile.methods[i].returnType);
                cout << "method name =" << classFile.methods[i].name << endl;
                for (int mi = 0; mi < methodInfo[i].attributes_count; mi++) {
                    u2 ani = methodInfo[i].attributes[mi].attribute_name_index;
                    CONSTANT_Utf8_info *cui = (CONSTANT_Utf8_info *) classFile.constant_pool[ani].info;
                    u1 *tempAttrName = cui->bytes;
                    if (!strcmp((const char *) tempAttrName, "Code")) {
                        cout << "Code!!\n" << endl;
                        //parseCodeAttribute(methodInfo[i].attributes[mi], methodInfo[i].attributes[mi],
                        //                  methodInfo[i].attributes[mi])
                        classFile.methods[i].code_length = ((CodeAttribute *) (methodInfo[i].attributes[mi].info))->code_length;
                        classFile.methods[i].code = ((CodeAttribute *) (methodInfo[i].attributes[mi].info))->code;
                        classFile.methods[i].jvmStackFrame = new JvmStackFrame;
                        classFile.methods[i].jvmStackFrame->max_locals = ((CodeAttribute *) (methodInfo[i].attributes[mi].info))->max_locals;
                        classFile.methods[i].jvmStackFrame->max_stack = ((CodeAttribute *) (methodInfo[i].attributes[mi].info))->max_stack;
                        classFile.methods[i].jvmStackFrame->method = &classFile.methods[i];
                        classFile.methods[i].jvmStackFrame->local_var_table = new u4[classFile.methods[i].jvmStackFrame->max_locals];
                        classFile.methods[i].jvmStackFrame->operand_stack = new u4[classFile.methods[i].jvmStackFrame->max_stack];
                        classFile.methods[i].jvmStackFrame->offset = 0;
                        classFile.methods[i].jvmStackFrame->prev_stack = NULL;
                        classFile.methods[i].jvmStackFrame->return_addr = (u2) NULL;
                        for (int codeIndex = 0; codeIndex < classFile.methods[i].code_length; codeIndex++) {
                            printf("%2X", classFile.methods[i].code[codeIndex]);
                        }
                    }
                }
            }
            delete[]methodInfo;
            cout << endl;
        } else {
            classFile.methods = NULL;
            cout << "null" << endl;
        }
    }

    void parse(ClassFile &classFile, File file) {
        long i = 0;
        char *startIndex = file.content;
        CLASS_READ_U4(classFile.magic, *(u4 *) startIndex);
        startIndex += 4;
        CLASS_READ_U2(classFile.minor_version, *(u2 *) startIndex);
        startIndex += 2;
        CLASS_READ_U2(classFile.major_version, *(u2 *) startIndex);
        startIndex += 2;
        CLASS_READ_U2(classFile.constant_pool_count, *(u2 *) startIndex);
        startIndex += 2;
        cout << "file size:" << file.size << endl;
        cout << hex << "magic:" << classFile.magic << endl;
        cout << dec << " minor:" << classFile.minor_version << " major:" << classFile.major_version <<
        " const_pool_count:" << classFile.constant_pool_count << endl;

        parseConstantPool(classFile, startIndex);
        CLASS_READ_U2(classFile.access_flags, *(u2 *) startIndex);
        startIndex += 2;
        printf("access_flags: %s", accessFlagsTranslate(classFile.access_flags));
        CLASS_READ_U2(classFile.this_class, *(u2 *) startIndex);
        startIndex += 2;
        cout << "this_class:" << classFile.this_class << endl;
        CLASS_READ_U2(classFile.super_class, *(u2 *) startIndex);
        startIndex += 2;
        cout << "super_class:" << classFile.super_class << endl;
        paresInterface(classFile, startIndex);
        parseField(classFile, startIndex);
        parseMethod(classFile, startIndex);
        classFile.attributes_count = parseAttribute((AttributeInfo *&) classFile.attributes, startIndex);
    }

public:
    ClassLoader(const char *filePath) {
        this->m_file = new File;
        this->m_file->file_path = filePath;
        this->readFile(*this->m_file);

        this->m_class_file = new ClassFile;
        parse(*this->m_class_file, *this->m_file);
    };

    ClassFile *getClassFile() {
        return this->m_class_file;
    }
};

u2 ClassLoader::parseAttribute(AttributeInfo *&attributes, char *&startIndex) {
    u2 attributes_count;
    CLASS_READ_U2(attributes_count, *(u2 *) startIndex);
    startIndex += 2;
    cout << "attributes_count:" << attributes_count << endl;
    attributes = new AttributeInfo[attributes_count];
    for (int i = 0; i < attributes_count; i++) {
        CLASS_READ_U2(attributes[i].attribute_name_index, *(u2 *) startIndex);
        cout << "#" << i << " = attribute_name_index:" << attributes[i].attribute_name_index << endl;
        startIndex += 2;
        CLASS_READ_U4(attributes[i].attribute_length, *(u4 *) startIndex);
        cout << "#" << i << " = attribute_length:" << attributes[i].attribute_length << endl;
        startIndex += 4;
        if (!strcmp(
                (const char *) (((CONSTANT_Utf8_info *) this->m_class_file->constant_pool[attributes[i].attribute_name_index].info)->bytes),
                "Code")) {
            CodeAttribute *codeAttribute = new CodeAttribute;
            //codeAttribute->attribute_name_index = attributes[i].attribute_name_index;
            //codeAttribute->attribute_length = attributes[i].attribute_length;
            //delete (&attributes[i]);
            this->parseCodeAttribute(codeAttribute, startIndex);
            attributes[i].info = (u1 *) codeAttribute;
        } else {
            attributes[i].info = (u1 *) startIndex;
            startIndex += attributes[i].attribute_length;
        }

    }
    return attributes_count;
}

void ClassLoader::parseCodeAttribute(CodeAttribute *&codeAttribute, char *&startIndex) {
    CLASS_READ_U2(codeAttribute->max_stack, *(u2 *) startIndex);
    startIndex += 2;
    printf("max_stack:%d\n", codeAttribute->max_stack);
    CLASS_READ_U2(codeAttribute->max_locals, *(u2 *) startIndex);
    startIndex += 2;
    printf("max_locals:%d\n", codeAttribute->max_locals);
    CLASS_READ_U4(codeAttribute->code_length, *(u4 *) startIndex);
    startIndex += 4;
    if (codeAttribute->code_length > 0) {
        CLASS_READ_U1(codeAttribute->code, (u1 *) startIndex);
        startIndex += codeAttribute->code_length;
    }
    CLASS_READ_U2(codeAttribute->exception_table_length, *(u2 *) startIndex);
    startIndex += 2;
    if (codeAttribute->exception_table_length > 0) {
        codeAttribute->exceptionTable = (ExceptionTable *) startIndex;
        startIndex += codeAttribute->exception_table_length;
    }
    parseAttribute((AttributeInfo *&) codeAttribute->attributes, startIndex);
}

#endif //TAOJVM_CLASS_LOADER_H
