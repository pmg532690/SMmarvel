//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODENR		100
#define MAX_NODETYPE    7





#define GRADE_A+		0
#define GRADE_A0		1
#define GRADE_A-		2
#define GRADE_B+		3
#define GRADE_B0		4
#define GRADE_B-		5
#define GRADE_C+		6
#define GRADE_C0		7
#define GRADE_C-		8
#define GRADE_D+		9
#define GRADE_D0		10
#define GRADE_D-		11
#define GRADE_F			12

static char smmObj_nodeName [MAX_NODETYPE][MAX_CHARNAME] = {
		"lecture",
		"restaurant",
		"laboratory",
		"home",
		"gotoLab",
		"foodChance", 
		"festival"
};

#define MAX_GRADE SMMNODE_MAX_GRADE

static char smmObj_gradeName [MAX_GRADE][MAX_CHARNAME] = {
		"A+",
		"A0",
		"A-",
		"B+",
		"B0",
		"B-",
		"C+",
		"C0",
		"C-",
		"D+",
		"D0",
		"D-",
		"F"
};


//structure type definition
typedef struct {
	char name[MAX_CHARNAME];
	int objType;
	int type;
	int credit;
	int energy;
	int grade;
} smmObj_object_t;

//object generation
void* smmObj_getObject(char* name, int objType, int type, int credit, int energy, int grade)
{
	smmObj_object_t* ptr;
	ptr = (smmObj_object_t*)malloc(sizeof(smmObj_object_t));
	
	strcpy(ptr->name,name);
    ptr->type = type;
    ptr->objType = objType;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    
    
    return ((void*)ptr);
}


//member retrieving
char* smmObj_getObjectName(void *ptr)
{
	smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
	return (objPtr-> name);
}
int smmObj_getObjectType(void *ptr) // smmObj_getNodeName(void *ptr) 처럼 다 바꾸기 
{
	smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
	return (objPtr-> type);
}
int smmObj_getObjectCredit(void *ptr)
{
	smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
	return (objPtr-> credit);
}
int smmObj_getObjectEnergy(void *ptr)
{
	smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
	return (objPtr-> energy);
}
int smmObj_getObjectGrade(void *ptr)
{
    smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
    return (objPtr->grade);
}





//element to string
char* smmObj_getNodeName(int type)
{
    if (type >= 0 && type < MAX_NODETYPE)
        return smmObj_nodeName[type];
}

char* smmObj_getGradeName(int grade)
{
    if (grade >= 0 && grade < MAX_GRADE)
        return smmObj_gradeName[grade];
    
}

