#ifndef VALUE_H
#define VALUE_H

    typedef struct Obj Obj;
    typedef struct ObjString ObjString;

    typedef enum {
        VAL_BOOL,
        VAL_NIL, 
        VAL_NUMBER,
        VAL_OBJ
    } ValueType;

    typedef struct {
        ValueType type;
        union {
            bool boolean;
            double number;
            Obj* obj;
        } as;
    } value;


    #define IS_BOOL(valuee)    ((valuee).type == VAL_BOOL)
    #define IS_NIL(valuee)     ((valuee).type == VAL_NIL)
    #define IS_NUMBER(valuee)  ((valuee).type == VAL_NUMBER)
    

    #define BOOL_VAL(valuee)   ((value){VAL_BOOL, {.boolean = valuee}})
    #define NIL_VAL            ((value){VAL_NIL, {.number = 0}})
    #define NUMBER_VAL(valuee) ((value){VAL_NUMBER, {.number = valuee}})

    #define AS_BOOL(valuee)   ((valuee).as.boolean)
    #define AS_NUMBER(valuee) ((valuee).as.number)
#endif