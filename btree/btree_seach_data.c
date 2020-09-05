//5 ��ѯ
#include "pch.h"


extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct config_information config_inf;	//config��Ϣ�б�//b+tree.c


int btree_search_data(int key, int block_index, Status *find_flag) {
	int unit;
	long position;
	bnode node;

	/*
	if (index_num == 0)	//��ʼ�����飬�ļ�����һ��block
	{
		position = disk_file[0].file_block_amount * sizeof(int);
		fseek(disk_file[0].file_point, position, 0);	//��λ��ʼfatλ��
		fread(&unit, sizeof(int), 1, disk_file[0].file_point);

		if (unit != 0)
		{
			fseek(disk_file[0].file_point, 0 + sizeof(int), 0);		//��λ��ʼ
			fread(&node, sizeof(bnode), 1, disk_file[0].file_point);

			for (int i = 0; i < node.keynum; i++)
			{
				if (key < node.key[i])
				{
					if (node.child_block_num[i] != 0)
					{
						btree_search_data(key, node.child_block_num[i]);
					}
					else
					{
						return 0;
					}
				} 
				else if (key == node.key[i])
				{
					return 0;
				}
			}
		}
	}
	else
	{
		position = (index_num - disk_file[0].file_block_amount) * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
		fseek(disk_file[0].file_point, position, 0);	//��λ��ʼ
		fread(&node, sizeof(bnode), 1, disk_file[0].file_point);

		for (int i = 0; i < node.keynum; i++)
		{
			if (key < node.key[i])
			{
				if (node.child_block_num[i] != 0)
				{
					return btree_search_data(key, node.child_block_num[i]);
				}
				else
				{
					return 0;
				}
			}
			else if (key == node.key[i])
			{
				return index_num;
			}
			else if (key > node.key[i] && i == node.keynum - 1)
			{
				if (node.child_block_num[node.keynum] != 0)
				{
					return btree_search_data(key, node.child_block_num[node.keynum]);
				}
				else
				{
					return 0;
				}
			}
		}
		return 0;
	}
	*/
	
	position = block_index * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
	fseek(disk_file[0].file_point, position, 0);	//��λ��ʼ
	fread(&node, sizeof(bnode), 1, disk_file[0].file_point);

	printf(" ��ǰ��ѯblock_index��%d\n", block_index);

	//��ʼ���ĸ��ڵ㣬��δ�洢����
	if (node.keynum == 0)
	{
		return block_index;	//���ص�ǰ��ţ��鵽����
	}

	//�Ѵ洢����
	printf(" �Ѵ洢����node.keynum = %d\n", node.keynum);
	for (int i = 0; i < node.keynum; i++)
	{
		printf(" node.key[%d] = %d\n", i, node.key[i]);
		if (key < node.key[i])
		{
			if (node.child_block_num[i] != 0)
			{
				return btree_search_data(key, node.child_block_num[i], find_flag);
			}
			else
			{
				*find_flag = false;
				return block_index;
			}
		}
		else if (key == node.key[i])
		{
			printf(" �ؼ��ִ洢��block_index��%d\n", block_index);
			*find_flag = true;
			return block_index;
		}
		else if (key > node.key[i] && i == node.keynum - 1)
		{
			if (node.child_block_num[i] != 0)
			{
				return btree_search_data(key, node.child_block_num[node.keynum], find_flag);
			}
			else
			{
				*find_flag = false;
				return block_index;
			}
		}
	}
	*find_flag = false;
	printf(" �޴洢��\n");
	return 0;
}


/*
void b_index(Ope ope, int key) {
	int prior_num[1] = { 0 };
	switch (ope)
	{
	case query: 
		//��ѯ
		
		
		break;


	case insert: 
		//����
		if (btree_search_data(key, prior_num) == ok)
		{
			btree_insert_data(key, prior_num);
			printf("InsertBTree successfully.\n");
			break;
		}
		else
		{
			printf("�ؼ��ֲ�����.\n");
		}


	case delete: 
		//ɾ��
		if (btree_search_data(key, prior_num) == ok)
		{
			delete_key(key, prior_num);
			printf("DeleteBTree successfully.\n");
			break;
		}
		else
		{
			printf("�ؼ��ֲ�����.\n");
		}
		break;


	case read: 
		break;
		printf("DestroyBTree successfully.\n");
	}
}
*/
