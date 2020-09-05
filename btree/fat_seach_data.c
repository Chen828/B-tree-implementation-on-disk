//5 ��ѯ�ؼ���
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct fat_block_structure fat_s;	//b+tree.c
extern struct config_information config_inf;	//config��Ϣ�б�
extern struct save_content s_c[content_num];					//Ŀ¼��
struct data_structure data_str;				//table���ݱ������ݸ�ʽ


int fat_search_data(int table_num, int key, int *target_unit) {		//�ؼ��֣���¼С��Ŀ��ؼ��ֵĿ�ţ�С��ȡ���
	int content_block, unit, next_unit;	//����ֵ=1 ��ʾ�ñ���ֵ�����ݣ���һ����ֵ��
	long fat_position, block_position;
	//int first_unit = 0;	//��ʼ���ݱ���
	
	//�洢�ļ��ı���ֵ:0�����ݣ�1����ţ�δ�ҵ��ؼ��֣���¼С�ڹؼ����е����ؼ���λ�ڵĿ�ţ���2���ڹؼ���
	int read_over_flag = 0;	

	//printf("=============== 5 ��ѯ�ؼ��� ===============\n");

	//��ʼλ�ú�
	if (s_c[table_num].data_block == 0)
	{
		printf("���ݱ����ڣ�\n");
		return 0;
	}
	else
	{
		fat_position = ((s_c[table_num].data_block / fat_s.fat_value_amount + 1) + s_c[table_num].data_block) * sizeof(int);
		fseek(disk_file[0].file_point, fat_position, 0);	//��λ�ļ���ʼ

		fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
		unit = s_c[table_num].data_block;	//�ӵ�һ��block��ʼ��
		while (next_unit != 0 && !read_over_flag)	//�ǿ���block
		{
			//���㵱������block��λ�ã���������
			block_position = unit * config_inf.block_size * 1024;
			fseek(disk_file[0].file_point, block_position + sizeof(int), 0);

			fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

			if (data_str.key < key)
			{
				//��¼С�ڹؼ����е����ؼ���λ�ڵĿ��
				*target_unit = unit;
			}
			else if (data_str.key == key)
			{
				//�ؼ��ִ���
				printf(" block_num = %d ; fat_position = %d ; block_position = %dB \n", unit, fat_position, block_position);
				//printf(" next_block_num = %d\n", next_unit);
				read_over_flag = 2;
			}

			//��ȡ��һfat����ֵ
			if (!read_over_flag)
			{
				if (next_unit != 1)
				{
					unit = next_unit;	//�����ţ��Զ���һ����ֵ

					//����next_unit��fat�е�λ��
					fat_position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
					//next_unit / fat_s.fat_value_amount + 1ռ�˼���block
					//��λ
					fseek(disk_file[0].file_point, fat_position, 0);
					//��ȡnext_unit��fat�еı���ֵ
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
	//fat��ѭ����
	for (int i = 0; i < fat_s.fat_block_amount && !read_over_flag; i++)
	{
		fseek(disk_file[0].file_point, sizeof(int), 1);	//����ÿ��Ŀ��

		//��λÿ��block�е���ʼ���ݱ���
		//0�����Ļ�����fat��Ӧ�ĵ�Ԫ��
		if (!i)
		{
			fseek(disk_file[0].file_point, fat_s.fat_block_amount * sizeof(int), 1);
			first_unit = fat_s.fat_block_amount;	//fatռ��block��������ֵ = ��һ�������ݵı���ֵ��ǩ��Ӧ�Ŀ�ŵ���ֵ��
		}

		//����ֵѭ����
		unit = first_unit;
		for (int j = first_unit; j < fat_s.fat_value_amount && !read_over_flag; j++)
		{
			fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);

			while (next_unit != 0 && !read_over_flag)	//�ǿ���block
			{
				//���㵱������block��λ�ã���������
				block_position = unit * config_inf.block_size * 1024;
				fseek(disk_file[0].file_point, block_position + sizeof(int), 0);

				fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

				if (data_str.key < key)
				{
					//��¼С�ڹؼ����е����ؼ���λ�ڵĿ��
					*target_unit = unit;
				}
				else if (data_str.key == key)
				{
					//�ؼ��ִ���
					printf(" block_num = %d ; fat_position = %d ; block_position = %dB \n", unit, fat_position, block_position);
					//printf(" next_block_num = %d\n", next_unit);
					read_over_flag = 2;
				}

				//��ȡ��һfat����ֵ
				if (!read_over_flag)
				{
					if (next_unit != 1)
					{
						unit = next_unit;	//�����ţ��Զ���һ����ֵ

						//����next_unit��fat�е�λ��
						fat_position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
						//next_unit / fat_s.fat_value_amount + 1ռ�˼���block
						//��λ
						fseek(disk_file[0].file_point, fat_position, 0);
						//��ȡnext_unit��fat�еı���ֵ
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

	//��ѯ���
	if (read_over_flag == 0)
	{
		printf("�����ݴ洢��\n");

		return 0;
	}
	else if (read_over_flag == 1)
	{
		printf(" �ؼ��ֲ����ڣ�\n");

		return 1;
	}
	else
	{
		//read_over_flag == 2
		printf(" data_str.key = %3d, data_str.name = %s\n", data_str.key, data_str.name);

		return 2;
	}
}