#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct{
	int month;
	int day;
	int year;
	int patientID;
	char action[10];
	int infected_by;
}Memory;

typedef struct patient{
	int patientID;
	int check_in_month;
	int check_in_day;
	int check_in_year;
	int check_out_month;
	int check_out_day;
	int check_out_year;
	char condition[10];
	int deceased_month;
	int deceased_day;
	int deceased_year;
	int infected_by_patientID;
	struct patient *infected_by;
}Patient;

Patient *initialize(int month, int day, int year, int patientID, int infected_by){
	Patient *new_patient = (Patient *)malloc(sizeof(Patient));
	new_patient->patientID = patientID;
	new_patient->check_in_month = month;
	new_patient->check_in_day = day;
	new_patient->check_in_year = year;
	new_patient->check_out_month = -1;
	new_patient->check_out_day = -1;
	new_patient->check_out_year = -1;
	strcpy(new_patient->condition,"check in");
	new_patient->deceased_month = -1;
	new_patient->deceased_day = -1;
	new_patient->deceased_year = -1;
	new_patient->infected_by_patientID = infected_by;
	new_patient->infected_by = NULL;
	return new_patient;
}

Memory *read_data(FILE *in, int *list_size){
	char buffer[40];
	char month[3];
	char day[3];
	char year[5];
	char ID[10];
	char infected_by[10];

	int line = 0;
	int i;
	int index = 0;

	Memory *list = (Memory *)malloc(sizeof(Memory));
	while(fgets(buffer,40,in) != NULL){
		// printf("%s\n", buffer);
		// printf("%d\n", (int)strlen(buffer));
		list = (Memory *)realloc(list, sizeof(Memory)*(line+1));

		if(!isdigit(buffer[0])){
			break;
		}

		for(i = 0; i < 2; i++){
			month[i] = buffer[i];
		}
		month[i] = '\0';

		for(i = 0; i < 2; i++){
			day[i] = buffer[i+3];
		}
		day[i] = '\0';

		for(i = 0; i < 4; i++){
			year[i] = buffer[i+6];
		}
		year[i] = '\0';

		list[line].month = atoi(month);
		list[line].day = atoi(day);
		list[line].year = atoi(year);

		i = 11;
		while(isdigit(buffer[i])){
			ID[i-11] = buffer[i];
			i++;
		}
		ID[i-11] = '\0';

		list[line].patientID = atoi(ID);

		/*to skip the space after patientID*/
		i++;

		if(buffer[i] == 'd'){
			strcpy(list[line].action, "deceased");
			list[line].infected_by = -1;
		}else{
			if(buffer[i+6] == 'i'){
				strcpy(list[line].action, "check in");

				i = i + 9;
				index = 0;
				while(isdigit(buffer[i])){
					infected_by[index] = buffer[i];
					i++;
					index++;
				}
				infected_by[index] = '\0';
				list[line].infected_by = atoi(infected_by);
			}else{
				strcpy(list[line].action, "check out");
				list[line].infected_by = -1;
			}
		}
		line++;
	}

	*list_size = line;
	return list;
}

int main(int argc, char **argv){
	/* Declare a memory structure to store all the info from the input file */
	Memory *list;
	int list_size = 0;
	int patient_count = 0;

	FILE *in;
	FILE *out;
	int i;

	Patient *patient_list = (Patient *)malloc(sizeof(Patient));

	/*Safety Checks*/
	if(argc != 3){
		printf("Wrong number of arguments\n");
		exit(-1);
	}

	in = fopen(argv[1],"r");
	out = fopen(argv[2],"w");

	if(in == NULL){
		printf("Input file does not exist.\n");
		exit(-2);
	}

	list = read_data(in, &list_size);

	/*Print out the list to see if everything above works*/
	// for(i = 0; i < list_size; i++){
	// 	printf("%d/%d/%d %d %s %d\n", 
	// 		list[i].month, list[i].day, list[i].year, list[i].patientID,
	// 		list[i].action, list[i].infected_by);
	// }

	// printf("\nlist_size = %d\n", list_size);

	for(i = 0; i < list_size; i++){
		if(!strcmp(list[i].action,"check in")){
			patient_count++;
			patient_list = (Patient *)realloc(patient_list,sizeof(Patient)*patient_count);
			patient_list[patient_count-1] = *initialize(list[i].month, list[i].day, 
				list[i].year, list[i].patientID, list[i].infected_by);
		}else if(!strcmp(list[i].action,"check out")){
			; /*unfinished*/
		}else{
			; /*unfinished*/
		}
	}

	fprintf(out,"Total number of patients: %d\n", patient_count);
	for(i = 0; i < patient_count; i++){
		fprintf(out,"Patient ID: %d\n", patient_list[i].patientID);
		fprintf(out,"Check in Date: %d/%d/%d\n\n", patient_list[i].check_in_month
			,patient_list[i].check_in_day, patient_list[i].check_in_year);
	}

	/* I still need to link the patients together */

	return 0;
}