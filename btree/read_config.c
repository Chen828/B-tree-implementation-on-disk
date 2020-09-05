#include "pch.h"

extern struct config_information config_inf;				//config��Ϣ�б�//b+tree.c
extern struct disk_file_information disk_file[FILE_AMOUNT];	//b+tree.c
extern void creat_define_config();							//writer_config.c

static FILE *config_point;
static char lineBuf[Max_Line_LENGTH] = { 0 };	//�л���
static char *name_judge = NULL;					//name��ʼλ��
static int kind_flag;							//0.flag_time; 1.block; 2.File_name; 3.File_size
int temp;								//��ʱ����
int rebuild_flag = 1;					//�Ƿ��ؽ����־��1��Ĭ���ؽ���0������


void get_value()
{
	//char *mark_temp = NULL;	//mark position
	char *value_start = NULL; //value��ʼλ��
	char *value_endif = NULL;  //value����λ��
	//char *temp = NULL;	//temp position
	char *tempValue = NULL;	//temp position ��size��
	int len1;
	//char value_buf[FILE_NAME_LENGTH] = { 0 };	//�ļ�������


	//value start position
	value_start = strchr(lineBuf, '=') + 1;//lineBuf���ַ���=ƥ�䣬ָ��ָ��=��һ���ַ�
	//printf("start=%s\n",start);
	value_endif = value_start;

	//0.flag_time; 1.block; 2.File_name; 3.File_size
	switch (kind_flag)
	{
	case 0:
		//0.flag_time

		//int flag_time;	//flag_time ���ζ�ȡconfig��
		//printf("flag_time = %d", *(value_start));
		//flag_time = atoi(value_start);//���ַ���tempת��Ϊ����
		printf(" flag_time = %d\n", atoi(value_start));  //atoi�����ַ���������//1:have read ,0 have not read
		if (atoi(value_start))
		{
			printf(" Have read config! Do you want to build file again?(1:yes/0:no)\n");
			temp = 2;
			while (temp !=0 && temp != 1)
			{
				scanf("%d", &temp);
				if (temp == 0)	// "= return 1"
				{
					rebuild_flag = 0;//���ؽ�
					//printf("")
				}
			}		
		}
		break;
	case 1:
		//1.block_size
		while (1) {
			if (*value_endif == 'K') {
				break;
			}
			else {
				value_endif++;
			}
		}		
		len1 = value_endif - value_start;//Ϊ�˼�����ȷ���ȣ�����һ��0 1 2 k��3-0=3��
		tempValue = (char*)malloc(len1 * sizeof(char));
		memcpy(tempValue, value_start, len1);//����startָ�뿪ʼ��en1���ֽڵ����ݸ��Ƶ�tempValue��,tempValue��ָ������鶼����
		config_inf.block_size = atoi(tempValue);//���ַ���tempת��Ϊ����
		printf(" block_size = %d KB\n", config_inf.block_size);
		break;
	case 2:
		//2.File_name
		while (1) {
			if (*value_endif == '\n') {
				break;
			}
			else {
				value_endif++;
			}
		}
		for (int i = 0; value_start + i < value_endif; i++)
		{
			disk_file[config_inf.disk_file_amount].file_name[i] = *(value_start + i);
		}
		printf(" file_name%d = %s\n", config_inf.disk_file_amount + 1, disk_file[config_inf.disk_file_amount].file_name);
		break;
	case 3:
		//3.File_size
		while (1) {
			if (*value_endif == 'M') {
				break;
			}
			else {
				value_endif++;
			}
		}
		len1 = value_endif - value_start;//Ϊ�˼�����ȷ���ȣ�����һ��0 1 2 k��3-0=3��
		tempValue = (char*)malloc(len1 * sizeof(char));
		memcpy(tempValue, value_start, len1);//����startָ�뿪ʼ��en1���ֽڵ����ݸ��Ƶ�tempValue��,tempValue��ָ������鶼����
		disk_file[config_inf.disk_file_amount].file_size = atol(tempValue);//���ַ���tempת��Ϊ����
		printf(" file_size%d = %d MB\n", config_inf.disk_file_amount + 1, disk_file[config_inf.disk_file_amount].file_size);
		break;
	default:
		break;
	}
}


//read "config.ini"
int read_config()
{
	int flag_flag = 0;
	int block_flag = 0;
	
	long now;//��¼config_pointλ��

	//open file                           дflag
	if ((config_point = fopen("config.ini", "r+")) == NULL) //rb�����ն����Ƶķ�ʽ���ж�
	{
		printf(" Can't open config.ini!\nPlease define config.ini��(1:yes/0:no)\n");
		scanf("%d", &temp);
		if (temp == 1)	// "= return 0"
		{
			creat_define_config();
		}
		else
		{
			printf(" So byebye��\n");
			exit(0);
		}
	}
		
	//circulation read a line
	while (1)
	{	
		//mate name ������ǰ�ߵ������жϱ�����ʲô��Ϣ
		name_judge = fgets(lineBuf, Max_Line_LENGTH, config_point);//һ�ζ�ȡһ��,ÿ������ȡ1024���ֽ�,��ȡ��һ��
		//printf("%s\n", name_judge);//��һ�е���������

		if (name_judge == NULL)
		{
			printf(" Can't read new line!\n");
			break;
		}

		if (*(name_judge) == 'f' && flag_flag == 0)
		{
			//flag
			kind_flag = 0;
			get_value();
			flag_flag = 1;
			//write flag_time   ��ʱpoint��ʹconfig_point������
			//fp = config_point;
			now = ftell(config_point);
			fseek(config_point, 10, 0);// ���ļ���ָ�� ���� ָ����С��λ��

			//д��1����ʾ�Ѷ�
			fputc('1', config_point);


			//fclose(fp);
			fseek(config_point, now, 0);// ��λ��ͷ�����µ��н���
			//name_judge = fgets(lineBuf, Max_Line_LENGTH, config_point);//һ�ζ�ȡһ��,ÿ������ȡ1024���ֽ�,��ȡ��һ��

		}
		else if (*(name_judge) == 'b' && block_flag == 0)
		{
			//block
			kind_flag = 1;
			get_value();
			block_flag = 1;
		}
		else if (*(name_judge + 5) == 'n')
		{
			//File_name
			kind_flag = 2;
			get_value();
			//disk_file.file_amount++;//�ļ���Сȡ��������ļ��Ž���
		}
		else if (*(name_judge + 5) == 's')
		{
			//File_size
			kind_flag = 3;
			get_value();
			config_inf.disk_file_amount++;
		}/*
		else
		{
			printf("Have not mate name!\n");
		}*/
	}

	//���������ļ�
	if (rebuild_flag)
	{
		//����
		int control_size = 0;
		for (int i = 0; i < config_inf.disk_file_amount; i++)
		{
			//Ϊ�˶�д������һ���µĶ������ļ�
			if ((disk_file[i].file_point = fopen(disk_file[i].file_name, "wb+")) == NULL)
			{
				printf(" ���ܴ�:%s\n", disk_file[i].file_name);
				//exit(0);
			}
			//printf("........\n");
			fseek(disk_file[i].file_point, disk_file[i].file_size * 1024 * 1024 - 1, 0);// ���ļ���ָ�� ���� ָ����С��λ��
			//��Ҫָ����С�ļ���ĩβ����һ������,�����д�˾䣬�򴴽������ļ��Ĵ�С��Ϊ0KB
			fputc(0, disk_file[i].file_point);//��Ҫָ����С�ļ���ĩβ����һ������,�����д�˾䣬�򴴽������ļ��Ĵ�С��Ϊ0KB
		}
	}
	else
	{
		//������
		for (int i = 0; i < config_inf.disk_file_amount; i++)
		{
			//Ϊ�˶�д����һ���������ļ�
			if ((disk_file[i].file_point = fopen(disk_file[i].file_name, "rb+")) == NULL)
			{
				printf(" ���ܴ�:%s\n", disk_file[i].file_name);
				//exit(0);
			}
		}
	}

	//����
	fclose(config_point);

	return rebuild_flag;
}