//5 查询关键字
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct fat_block_structure fat_s;	//b+tree.c
extern struct config_information config_inf;	//config信息列表
extern struct save_content s_c[content_num];					//目录块
struct data_structure data_str;				//table数据表中数据格式


int fat_search_data(int table_num, int key, int *target_unit) {		//关键字，记录小于目标关键字的块号（小于取最大）
	int content_block, unit, next_unit;	//表项值=1 表示该表项值有数据，下一表项值，
	long fat_position, block_position;
	//int first_unit = 0;	//起始数据表项
	
	//存储文件的表项值:0无数据；1（块号）未找到关键字（记录小于关键字中的最大关键字位于的块号）；2存在关键字
	int read_over_flag = 0;	

	//printf("=============== 5 查询关键字 ===============\n");

	//起始位置号
	if (s_c[table_num].data_block == 0)
	{
		printf("数据表不存在！\n");
		return 0;
	}
	else
	{
		fat_position = ((s_c[table_num].data_block / fat_s.fat_value_amount + 1) + s_c[table_num].data_block) * sizeof(int);
		fseek(disk_file[0].file_point, fat_position, 0);	//定位文件起始

		fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
		unit = s_c[table_num].data_block;	//从第一个block开始读
		while (next_unit != 0 && !read_over_flag)	//非空闲block
		{
			//计算当下数据block的位置，读出数据
			block_position = unit * config_inf.block_size * 1024;
			fseek(disk_file[0].file_point, block_position + sizeof(int), 0);

			fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

			if (data_str.key < key)
			{
				//记录小于关键字中的最大关键字位于的块号
				*target_unit = unit;
			}
			else if (data_str.key == key)
			{
				//关键字存在
				printf(" block_num = %d ; fat_position = %d ; block_position = %dB \n", unit, fat_position, block_position);
				//printf(" next_block_num = %d\n", next_unit);
				read_over_flag = 2;
			}

			//读取下一fat表项值
			if (!read_over_flag)
			{
				if (next_unit != 1)
				{
					unit = next_unit;	//保存块号，以读下一表项值

					//计算next_unit在fat中的位置
					fat_position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
					//next_unit / fat_s.fat_value_amount + 1占了几个block
					//定位
					fseek(disk_file[0].file_point, fat_position, 0);
					//读取next_unit在fat中的表项值
					fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
				}
				else
				{
					read_over_flag = 1;
				}
			}
		}

	}
	/*
	//fat块循环读
	for (int i = 0; i < fat_s.fat_block_amount && !read_over_flag; i++)
	{
		fseek(disk_file[0].file_point, sizeof(int), 1);	//跳过每块的块号

		//定位每个block中的起始数据表项
		//0块里存的还有与fat对应的单元格
		if (!i)
		{
			fseek(disk_file[0].file_point, fat_s.fat_block_amount * sizeof(int), 1);
			first_unit = fat_s.fat_block_amount;	//fat占用block个数的数值 = 第一个存数据的表项值标签对应的块号的数值；
		}

		//表项值循环找
		unit = first_unit;
		for (int j = first_unit; j < fat_s.fat_value_amount && !read_over_flag; j++)
		{
			fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);

			while (next_unit != 0 && !read_over_flag)	//非空闲block
			{
				//计算当下数据block的位置，读出数据
				block_position = unit * config_inf.block_size * 1024;
				fseek(disk_file[0].file_point, block_position + sizeof(int), 0);

				fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

				if (data_str.key < key)
				{
					//记录小于关键字中的最大关键字位于的块号
					*target_unit = unit;
				}
				else if (data_str.key == key)
				{
					//关键字存在
					printf(" block_num = %d ; fat_position = %d ; block_position = %dB \n", unit, fat_position, block_position);
					//printf(" next_block_num = %d\n", next_unit);
					read_over_flag = 2;
				}

				//读取下一fat表项值
				if (!read_over_flag)
				{
					if (next_unit != 1)
					{
						unit = next_unit;	//保存块号，以读下一表项值

						//计算next_unit在fat中的位置
						fat_position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
						//next_unit / fat_s.fat_value_amount + 1占了几个block
						//定位
						fseek(disk_file[0].file_point, fat_position, 0);
						//读取next_unit在fat中的表项值
						fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
					}
					else
					{
						read_over_flag = 1;
					}
				}
			}
		}
	}*/

	//查询结果
	if (read_over_flag == 0)
	{
		printf("无数据存储！\n");

		return 0;
	}
	else if (read_over_flag == 1)
	{
		printf(" 关键字不存在！\n");

		return 1;
	}
	else
	{
		//read_over_flag == 2
		printf(" data_str.key = %3d, data_str.name = %s\n", data_str.key, data_str.name);

		return 2;
	}
}