#define _XOPEN_SOURCE 700

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct Patient
{
	int is_not_null;
	char name[50];
	char year[10];
	char phone_number[15];
	char is_paid[10];
	char is_vaccinated[15];
} PATIENT;

void handler(int signo, siginfo_t *info, void *context)
{
	int bus = info->si_value.sival_int;
	if(signo == 10)
	{
		printf("1-es oltóbusz: HARCRA FEL!\n");
	}
	else if(signo == 12)
	{
		printf("2-es oltóbusz: HARCRA FEL!\n");
	}
	else if(signo == 15)
	{
		printf("A(z) %d-es oltóbusz teljesítette a mai oltásokat.\n", bus);
	}
	
	
}

PATIENT read_one_patient_from_file(FILE *file_ptr)
{
	PATIENT patient;
	patient.is_not_null = 0;
	char line[100];
	if(fgets(line, sizeof line, file_ptr) != NULL)
	{
		sscanf(line, "%[^,], %[^,], %[^,], %[^,], %[^,\r\n]", patient.name, patient.year, patient.phone_number, patient.is_paid, patient.is_vaccinated);
		patient.is_not_null = 1;
		return patient;
	}
	else
	{
		return patient;
	}
}

char *read_input() {
    char *input = 0;
    size_t length = 0;
    getline(&input, &length, stdin);
    input[strlen(input) - 1] = 0;
    return input;
}

void write_patient_into_file(FILE *file_ptr, PATIENT patient)
{
	fprintf(file_ptr, "%s, %s, %s, %s, %s\n", patient.name, patient.year, patient.phone_number, patient.is_paid, patient.is_vaccinated);
}

void add_patient(FILE *file_ptr)
{
	file_ptr =  fopen("paciensek.txt", "a");
	int new_patient = 0;
	while(new_patient == 0)
	{
		PATIENT patient;
		patient.is_not_null = 1;
		printf("\n");
					printf("Új páciens regisztrálása\n");

					printf("Páciens neve: ");	
					fgets(patient.name, 50, stdin);
					patient.name[strcspn(patient.name, "\n")] = 0;
					
					printf("Páciens születési éve: ");
					fgets(patient.year, 10, stdin);
					patient.year[strcspn(patient.year, "\n")] = 0;

					printf("Páciens telefonszáma: ");
					fgets(patient.phone_number, 15, stdin);
					patient.phone_number[strcspn(patient.phone_number, "\n")] = 0;

					printf("A fizetős szolgáltatást szeretné igénybe venni? (IGEN/NEM): ");
					fgets(patient.is_paid, 10, stdin);
					patient.is_paid[strcspn(patient.is_paid, "\n")] = 0;
					if(strlen(patient.is_paid) > 0)
					{
						int i = 0;
						while(patient.is_paid[i])
						{
							patient.is_paid[i] = toupper(patient.is_paid[i]);
							++i;
						}
					}
					else
					{
						strcpy(patient.is_paid, "NEM");
					}

					printf("Páciens oltottsági státusza? (OLTVA/NEM OLTOTT): ");
					fgets(patient.is_vaccinated, 15, stdin);
					patient.is_vaccinated[strcspn(patient.is_vaccinated, "\n")] = 0;
					if(strlen(patient.is_vaccinated) > 0)
					{
						int i = 0;
						while(patient.is_vaccinated[i])
						{
							patient.is_vaccinated[i] = toupper(patient.is_vaccinated[i]);
							++i;
						}
					}
					else
					{
						strcpy(patient.is_vaccinated, "NEM OLTOTT");
					}

					write_patient_into_file(file_ptr, patient);

					printf("\n");
					printf("Szeretne további pácienst regisztrálni?\n");
					printf("[0] Igen, [1] Nem\n");
					scanf("%d", &new_patient);
					while(getchar() != '\n');
	}
	fclose(file_ptr);
}

void edit_patient(FILE *file_ptr, FILE *tmp_file_ptr, char *phone_number, int edit_or_delete)
{
	file_ptr = fopen("paciensek.txt", "r+");
	tmp_file_ptr = fopen("temp.txt","w");
	
	PATIENT patient = read_one_patient_from_file(file_ptr);

	switch(edit_or_delete)
	{
		case 1 : //edit patient
		{
			while(patient.is_not_null)
			{
				if(strcmp(patient.phone_number, phone_number) == 0)
				{
					char *edited_name;
					char *edited_year;
					char *edited_phone_number;
					char *edited_phone_number2;
					char *edited_is_paid;
					char *edited_is_vaccinated;
					size_t lenght = 0;

					printf("A módosítani nem kívánt mezőt hagyja üresen és nyomjon egy ENTER-t!\n");
					printf("Páciens neve: ");
					edited_name = read_input();
					if(strlen(edited_name) > 0)
					{
						strcpy(patient.name, edited_name);
					}
					printf("Páciens születési éve: ");
					edited_year = read_input();
					if(strlen(edited_year) > 0)
					{
						strcpy(patient.year, edited_year);
					}
					printf("Páciens telefonszáma: ");
					edited_phone_number = read_input();
					if(strlen(edited_phone_number) > 0)
					{
						strcpy(patient.phone_number, edited_phone_number);
					}
					printf("A fizetős szolgáltatást szeretné igénybe venni? (Igen/Nem): ");
					edited_is_paid = read_input();
					if(strlen(edited_is_paid) > 0)
					{
						int i = 0;
						while(edited_is_paid[i])
						{
							edited_is_paid[i] = toupper(edited_is_paid[i]);
							++i;
						}
						strcpy(patient.is_paid, edited_is_paid);
					}
					printf("Páciens oltottsági státusza? (OLTVA/NEM OLTOTT):  ");
					edited_is_vaccinated = read_input();
					if(strlen(edited_is_vaccinated) > 0)
					{
						int i = 0;
						while(edited_is_vaccinated[i])
						{
							edited_is_vaccinated[i] = toupper(edited_is_vaccinated[i]);
							++i;
						}
						strcpy(patient.is_vaccinated, edited_is_vaccinated);
					}
				}
				write_patient_into_file(tmp_file_ptr, patient);
				patient = read_one_patient_from_file(file_ptr);
			}
			break;
		}
		case 2 : //delete patient
		{
			while(patient.is_not_null)
			{
				if(strcmp(patient.phone_number, phone_number) != 0)
				{	
					write_patient_into_file(tmp_file_ptr, patient);
					patient = read_one_patient_from_file(file_ptr);
				}
				else
				{
					patient = read_one_patient_from_file(file_ptr);
				}
				
			}
			break;
		}
	}

	fclose(file_ptr);
	fclose(tmp_file_ptr);
	remove("paciensek.txt");
	rename("temp.txt", "paciensek.txt");
}

void update_vaccinate_status(PATIENT patient)
{
	FILE *file_ptr = fopen("paciensek.txt", "r+");
	FILE *new_file_ptr = fopen("temp.txt", "w");
	
	PATIENT other = read_one_patient_from_file(file_ptr);
	while(other.is_not_null)
	{
		if(strcmp(patient.phone_number, other.phone_number) == 0)
			{
					strcpy(other.is_vaccinated, "OLTVA");
			}
			write_patient_into_file(new_file_ptr, other);
			other = read_one_patient_from_file(file_ptr);
			
	}

	fclose(file_ptr);
	fclose(new_file_ptr);
	remove("paciensek.txt");
	rename("temp.txt", "paciensek.txt");
}

void create_pipe(const char *pipe_name)
{
	int pipe_id = mkfifo(pipe_name, S_IRUSR | S_IWUSR);
	if (pipe_id == -1)
	{
		printf("Error: Pipe %s may exists!\n", pipe_name);
	}
}

void close_pipe(int pipe, char *pipe_name)
{
	close(pipe);
	unlink(pipe_name);
}

void bus_action(int bus_number, char *ch_to_p_name, char *p_to_ch_name, union sigval sv, int signum)
{
	srand(time(NULL));
	sleep(2);
	printf("A(z) %d-es oltóbusz elindult.\n", bus_number);
	sigqueue(getppid(), signum, sv);
	printf("A(z) %d-es oltóbusz megérkezett.\n", bus_number);
	int ch_to_p1 = open(ch_to_p_name, O_WRONLY);
	int p_to_ch1 = open(p_to_ch_name, O_RDONLY);

	sleep(2);
	PATIENT patients_wait_for_vaccine[5];
	int num_of_vaccinated = 0;
	for (int i = 0; i < 5; ++i)
	{
		PATIENT patient;
		read(p_to_ch1, &patient, sizeof (PATIENT));

		int chance_to_arrive = rand() % 100 + 1;
		if (chance_to_arrive < 90)
		{
			printf("%s megérkezett a(z) %d-es oltóbuszhoz és megkapta az oltást.\n", patient.name, bus_number);
			patients_wait_for_vaccine[num_of_vaccinated] = patient;
			num_of_vaccinated++;
		}
		else
		{
			printf("%s nem érkezett meg a(z) %d-es oltóbuszhoz.\n", patient.name, bus_number);
		}
	}

	write(ch_to_p1, &num_of_vaccinated, sizeof (int));
	for (int i = 0; i < num_of_vaccinated; ++i)
	{
		write(ch_to_p1, &patients_wait_for_vaccine[i], sizeof(PATIENT));
	}

	sigqueue(getppid(), SIGTERM, sv);
	close(ch_to_p1);
	close(p_to_ch1);
	exit(0);
		
}

void start_mission(PATIENT patients[], int size)
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	siginfo_t sinfo;

	union sigval sv;
	sv.sival_int = 1;

	union sigval sv2;
	sv2.sival_int = 2;

	struct timespec timer;
	timer.tv_sec = 3;

	char *B1PC = "./b1pc_bt5vgx";
	char *B1CP = "./b1cp_bt5vgx";
	char *B2PC = "./b2pc_bt5vgx";
	char *B2CP = "./b2cp_bt5vgx";

	create_pipe(B1CP);
	create_pipe(B1PC);

	pid_t pid1, pid2;
	pid1 = fork();
	if (pid1 > 0) // parent
	{
		if (size > 9)
		{
			create_pipe(B2CP);
			create_pipe(B2PC);

			pid2 = fork();
			if (pid2 > 0) // parent with two busses
			{
				printf("Ma mindkét oltóbusz kimegy.\n");
				int ch_to_p1 = open(B1CP, O_RDONLY);
				int p_to_ch1 = open(B1PC, O_WRONLY);
				int ch_to_p2 = open(B2CP, O_RDONLY);
				int p_to_ch2 = open(B2PC, O_WRONLY);

				sigemptyset(&sa.sa_mask);
				sigaddset(&sa.sa_mask, SIGUSR1);
				sigtimedwait(&sa.sa_mask, &sinfo, &timer);
				sleep(1);
				sigemptyset(&sa.sa_mask);
				sigaddset(&sa.sa_mask, SIGUSR2);
				sigtimedwait(&sa.sa_mask, &sinfo, &timer);
				sleep(1);

				for (int i = 0; i < 5; ++i)
				{
					printf("%s kérem fáradjon az 1-es oltóbuszhoz.\n", patients[i].name);
					write(p_to_ch1, &patients[i], sizeof (PATIENT));
					printf("%s kérem fáradjon az 2-es oltóbuszhoz.\n", patients[i+5].name);
					write(p_to_ch2, &patients[i+5], sizeof (PATIENT));
				}

				sigemptyset(&sa.sa_mask);
				sigaddset(&sa.sa_mask, SIGTERM);
				sigtimedwait(&sa.sa_mask, &sinfo, &timer);
				sleep(1);
				sigtimedwait(&sa.sa_mask, &sinfo, &timer);
				sleep(1);

				printf("\n");
				int vaccinated_count;
				read(ch_to_p1, &vaccinated_count, sizeof (int));
				for (int i = 0; i < vaccinated_count; ++i)
				{
					PATIENT patient;
					read(ch_to_p1, &patient, sizeof (PATIENT));
					update_vaccinate_status(patient);
					printf("%s megkapta az OLTVA státuszt.\n", patient.name);
				}

				read(ch_to_p2, &vaccinated_count, sizeof (int));
				for (int i = 0; i < vaccinated_count; ++i)
				{
					PATIENT patient;
					read(ch_to_p2, &patient, sizeof (PATIENT));
					update_vaccinate_status(patient);
					printf("%s megkapta az OLTVA státuszt.\n", patient.name);
				}

				close_pipe(ch_to_p1, B1CP);
				close_pipe(p_to_ch1, B1PC);
				close_pipe(ch_to_p2, B2CP);
				close_pipe(p_to_ch2, B2PC);
			}
			else // bus 2
			{
				bus_action(2, B2CP, B2PC, sv2, SIGUSR2);

			}
		}
		else // parent with one bus
		{
			printf("Mai nap egy oltóbusz indul útnak.\n");
			int ch_to_p1 = open(B1CP, O_RDONLY);
			int p_to_ch1 = open(B1PC, O_WRONLY);

			sigemptyset(&sa.sa_mask);
			sigaddset(&sa.sa_mask, SIGUSR1);
			sigtimedwait(&sa.sa_mask, &sinfo, &timer);
			sleep(1);

			for (int i = 0; i < 5; ++i)
			{
				printf("%s kérem fáradjon az 1-es oltóbuszhoz.\n", patients[i].name);
				write(p_to_ch1, &patients[i], sizeof (PATIENT));
			}

			sigemptyset(&sa.sa_mask);
			sigaddset(&sa.sa_mask, SIGTERM);
			sigtimedwait(&sa.sa_mask, &sinfo, &timer);
			sleep(1);

			printf("\n");
			int vaccinated_count;
			read(ch_to_p1, &vaccinated_count, sizeof (int));
			for (int i = 0; i < vaccinated_count; ++i)
			{
				PATIENT patient;
				read(ch_to_p1, &patient, sizeof (PATIENT));
				update_vaccinate_status(patient);
				printf("%s megkapta az OLTVA státuszt.\n", patient.name);
				
			}

			close_pipe(ch_to_p1, B1CP);
			close_pipe(p_to_ch1, B1PC);
		}
	}
	else // bus 1
	{
		bus_action(1, B1CP, B1PC, sv, SIGUSR1);
	}
}

void is_enough_patient(FILE *file_ptr)
{
	file_ptr = fopen("paciensek.txt", "r");
	PATIENT patients[10];
	int num_of_not_vaccinated = 0;
	PATIENT patient = read_one_patient_from_file(file_ptr);
	while(patient.is_not_null && num_of_not_vaccinated < 10)
	{
		if(strcmp(patient.is_vaccinated, "NEM OLTOTT") == 0 || strcmp(patient.is_vaccinated, "NEM OLTOTT\n") == 0)
		{
			patients[num_of_not_vaccinated] = patient;
			num_of_not_vaccinated++;
		}
		patient = read_one_patient_from_file(file_ptr);
	}

	printf("\n");
	if (num_of_not_vaccinated < 5)
	{
		if (num_of_not_vaccinated == 0)
		{
			printf("Ma egy ember sem vár oltásra, nem indul oltóbusz.\n");
		}
		else
		{
			printf("Ma csak %d ember vár oltásra, nem indul oltóbusz.\n", num_of_not_vaccinated);
		}
	}
	else
	{
		start_mission(patients, num_of_not_vaccinated);
	}
}

int main()
{
	setlocale(LC_ALL, "");

	FILE *filePointer;
	FILE *newFilePointer;
	int optNum;
	
	printf("\n");
	printf("Üdvözlöm!\n");
	printf("Miben segíthetek?\n");
	while(1)
	{
		printf("[1] Új páciens regisztrálása\n");
		printf("[2] Regisztrált páciens adatainak módosítása\n");
		printf("[3] Regisztrált páciens törlése\n");
		printf("[4] Teljes lista készítése\n");
		printf("[5] Oltóbuszok indítása\n");
		printf("[6] Kilépés a programból\n");

		printf("A választott opció: ");	
		scanf("%d", &optNum);
		while(getchar() != '\n');
		
		switch(optNum)
		{
			case 1:
			{
				add_patient(filePointer);
				break;
			}		
			case 2:
			{
				char given_phone_number[15];
				printf("\n");
				printf("Regisztrált páciens adatainak módosítása\n");
				printf("Kérem adja meg a páciens telefonszámát az adatmódosításhoz: ");
				fgets(given_phone_number, sizeof given_phone_number, stdin);
				given_phone_number[strcspn(given_phone_number, "\n")] = 0;
				edit_patient(filePointer, newFilePointer, given_phone_number, 1);
				break;
			}
			case 3:
			{
				char given_phone_number[15];
				printf("\n");
				printf("Regisztrált páciens törlése\n");
				printf("Kérem adja meg a törölni kívánt páciens telefonszámát: ");
				fgets(given_phone_number, sizeof given_phone_number, stdin);
				given_phone_number[strcspn(given_phone_number, "\n")] = 0;
				edit_patient(filePointer, newFilePointer, given_phone_number, 2);
				break;
			}
			case 4:
			{
				printf("\n");
				printf("A regisztrált páciensek listája\n");
				filePointer = fopen("paciensek.txt", "r");
				char line[100];
				while(fgets(line, sizeof line, filePointer) != NULL)
				{
					printf("%s", line);
				}
				fclose(filePointer);
				break;
			}
			case 5:
				is_enough_patient(filePointer);
				break;
			case 6:
			printf("Viszlát!\n");
				exit(0);
			default:
				printf("Ismeretlen opció!\n");
				break;
		}
		printf("\n");
	}
	return 0;
}
