// b+tree.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������

#include "pch.h"

extern struct config_information config_inf;					//config��Ϣ�б�//b+tree.c
extern struct disk_file_information disk_file[FILE_AMOUNT];		//b+tree.c

static FILE *d_config_point;


//creat file and write in define_config.ini
void write_file()
{
	//char file_name[FILE_NAME_LENGTH];
	long file_size;

	//��ȡ����
	printf(" ������file_name(Ĭ���ļ���׺.txt����������):\n");
	scanf("%s", disk_file[config_inf.disk_file_amount].file_name);
	printf(" ������file_size(Ĭ�ϵ�λMB):\n");
	scanf("%d", &disk_file[config_inf.disk_file_amount].file_size);

	//write "txt" in file_name
	strcat(disk_file[config_inf.disk_file_amount].file_name, ".txt");

	//printf("%s\n%d\n", disk_file.file_name[*disk_file.file_amount], disk_file.file_size[*disk_file.file_amount]);

	//���������ļ�
	//Ϊ�˶�д������һ���µĶ������ļ�
	if ((disk_file[config_inf.disk_file_amount].file_point = fopen(disk_file[config_inf.disk_file_amount].file_name, "wb+")) == NULL)
	{
		printf(" ���ܴ�:%s\n", disk_file[config_inf.disk_file_amount].file_name);
		//exit(0);
	}
	fseek(disk_file[config_inf.disk_file_amount].file_point, disk_file[config_inf.disk_file_amount].file_size * 1024 * 1024 - 1, 0);// ���ļ���ָ�� ���� ָ����С��λ��
	fputc(0, disk_file[config_inf.disk_file_amount].file_point);//��Ҫָ����С�ļ���ĩβ����һ������,�����д�˾䣬�򴴽������ļ��Ĵ�С��Ϊ0KB
	//fclose(disk_file[config_inf.disk_file_amount].file_point);

	//д��
	fprintf(d_config_point, "file_name%d = %s\n", config_inf.disk_file_amount+1, disk_file[config_inf.disk_file_amount].file_name);
	fprintf(d_config_point, "file_size%d = %d MB\n", config_inf.disk_file_amount+1, disk_file[config_inf.disk_file_amount].file_size);

	//���
	printf("System:\n\tfile_name%d = %s\n", config_inf.disk_file_amount + 1, disk_file[config_inf.disk_file_amount].file_name);
	printf("\tfile_size%d = %d MB\n", config_inf.disk_file_amount + 1, disk_file[config_inf.disk_file_amount].file_size);

	//�ļ�����
	config_inf.disk_file_amount++;
}


//get block_size
void write_block_size()
{
	//��ȡ����
	printf("������block_size(Ĭ�ϵ�λKB):\n");
	scanf("%d", &config_inf.block_size);
	printf("System��\n\tblock_size = %d KB\n", config_inf.block_size);
	printf("write_block_size has done!\n");

	//���ӡ�д��config
	//strcat(block_size, size);
	//strcat(block_size, "MB");
	//д��
	fprintf(d_config_point, "block_size = %d KB\n", config_inf.block_size);
	//fwrite(file_name, sizeof(char), strlen(file_name), file_point);//�����ƶ�д
}


//creat_define_config list
void setting_list()
{
	printf("===========================\n");
	printf("   �����Զ��������ļ���\n");
	printf("   ===================\n");
	printf("   1 ���������ļ�\n");
	printf("   2 ����block_size\n");
	printf("   0 �˳�����\n");
	printf("===========================\n");
	printf(" �������ţ�");
}


//�������
void display()
{
	printf("===========================\n");
	printf(" block_size = %d KB\n", config_inf.block_size);
	for (int i = 0; i < config_inf.disk_file_amount; i++)
	{
		printf(" file_name%d = %s\n", i + 1, disk_file[i].file_name);
		printf(" file_size%d = %d MB\n", i + 1, disk_file[i].file_size);
	}
	printf("===========================\n");
}


//�����ļ���д��config
void creat_define_config()//define_config.iniָ��
{
	char define_config_name[] = "define_config.ini";
	//char filename[] = "./define_config.ini";

	static int choose = 1;//Ŀ¼ѡ��
	static int flag = 1;//block_sizeֻ������һ��

	//���������ļ�
	//file_point = fopen(filename, "a");//׷�Ӷ�д���������򴴽�
	fopen_s(&d_config_point, define_config_name, "w");//д���������򴴽�

	while (choose)
	{
		setting_list();//creat_define_config list

		scanf("%d", &choose);
		switch (choose)
		{
		case 1:
			write_file();
			break;
		case 2:
			if (flag)
			{
				write_block_size();
				flag = 0;
			}
			else
			{
				printf(" write_block_size was already done!\n");
			}
			break;
		case 0:
		default:
			break;
		}
	}

	//���
	display();

	fclose(d_config_point);
}

