#ifndef OBJECT_H
#define OBJECT_H
    #include "value.h"
    #include <string>

    using namespace std;

    typedef enum {
        OBJ_STRING,
    } ObjType;
    struct Obj {
        ObjType type;
    };
    struct ObjString {
        Obj obj;
        std::string str; 
    };
    
    
    #define OBJ_TYPE(valuee)   (AS_OBJ(valuee)->type)
    #define IS_OBJ(valuee)     ((valuee).type == VAL_OBJ)
    #define IS_STRING(valuee)  isObjType(valuee, OBJ_STRING)
    #define AS_STRING(valuee)  ((ObjString*)AS_OBJ(valuee))
    #define AS_CPPSTRING(valuee)  (((ObjString*)AS_OBJ(valuee))->str)


    #define OBJ_VAL(object)    ((value){VAL_OBJ, {.obj = (Obj*)object}})
    #define AS_OBJ(valuee)     ((valuee).as.obj)

    static inline bool isObjType(value valuee, ObjType type)
    {
        return IS_OBJ(valuee) && AS_OBJ(valuee)->type == type;
    }
    // make this work
    static ObjString* copyString(string str)
    {
        ObjString* striing = new ObjString();
        striing->obj.type = OBJ_STRING;
        striing->str = str;

        return striing;
    }
#endif