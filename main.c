//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

void* findGrade(int player, char *lectureName); 

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"


//board configuration parameters
static int smm_board_nr;
static int smm_food_nr;
static int smm_festival_nr;
static int smm_player_nr;

typedef struct {
	char name[MAX_CHARNAME];
	int pos;
	int credit;
	int energy;
	int flag_graduated;
}smm_player_t;

smm_player_t *smm_players;

void generatePlayers(int n, int initEnergy); //generate a new player
void printPlayerStatus(void); //print all player status at the beginning of each turn

//function prototypes
#if 0
void readingboard(){
	FILE * fp = fopen("marbleBoardConfig.txt", "r");
	
	char name[50];
	int type, credit, energy;
	
	while(fscanf(fp, "%d %d %d %d", name, &type, &credit, &energy) != EOF){
		//정보 읽어오기
		void* newNode = smm_obj_create(name, type, credit, energy);
		
		//데이터베이스에 정보 저장
		smm_add_node(newNode); 
	}
	fclose(fp);
}
void* findGrade(int player, char *lectureName) //find the grade from the player's grade history
{
      int size = smmdb_len(LISTNO_OFFSET_GRADE+player);
      int i;
      
      for (i=0;i<size;i++)
      {
          void *ptr = smmdb_getData(LISTNO_OFFSET_GRADE+player, i);
          if (strcmp(smmObj_getObjectName(ptr), lectureName) == 0)
          {
              return ptr;
          }
      }
      
      return NULL;
}

void printGrades(int player); //print all the grade history of the player
#endif

char* get_grade_str(int grade_idx) {
    switch(grade_idx) {
        case 0: return "A+";
        case 1: return "A0";
        case 2: return "A-";
        case 3: return "B+";
        case 4: return "B0";
        case 5: return "B-";
        case 6: return "C+";
        case 7: return "C0";
        case 8: return "C-";
        default: return "ERROR";
    }
}
smmGrade_e takeLecture(int player, char *lectureName, int credit) //take the lecture (insert a grade of the player)
{
	int selection;
	smmGrade_e grade;
	int grade_idx;
	
	//1.수강 여부 확인
	printf("%s 강의를 수강하시겠습니까? (수강 시 1, 수강 포기 시 0)", lectureName);
	scanf("%d", &selection);
	//2.성적 배정 
	if (selection == 1){
		grade = (smmGrade_e)(rand() % 9);
		
		printf("성적 결과: %s, %d 학점\n", get_grade_str(grade_idx), credit);
		
		return grade_idx; 
			
	}
	printf("수강 포기\n");
	return -1;
	 
};
float calcAverageGrade(int player)//calculate average grade of the player
{
	int i;
    int len = smmdb_len(LISTNO_OFFSET_GRADE + player);
    void *gradePtr;
    int totalCredits = 0;
    float totalScore = 0.0;
    
    float gradeScores[] = {4.5f, 4.0f, 3.5f, 3.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f};

    if (len == 0) return 0.0f;

    for (i = 0; i < len; i++) {
        gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        if (gradePtr != NULL) {
            int grade = smmObj_getObjectGrade(gradePtr);
            int credit = smmObj_getObjectCredit(gradePtr);
            
            if (grade >= 0 && grade < SMMNODE_MAX_GRADE) {
                totalScore += gradeScores[grade] * credit;
                totalCredits += credit;
            }
        }
}
if (totalCredits == 0) return 0.0f;
return totalScore / totalCredits;
}
void printGrades(int player) //print grade history of the player
{
	int i;
    int len = smmdb_len(LISTNO_OFFSET_GRADE + player);
    void *gradePtr;
    
    printf("\n--- %s's Grade History (Total: %i credits) ---\n", 
           smm_players[player].name, smm_players[player].credit);
    
    for (i = 0; i < len; i++) {
        gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        if (gradePtr != NULL) {
            printf("[%i] %s (Credit: %i) - Grade: %s\n",
                   i + 1,
                   smmObj_getObjectName(gradePtr),
                   smmObj_getObjectCredit(gradePtr),
                   smmObj_getGradeName(smmObj_getObjectGrade(gradePtr))); // smmObj_getGradeName 사용
        }
    }
}

int isGraduated(void) //check if any player is graduated
{
	int i;
	for (i=0;i<smm_player_nr;i++)
	{
		if (smm_players[i].flag_graduated == 1)
			return 1;
	}
	
	return 0;
}  


void goForward(int player, int step)
{ //make player go "step" steps on the board (check if player is graduated)
    int i;
    void *ptr;
    
    
    //player_pos[player] = player_pos[player]+ step;
    ptr = smmdb_getData(LISTNO_NODE, smm_players[player].pos);
    printf("start from %i(%s) (%i)\n", smm_players[player].pos, 
                                         smmObj_getObjectName(ptr), step);
    for (i=0;i<step;i++)
    {
        smm_players[player].pos = (smm_players[player].pos + 1)%smm_board_nr;
        printf("  => moved to %i(%s)\n", smm_players[player].pos, 
                                         smmObj_getNodeName(smm_players[player].pos));
    }
}

void printPlayerStatus(void)
{
     int i;
     for (i=0;i<smm_player_nr;i++)
     {
         printf("%s - position:%i(%s), credit:%i, energy:%i\n",
                    smm_players[i].name, smm_players[i].pos, smmObj_getNodeName(smm_players[i].pos), smm_players[i].credit, smm_players[i].energy);
     }
}

void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     
     smm_players = (smm_player_t*)malloc(n*sizeof(smm_player_t));
     
     for (i=0;i<n;i++)
     {
         smm_players[i].pos = 0;
         smm_players[i].credit = 0;
         smm_players[i].energy = initEnergy;
         smm_players[i].flag_graduated = 0;
         
         printf("Input %i-th player name:", i);
         scanf("%s", &smm_players[i].name[0]);
         fflush(stdin); 
     }
}

int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}


//action code when a player stays at a node
void actionNode(int player)
{
	void *ptr = smmdb_getData(LISTNO_NODE,smm_players[player].pos);
	
	int type = smmObj_getObjectType (ptr);
	int credit = smmObj_getObjectCredit (ptr);
	int energy = smmObj_getObjectEnergy (ptr);
	char* nodeName = smmObj_getObjectName(ptr);
	
	printf(" --> player%i pos : %i, type : %s, credit : %i, energy : %i\n",
			player, smm_players[player].pos, smmObj_getNodeName(type), credit, energy);
			
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
        	//이미 수강한 강의인지 확인 
        	if (findGrade(player, nodeName) == NULL){
        		//현재 에너지가 필요한 에너지보다 많은지 확인 
        		if (smm_players[player].energy >= energy){
        			//성적 산출 
        			int grade_idx = takeLecture(player, nodeName, credit);
        			
        			if (grade_idx != -1){
        				smm_players[player].credit += credit;
        				smm_players[player].energy -= energy;
        				
        				void* gradePtr = smmObj_getObject(nodeName, SMMNODE_OBJTYPE_GRADE, type, credit, energy, grade_idx);
        				smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
					}
				} else{
				printf("에너지가 부족합니다.\n");
				}
			} else {
				printf("이미 수강한 과목힙니다.\n");
				
			} 
			break; 
        
        case SMMNODE_TYPE_RESTAURANT: //에너지 보충 
        	smm_players[player].energy += energy;
            break;
        case SMMNODE_TYPE_LABORATORY:
            break;
        case SMMNODE_TYPE_HOME:
        	smm_players[player].energy += energy; //에너지 보충 
        	if (smm_players[player].credit >= GRADUATE_CREDIT)
        	{
        		smm_players[player].flag_graduated = 1;//졸업 
			}
            break;
        case SMMNODE_TYPE_GOTOLAB:
            break;
        case SMMNODE_TYPE_FOODCHANGE:
            break;
        case SMMNODE_TYPE_FESTIVAL:
            break;
            
        default:
        	break;


    }
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int turn;
    
    smm_board_nr = 0;
    smm_food_nr = 0;
    smm_festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        void* ptr;
        printf("%s %i %i %i\n",name, type, credit, energy);
        ptr = smmObj_getObject(name, SMMNODE_OBJTYPE_BOARD, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, ptr);
		smm_board_nr = smmdb_len(LISTNO_NODE);
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", smm_board_nr);
    
    
#if 1  
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        void* ptr = smmObj_getObject(name, SMMNODE_OBJTYPE_FOOD, 0, 0, energy, 0);//store the parameter set
        smmdb_addTail(LISTNO_FOODCARD, ptr);
        smm_food_nr++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", smm_food_nr);
    

    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {
        void* ptr = smmObj_getObject(name, SMMNODE_OBJTYPE_FEST, 0, 0, 0, 0);//store the parameter set
        smmdb_addTail(LISTNO_FESTCARD, ptr);
        smm_festival_nr++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", smm_festival_nr);
    
    
#endif   
    //2. Player configuration ---------------------------------------------------------------------------------
    
	void *startNodePtr = smmdb_getData(LISTNO_NODE, 0);
    int initEnergy;
    
    if (startNodePtr != NULL) {
        initEnergy = smmObj_getObjectEnergy(startNodePtr);
    } else {
        printf("[WARNING] Failed to get start node for initial energy. Using default 100.\n");
        initEnergy = 100; // Fallback
    }
    
    do
    {
        //input player number to player_nr
		printf("Input player number:");
        scanf("%i", &smm_player_nr);
        
        if (smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER)
           printf("Invalid player number!\n");
    }
    
    while (smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER);
    
    generatePlayers(smm_player_nr, initEnergy);
   
    
    
	turn = 0;
    //3. SM Marble game starts ---------------------------------------------------------------------------------
#if 1
    while (!isGraduated()) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie(turn);
        printf("\nPlayer %s rolled a %i.\n", smm_players[turn].name, die_result);
        
        //4-3. go forward
        goForward(turn, die_result);
       
		//pos = pos + 2;

		
		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn+1)%smm_player_nr;

    }
    
    printf("\n\n--- Game Over ---\n");
    printPlayerStatus();
    printf("A player has graduated!\n");
    
   	free(smm_players);
#endif
	system("PAUSE");
    return 0;

}
