//4 �޸�
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct fat_block_structure fat_s;					//b+tree.c
extern struct config_information config_inf;				//config��Ϣ�б�
struct data_structure data_str;								//table���ݱ������ݸ�ʽ
extern int fat_search_data(int table_num, int key, int *target_unit);			//fat_search_data.c
extern void fat_read_table(int table_num);									//fat_read_table.c
extern void content();									//b+tree.c


void fat_alter_data() {
	int unit, next_unit;	//����ֵ=1 ��ʾ�ñ���ֵ�����ݣ���һ����ֵ��
	int new_num;	//�µ�block��
	long fat_position, block_position;

	int query_result = 2;		//��ѯ�ؼ��ַ���ֵ��0�����ݣ�1δ�ҵ��ؼ��֣�2���ڹؼ���
	int target_unit =  0 ;

	int table_num;
	int key;	//���ݹؼ���
	char name[MAN_NAME_LENGTH];	//��������

	//printf("=============== 4 �޸����� ===============\n");

	//����
	content();
	printf(" ���������ݱ��ţ�");
	scanf("%d", &table_num);
	//����ؼ���
	printf(" ������ؼ��֣�");
	scanf("%d", &key);
	query_result = fat_search_data(table_num, key, &target_unit);

	if (query_result == 2)    //�޸�
	{
		printf(" ������������");
		scanf("%s", &name);

		data_str.key = key;
		strcpy(data_str.name, name);

		//Ŀ��ؼ��ֵ�block��
		fat_position = ((target_unit / fat_s.fat_value_amount + 1) + target_unit) * sizeof(int);
		fseek(disk_file[0].file_point, fat_position, 0);
		fread(&unit, sizeof(int), 1, disk_file[0].file_point);	//next_unit��Ŀ��ؼ��ֵ�block��

		//�޸�����λ�ã���д��
		block_position = unit * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
		fseek(disk_file[0].file_point, block_position, 0);
		fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
		printf(" �޸ĳɹ���\n==============\n");

		//����
		fat_read_table(table_num);
	}
}