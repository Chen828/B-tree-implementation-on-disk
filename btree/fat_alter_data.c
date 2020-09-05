//4 修改
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct fat_block_structure fat_s;					//b+tree.c
extern struct config_information config_inf;				//config信息列表
struct data_structure data_str;								//table数据表中数据格式
extern int fat_search_data(int table_num, int key, int *target_unit);			//fat_search_data.c
extern void fat_read_table(int table_num);									//fat_read_table.c
extern void content();									//b+tree.c


void fat_alter_data() {
	int unit, next_unit;	//表项值=1 表示该表项值有数据，下一表项值，
	int new_num;	//新的block号
	long fat_position, block_position;

	int query_result = 2;		//查询关键字返回值：0无数据；1未找到关键字；2存在关键字
	int target_unit =  0 ;

	int table_num;
	int key;	//数据关键字
	char name[MAN_NAME_LENGTH];	//数据人名

	//printf("=============== 4 修改数据 ===============\n");

	//查找
	content();
	printf(" 请输入数据表编号：");
	scanf("%d", &table_num);
	//输入关键字
	printf(" 请输入关键字：");
	scanf("%d", &key);
	query_result = fat_search_data(table_num, key, &target_unit);

	if (query_result == 2)    //修改
	{
		printf(" 请输入姓名：");
		scanf("%s", &name);

		data_str.key = key;
		strcpy(data_str.name, name);

		//目标关键字的block号
		fat_position = ((target_unit / fat_s.fat_value_amount + 1) + target_unit) * sizeof(int);
		fseek(disk_file[0].file_point, fat_position, 0);
		fread(&unit, sizeof(int), 1, disk_file[0].file_point);	//next_unit：目标关键字的block号

		//修改数据位置，并写入
		block_position = unit * config_inf.block_size * 1024 + sizeof(int);	//越过块号
		fseek(disk_file[0].file_point, block_position, 0);
		fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
		printf(" 修改成功：\n==============\n");

		//读表
		fat_read_table(table_num);
	}
}