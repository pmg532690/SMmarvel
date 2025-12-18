//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE 			0
#define SMMNODE_TYPE_RESTAURANT 		1
#define SMMNODE_TYPE_LABORATORY			2
#define SMMNODE_TYPE_HOME				3
#define SMMNODE_TYPE_GOTOLAB 			4
#define SMMNODE_TYPE_FOODCHANGE 		5
#define SMMNODE_TYPE_FESTIVAL			6

#define SMMNODE_OBJTYPE_BOARD	0
#define SMMNODE_OBJTYPE_GRADE	1
#define SMMNODE_OBJTYPE_FOOD	2
#define SMMNODE_OBJTYPE_FEST	3

typedef enum smmGrade {
	smmGrade_AP = 0,
	smmGrade_A0,
	smmGrade_AM,
	smmGrade_BP,
	smmGrade_B0,
	smmGrade_BM,
	smmGrade_CP,
	smmGrade_C0,
	smmGrade_CM,
}smmGrade_e;

#define SMMNODE_MAX_GRADE       13


//object generation
void* smmObj_getObject(char* name, int objType, int type, int credit, int energy, int grade);
char* smmObj_getObjectName(void *ptr);
int smmObj_getObjectType(void *ptr);
int smmObj_getObjectEnergy(void *ptr);
char* smmObj_getObjectName(void *ptr);
int smmObj_getObjectCredit(void *ptr);
int smmObj_getObjectGrade(void *ptr);
//member retrieving


//element to string

char* smmObj_getNodeName(int type);
char* smmObj_getGradeName(int grade);

#endif /* smm_object_h */
