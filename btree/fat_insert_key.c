//2 插入一条数据
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct fat_block_structure fat_s;					//b+tree.c
extern struct config_information config_inf;				//config信息列表
struct data_structure data_str;								//table数据表中数据格式
extern struct save_content s_c[content_num];					//目录块
extern int fat_search_key(int table_num, int key, int *target_unit);			//fat_search_key.c
extern void fat_read_table(int table_num);									//fat_read_table.c
extern void content();									//b+tree.c


void fat_insert_data() {
	int unit, next_unit;	//表项值=1 表示该表项值有数据，下一表项值，
	int new_num;	//新的block号
	long fat_position, block_position;

	int query_result = 2;		//查询关键字返回值：0无数据；1未找到关键字；2存在关键字
	int small_unit =  0 ;

	int table_num;	//数据表号
	int key;	//数据关键字
	char name[MAN_NAME_LENGTH];	//数据人名

	//printf("=============== 2 插入一条数据 ===============\n");

	//查找
	content();

	printf(" 请输入数据表编号：");
	scanf("%d", &table_num);
	//输入关键字
	printf(" 请输入关键字：");
	scanf("%d", &key); 
	query_result = fat_search_key(table_num, key, &small_unit);
	
	//循环至要插入的关键字不存在
	/*while (query_result == 2)
	{
		//输入关键字
		scanf("%d", &key);

		query_result = fat_search_key(key);
		printf(" 关键字存在，请重新输入关键字：");
	}*/

	
	if (query_result == 2)
	{
		printf(" 关键字已存在!\n");
	}
	else	//插入
	{
		printf(" 请输入姓名：");
		scanf("%s", &name);

		data_str.key = key;
		strcpy(data_str.name, name);

		//无存储数据
		if (query_result == 0)
		{
			//申请首块空闲
			new_num = fat_block_apply(1, 1, 0);
			s_c[table_num].data_block = new_num;

			block_position = new_num * config_inf.block_size * 1024 + sizeof(int);	//越过块号

			//写入数据位置
			printf(" new_block_position: %d ;\n", block_position);

			fseek(disk_file[0].file_point, block_position, 0);
			fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

			printf(" 插入成功:\n");

			//读表
			fat_read_table(table_num);
		}
		else     //有存储数据
		{
			//不存在关键字，fat_search_key()返回记录小于关键字中的最大关键字位于的块号

			fat_position = ((small_unit / fat_s.fat_value_amount + 1) + small_unit) * sizeof(int);
			fseek(disk_file[0].file_point, fat_position, 0);
			fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
			
			//printf("%d\n", small_unit);
			//printf("%d\n", next_unit);

			//插入数据的block号
			new_num = fat_block_apply(1, 0, small_unit);
			
			//printf("%d\n", new_num);

			//插入数据申请的block对应的fat表项值写入下一fat表项值
			fat_position = ((new_num / fat_s.fat_value_amount + 1) + new_num) * sizeof(int);
			fseek(disk_file[0].file_point, fat_position, 0);
			fwrite(&next_unit, sizeof(int), 1, disk_file[0].file_point);

			//插入数据位置，并写入
			block_position = new_num * config_inf.block_size * 1024 + sizeof(int);	//越过块号
			printf("             new_block_position: %d ;\n", block_position);
			fseek(disk_file[0].file_point, block_position, 0);
			fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

			printf(" 插入成功：\n==============\n");

			//读表
			fat_read_table(table_num);
		}
	}
}