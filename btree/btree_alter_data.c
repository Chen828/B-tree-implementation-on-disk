//4 �޸�
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct config_information config_inf;				//config��Ϣ�б�
extern struct save_content s_c[content_num];					//Ŀ¼��
struct data_structure data_str;								//table���ݱ������ݸ�ʽ

extern int btree_search_data(int key, int block_index, Status *find_flag);			//btree_search_data.c
extern void content();

void btree_alter_data(int table_num, int key) {
	int index_block, data_block;
	bnode node;
	long position;

	Status flag = false, *find_flag = &flag;

	char name[MAN_NAME_LENGTH];	//��������
	
	int i;
	//printf("=============== 4 �޸����� ===============\n");
	//����
	index_block = btree_search_data(key, s_c[table_num].index_block, find_flag);

	if (flag == true)
	{
		printf(" ������������");
		scanf("%s", &name);

		data_str.key = key;
		strcpy(data_str.name, name);

		//��ȡ����λ��
		position = index_block * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
		fseek(disk_file[0].file_point, position, 0);
		fread(&node, sizeof(node), 1, disk_file[0].file_point);

		for (i = 0; i < node.keynum; i++)
		{
			if (node.key[i] == key)
			{
				data_block = node.key_block[i];
				break;
			}
		}
		
		//���ݵ�λ�ã���д��
		position = data_block * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
		fseek(disk_file[0].file_point, position, 0);
		fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
		printf("д��\n");
		fseek(disk_file[0].file_point, position, 0);
		fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
		printf("�ض�\n");
		printf("data_str.key = %d, data_str.name = %s\n", data_str.key, data_str.name);

		printf(" �޸ĳɹ���\n==============\n");
	}
	else
	{
		printf(" �ؼ��ֲ����ڣ�");
	}
}