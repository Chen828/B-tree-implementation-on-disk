// b+tree.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "pch.h"

extern struct config_information config_inf;					//config信息列表//b+tree.c
extern struct disk_file_information disk_file[FILE_AMOUNT];		//b+tree.c

static FILE *d_config_point;


//creat file and write in define_config.ini
void write_file()
{
	//char file_name[FILE_NAME_LENGTH];
	long file_size;

	//获取输入
	printf(" 请输入file_name(默认文件后缀.txt，无需输入):\n");
	scanf("%s", disk_file[config_inf.disk_file_amount].file_name);
	printf(" 请输入file_size(默认单位MB):\n");
	scanf("%d", &disk_file[config_inf.disk_file_amount].file_size);

	//write "txt" in file_name
	strcat(disk_file[config_inf.disk_file_amount].file_name, ".txt");

	//printf("%s\n%d\n", disk_file.file_name[*disk_file.file_amount], disk_file.file_size[*disk_file.file_amount]);

	//创建磁盘文件
	//为了读写，建立一个新的二进制文件
	if ((disk_file[config_inf.disk_file_amount].file_point = fopen(disk_file[config_inf.disk_file_amount].file_name, "wb+")) == NULL)
	{
		printf(" 不能打开:%s\n", disk_file[config_inf.disk_file_amount].file_name);
		//exit(0);
	}
	fseek(disk_file[config_inf.disk_file_amount].file_point, disk_file[config_inf.disk_file_amount].file_size * 1024 * 1024 - 1, 0);// 将文件的指针 移至 指定大小的位置
	fputc(0, disk_file[config_inf.disk_file_amount].file_point);//在要指定大小文件的末尾随便放一个数据,如果不写此句，则创建出的文件的大小都为0KB
	//fclose(disk_file[config_inf.disk_file_amount].file_point);

	//写入
	fprintf(d_config_point, "file_name%d = %s\n", config_inf.disk_file_amount+1, disk_file[config_inf.disk_file_amount].file_name);
	fprintf(d_config_point, "file_size%d = %d MB\n", config_inf.disk_file_amount+1, disk_file[config_inf.disk_file_amount].file_size);

	//输出
	printf("System:\n\tfile_name%d = %s\n", config_inf.disk_file_amount + 1, disk_file[config_inf.disk_file_amount].file_name);
	printf("\tfile_size%d = %d MB\n", config_inf.disk_file_amount + 1, disk_file[config_inf.disk_file_amount].file_size);

	//文件总数
	config_inf.disk_file_amount++;
}


//get block_size
void write_block_size()
{
	//获取输入
	printf("请输入block_size(默认单位KB):\n");
	scanf("%d", &config_inf.block_size);
	printf("System：\n\tblock_size = %d KB\n", config_inf.block_size);
	printf("write_block_size has done!\n");

	//连接、写入config
	//strcat(block_size, size);
	//strcat(block_size, "MB");
	//写入
	fprintf(d_config_point, "block_size = %d KB\n", config_inf.block_size);
	//fwrite(file_name, sizeof(char), strlen(file_name), file_point);//二进制读写
}


//creat_define_config list
void setting_list()
{
	printf("===========================\n");
	printf("   创建自定义配置文件！\n");
	printf("   ===================\n");
	printf("   1 创建磁盘文件\n");
	printf("   2 设置block_size\n");
	printf("   0 退出设置\n");
	printf("===========================\n");
	printf(" 请输入编号：");
}


//输出配置
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


//创建文件并写入config
void creat_define_config()//define_config.ini指针
{
	char define_config_name[] = "define_config.ini";
	//char filename[] = "./define_config.ini";

	static int choose = 1;//目录选项
	static int flag = 1;//block_size只能设置一次

	//创建配置文件
	//file_point = fopen(filename, "a");//追加读写，不存在则创建
	fopen_s(&d_config_point, define_config_name, "w");//写，不存在则创建

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

	//输出
	display();

	fclose(d_config_point);
}

