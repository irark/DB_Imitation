#include"Slave.h"

using namespace std;

void updateDelS(int id) {
	FILE* waste;
	fopen_s(&waste, S_WASTE, "rb");
	int count = 0;
	fscanf_s(waste, "%d", &count);
	vector<int> ids(count + 1);
	for (int i = 0; i < count; i++) {
		fscanf_s(waste, "%d", &ids[i]);
	}
	ids[count] = id;
	fclose(waste);
	fopen_s(&waste,S_WASTE, "wb");
	count++;
	fprintf(waste, "%d", count);
	for (int i = 0; i < count; i++)
	{
		fprintf(waste, "%s%d", " ", ids[i]);
	}
	fclose(waste);
}

void overwriteWasteIdS(int count, FILE* waste, struct Slave* new_slave) {
	vector<int> ids(count + 1);
	for (int i = 0; i < count; i++)
	{
		fscanf_s(waste, "%d", &ids[i]);				
	}
	new_slave->selfAddress = ids[0];
	new_slave->nextAddress = ids[0];

	fclose(waste);									
	fopen_s(&waste, S_WASTE, "wb");							
	fprintf(waste, "%d", count - 1);			

	for (int i = 1; i < count; i++)
	{
		fprintf(waste, "%s%d", " ", ids[i]);
	}										
	fclose(waste);									

}

int insertS(struct Master master, struct Slave new_slave, char* err) 
{
	new_slave.exists = 1;		
	FILE* data;
	fopen_s(&data,S_FL, "a+b");
	FILE* waste;
	fopen_s(&waste,S_WASTE, "rb");
	int count;

	fscanf_s(waste, "%d", &count);

	if (count)
	{
		overwriteWasteIdS(count, waste, &new_slave);
		fclose(data);
		fopen_s(&data, S_FL, "r+b");
		fseek(data, new_slave.selfAddress, SEEK_SET);
	}
	else                                                        
	{
		fseek(data, 0, SEEK_END);

		int dbSize = ftell(data);

		new_slave.selfAddress = dbSize;
		new_slave.nextAddress = dbSize;
	}

	fwrite(&new_slave, sizeof(struct Slave), 1, data);					

	if (!master.slavesCount)								    
	{
		master.firstSlaveAddress = new_slave.selfAddress;
	}
	else                                                        
	{
		fclose(data);
		fopen_s(&data, S_FL, "r+b");
		struct Slave previous;

		fseek(data, master.firstSlaveAddress, SEEK_SET);

		for (int i = 0; i < master.slavesCount; i++)		    
		{
			fread(&previous, sizeof(struct Slave), 1, data);
			fseek(data, previous.nextAddress, SEEK_SET);
		}

		previous.nextAddress = new_slave.selfAddress;				
		fwrite(&previous, sizeof(struct Slave), 1, data);
	}

	fclose(data);	
	fclose(waste);

	master.slavesCount++;			
	master.allSlaves++;
	updateM(master);

	return 1;
		
}

int getS(struct Master master, struct Slave* slave, int studentId, char* err) {
	if (!master.slavesCount)									
	{
		strcpy_s(err, 30, "This master has no slaves yet");
		return 0;
	}				
	FILE* data;
	fopen_s(&data, S_FL, "rb");

	fseek(data, master.firstSlaveAddress, SEEK_SET);		
	fread(slave, sizeof(struct Slave), 1, data);

	for (int i = 0; i < master.slavesCount; i++)				
	{
		if (slave->studentId == studentId)						
		{
			fclose(data);
			return 1;
		}

		fseek(data, slave->nextAddress, SEEK_SET);
		fread(slave, sizeof(struct Slave), 1, data);
	}

	strcpy_s(err, 26, "No such slave in database");					
	fclose(data);
	return 0;
}

int updateS(struct Slave slave, int productId)
{
	FILE* data;
	fopen_s(&data, S_FL, "r+b");

	fseek(data, slave.selfAddress, SEEK_SET);
	fwrite(&slave, sizeof(struct Slave), 1, data);
	fclose(data);

	return 1;
}

void delS(struct Master master, struct Slave slave, int productId, char* err)
{
	FILE* data;
	fopen_s(&data, S_FL, "r+b");
	struct Slave previous;

	fseek(data, master.firstSlaveAddress, SEEK_SET);

	do		                                                    
	{															
		fread(&previous, sizeof(struct Slave), 1, data);
		fseek(data, previous.nextAddress, SEEK_SET);
	} while (previous.nextAddress != slave.selfAddress && slave.selfAddress != master.firstSlaveAddress);

	if (slave.selfAddress == master.firstSlaveAddress)		
	{
		if (slave.selfAddress == slave.nextAddress)			
		{
			master.firstSlaveAddress = -1;					
		}
		else                                                
		{
			master.firstSlaveAddress = slave.nextAddress;  
		}
	}
	else                                                    
	{
		if (slave.selfAddress == slave.nextAddress)			
		{
			previous.nextAddress = previous.selfAddress;    
		}
		else                                                
		{
			previous.nextAddress = slave.nextAddress;		
		}

		fseek(data, previous.selfAddress, SEEK_SET);	
		fwrite(&previous, sizeof(struct Slave), 1, data);			
	}
	updateDelS(slave.selfAddress);						

	slave.exists = 0;											

	fseek(data, slave.selfAddress, SEEK_SET);				
	fwrite(&slave, sizeof(struct Slave), 1, data);					
	fclose(data);

	master.slavesCount--;										
	updateM(master);

}

int calcS(char* err) {
	FILE* index;
	fopen_s(&index, G_IND, "rb");
	if (index == NULL)
	{
		strcpy_s(err, 18, "database is empty");
		return 0;
	}
	else {
		int slaveCount = 0;

		fseek(index, 0, SEEK_END);
		int amount = ftell(index) / sizeof(struct Indexer);

		Master master;

		for (int i = 1; i <= amount; i++)
		{
			if (getM(&master, i, err))
			{
				slaveCount += master.slavesCount;
			}
		}
		cout << "Total slaves: " << slaveCount << endl;
	}
	fclose(index);
	return 1;
}

int utS(char* err) {
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
		Slave slave;
		cout << "-------------------------------------------" << endl;
		for (int i = 1; i <= amount; i++)
		{
			if (getM(&master, i, err))
			{
				for (int j = 1; j <= master.allSlaves; j++)
				{
					if (getS(master, &slave, j, err)) {
						cout << "GroupId: " << slave.masterId << endl;
						cout << "StudentId: " << slave.studentId << endl;
						cout << "Name: " << slave.name << endl;
						cout << "Department: " << slave.department << endl;
						cout << "-------------------------------------------" << endl;
					}
				}
			}
		}
	}
	fclose(index);
	return 1;
}