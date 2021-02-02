#include"Master.h"

using namespace std;

void updateDelM(int id) {
	FILE* waste;
	fopen_s(&waste, M_WASTE, "rb");
	int count = 0;
	fscanf_s(waste, "%d", &count);
	vector<int> ids(count + 1);
	for (int i = 0; i < count; i++) {
		fscanf_s(waste, "%d", &ids[i]);
	}
	ids[count] = id;
	fclose(waste);
	FILE* w;
	fopen_s(&w, M_WASTE, "wb");
	count++;
	fprintf(w, "%d", count);
	for (int i = 0; i < count; i++)
	{
		fprintf(w, "%s%d", " ", ids[i]);
	}
	fclose(w);
}

void overwriteWasteId(int count, FILE* waste, struct Master* new_master) {
	vector<int> ids(count);
	for (int i = 0; i < count; i++)
	{
		fscanf_s(waste, "%d", &ids[i]);
	}
	new_master->id = ids[0];								

	fclose(waste);
	FILE* w;
	fopen_s(&w, M_WASTE, "wb");							
	fprintf(w, "%d", count - 1);			

	for (int i = 1; i < count; i++)
	{
		fprintf(w, "%s%d", " ", ids[i]);
	}										
	fclose(w);									

}

int insertM(struct Master new_master) {
	FILE* index;
	fopen_s(&index, G_IND, "a+b");
	FILE* data;
	fopen_s(&data, G_FL, "a+b");
	FILE* waste;
	fopen_s(&waste, M_WASTE, "rb");
	struct Indexer indexer;
	int count;

	fscanf_s(waste, "%d", &count);

	if (count)
	{
		overwriteWasteId(count, waste, &new_master);

		fclose(index);
		fclose(data);

		fopen_s(&index, G_IND, "r+b");
		fopen_s(&data, G_FL, "r+b");

		fseek(index, (new_master.id - 1) * sizeof(struct Indexer), SEEK_SET);
		fread(&indexer, sizeof(struct Indexer), 1, index);
		fseek(data, indexer.address, SEEK_SET);
	}
	else
	{
		long indexerSize = sizeof(struct Indexer);

		fseek(index, 0, SEEK_END);

		if (ftell(index))
		{
			fseek(index, -indexerSize, SEEK_END);
			fread(&indexer, sizeof(struct Indexer), 1, index);

			new_master.id = indexer.id + 1;
		}
		else
		{
			new_master.id = 1;
		}
	}
	new_master.firstSlaveAddress = -1;
	new_master.slavesCount = 0;
	new_master.allSlaves = 0;

	fwrite(&new_master, sizeof(struct Master), 1, data);

	indexer.id = new_master.id;
	indexer.address = (new_master.id - 1) * sizeof(struct Master);
	indexer.exists = 1;

	cout << "Your master\'s id is " << new_master.id << endl;

	fseek(index, (new_master.id - 1) * sizeof(struct Indexer), SEEK_SET);
	fwrite(&indexer, sizeof(struct Indexer), 1, index);
	fclose(index);
	fclose(data);
	fclose(waste);



	return 1;
}

int getM(struct Master* master, int id, char* err) {
	FILE* index;
	fopen_s(&index, G_IND, "rb");
	FILE* data;
	fopen_s(&data, G_FL, "rb");

	if (index == NULL || data == NULL)
	{
		strcpy_s(err, 18, "database is empty");
		fclose(index);
		fclose(data);
		return 0;
	}

	struct Indexer indexer;

	fseek(index, 0, SEEK_END);

	long size = ftell(index);

	if (size == 0 || id * sizeof(struct Indexer) > size)
	{
		strcpy_s(err, 20, "no such ID in table");
		fclose(index);
		fclose(data);
		return 0;
	}


	fseek(index, (id - 1) * sizeof(struct Indexer), SEEK_SET);	
	fread(&indexer, sizeof(struct Indexer), 1, index);			

	if (!indexer.exists)
	{
		strcpy_s(err, 47, "the record you\'re looking for has been removed");
		fclose(index);
		fclose(data);
		return 0;
	}

	fseek(data, indexer.address, SEEK_SET);				
	fread(master, sizeof(struct Master), 1, data);		
	fclose(index);										
	fclose(data);

	return 1;
}

int updateM(Master master) {
	FILE* index;
	fopen_s(&index, G_IND, "r+b");
	FILE* data;
	fopen_s(&data, G_FL, "r+b");
	
	Indexer indexer;
	int id = master.id;

	fseek(index, 0, SEEK_END);


	fseek(index, (id - 1) * sizeof(Indexer), SEEK_SET);	
	fread(&indexer, sizeof(Indexer), 1, index);			

	fseek(data, indexer.address, SEEK_SET);				
	fwrite(&master, sizeof(Master), 1, data);				
	fclose(index);										
	fclose(data);

	return 1;
}

int delM(int id, char* err) {
	
	
	struct Master master;
	if (getM(&master, id, err)) {
		FILE* index;
		fopen_s(&index, G_IND, "r+b");
		struct Indexer indexer;

		fseek(index, (id - 1) * sizeof(struct Indexer), SEEK_SET);
		fread(&indexer, sizeof(struct Indexer), 1, index);

		indexer.exists = 0;

		fseek(index, (id - 1) * sizeof(struct Indexer), SEEK_SET);

		fwrite(&indexer, sizeof(struct Indexer), 1, index);
		fclose(index);
		updateDelM(id);
		if (master.slavesCount)
		{
			FILE* slavesDb;
			fopen_s(&slavesDb, S_FL, "r+b");
			struct Slave slave;

			fseek(slavesDb, master.firstSlaveAddress, SEEK_SET);

			for (int i = 0; i < master.slavesCount; i++)
			{
				fread(&slave, sizeof(struct Slave), 1, slavesDb);
				fclose(slavesDb);
				delS(master, slave, slave.studentId, err);

				fopen_s(&slavesDb, S_FL, "r+b");
				fseek(slavesDb, slave.nextAddress, SEEK_SET);
			}

			fclose(slavesDb);
		}
		return 1;
	}
	else {
		return 0;
	}
}

int calcM(char* err) {
	FILE* index;
	fopen_s(&index, G_IND, "rb");
	if (index == NULL)
	{
		strcpy_s(err, 18, "database is empty");
		return 0;
	}
	else {
		int masterCount = 0;
		fseek(index, 0, SEEK_END);
		int amount = ftell(index) / sizeof(struct Indexer);

		Master master;
		for (int i = 1; i <= amount; i++)
		{
			if (getM(&master, i, err))
			{
				masterCount++;
			}
		}
		

		cout << "Total masters: " << masterCount << endl;
	}
	fclose(index);
	return 1;
}

int utM(char* err) {
	FILE* index;
	fopen_s(&index, G_IND, "rb");
	if (index == NULL)
	{
		strcpy_s(err, 18, "database is empty");
		return 0;
	}
	else {
		fseek(index, 0, SEEK_END);
		int amount = ftell(index) / sizeof(struct Indexer);

		Master master;
		cout << "-------------------------------------------" << endl;
		for (int i = 1; i <= amount; i++)
		{
			if (getM(&master, i, err))
			{
				cout << "Id: " << master.id << endl;
				cout << "Name: " << master.name << endl;
				cout << "Count of students: " << master.countOfStudents << endl;
				cout << "-------------------------------------------" << endl;
			}
		}
	}
	fclose(index);
	return 1;
}