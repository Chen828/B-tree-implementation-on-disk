#include "pch.h"

extern struct config_information config_inf;				//config信息列表//b+tree.c
extern struct disk_file_information disk_file[FILE_AMOUNT];	//b+tree.c
extern void creat_define_config();							//writer_config.c

static FILE *config_point;
static char lineBuf[Max_Line_LENGTH] = { 0 };	//行缓冲
static char *name_judge = NULL;					//name开始位置
static int kind_flag;							//0.flag_time; 1.block; 2.File_name; 3.File_size
int temp;								//临时变量
int rebuild_flag = 1;					//是否重建表标志，1：默认重建；0：不建


void get_value()
{
	//char *mark_temp = NULL;	//mark position
	char *value_start = NULL; //value开始位置
	char *value_endif = NULL;  //value结束位置
	//char *temp = NULL;	//temp position
	char *tempValue = NULL;	//temp position 读size用
	int len1;
	//char value_buf[FILE_NAME_LENGTH] = { 0 };	//文件名缓冲


	//value start position
	value_start = strchr(lineBuf, '=') + 1;//lineBuf中字符和=匹配，指针指向=后一个字符
	//printf("start=%s\n",start);
	value_endif = value_start;

	//0.flag_time; 1.block; 2.File_name; 3.File_size
	switch (kind_flag)
	{
	case 0:
		//0.flag_time

		//int flag_time;	//flag_time 初次读取config？
		//printf("flag_time = %d", *(value_start));
		//flag_time = atoi(value_start);//把字符串temp转换为整形
		printf(" flag_time = %d\n", atoi(value_start));  //atoi翻译字符串到整型//1:have read ,0 have not read
		if (atoi(value_start))
		{
			printf(" Have read config! Do you want to build file again?(1:yes/0:no)\n");
			temp = 2;
			while (temp !=0 && temp != 1)
			{
				scanf("%d", &temp);
				if (temp == 0)	// "= return 1"
				{
					rebuild_flag = 0;//不重建
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
		len1 = value_endif - value_start;//为了计算正确长度，不减一：0 1 2 k，3-0=3个
		tempValue = (char*)malloc(len1 * sizeof(char));
		memcpy(tempValue, value_start, len1);//将以start指针开始的en1个字节的内容复制到tempValue中,tempValue是指针或数组都可以
		config_inf.block_size = atoi(tempValue);//把字符串temp转换为整形
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
		len1 = value_endif - value_start;//为了计算正确长度，不减一：0 1 2 k，3-0=3个
		tempValue = (char*)malloc(len1 * sizeof(char));
		memcpy(tempValue, value_start, len1);//将以start指针开始的en1个字节的内容复制到tempValue中,tempValue是指针或数组都可以
		disk_file[config_inf.disk_file_amount].file_size = atol(tempValue);//把字符串temp转换为整形
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
	
	long now;//记录config_point位置

	//open file                           写flag
	if ((config_point = fopen("config.ini", "r+")) == NULL) //rb代表按照二进制的方式进行读
	{
		printf(" Can't open config.ini!\nPlease define config.ini！(1:yes/0:no)\n");
		scanf("%d", &temp);
		if (temp == 1)	// "= return 0"
		{
			creat_define_config();
		}
		else
		{
			printf(" So byebye！\n");
			exit(0);
		}
	}
		
	//circulation read a line
	while (1)
	{	
		//mate name 根据行前边的名字判断本行是什么信息
		name_judge = fgets(lineBuf, Max_Line_LENGTH, config_point);//一次读取一行,每次最多读取1024个字节,读取第一行
		//printf("%s\n", name_judge);//第一行的所有内容

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
			//write flag_time   临时point，使config_point正常读
			//fp = config_point;
			now = ftell(config_point);
			fseek(config_point, 10, 0);// 将文件的指针 移至 指定大小的位置

			//写入1，表示已读
			fputc('1', config_point);


			//fclose(fp);
			fseek(config_point, now, 0);// 定位开头，重新到行结束
			//name_judge = fgets(lineBuf, Max_Line_LENGTH, config_point);//一次读取一行,每次最多读取1024个字节,读取第一行

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
			//disk_file.file_amount++;//文件大小取定，这个文件才结束
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

	//创建磁盘文件
	if (rebuild_flag)
	{
		//覆盖
		int control_size = 0;
		for (int i = 0; i < config_inf.disk_file_amount; i++)
		{
			//为了读写，建立一个新的二进制文件
			if ((disk_file[i].file_point = fopen(disk_file[i].file_name, "wb+")) == NULL)
			{
				printf(" 不能打开:%s\n", disk_file[i].file_name);
				//exit(0);
			}
			//printf("........\n");
			fseek(disk_file[i].file_point, disk_file[i].file_size * 1024 * 1024 - 1, 0);// 将文件的指针 移至 指定大小的位置
			//在要指定大小文件的末尾随便放一个数据,如果不写此句，则创建出的文件的大小都为0KB
			fputc(0, disk_file[i].file_point);//在要指定大小文件的末尾随便放一个数据,如果不写此句，则创建出的文件的大小都为0KB
		}
	}
	else
	{
		//不覆盖
		for (int i = 0; i < config_inf.disk_file_amount; i++)
		{
			//为了读写，打开一个二进制文件
			if ((disk_file[i].file_point = fopen(disk_file[i].file_name, "rb+")) == NULL)
			{
				printf(" 不能打开:%s\n", disk_file[i].file_name);
				//exit(0);
			}
		}
	}

	//关流
	fclose(config_point);

	return rebuild_flag;
}