#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<fstream>
#include<vector>
#include<iostream>
#include<algorithm>
#include"Slave.h"
#define G_IND "groups_ind.txt"
#define G_FL "groups_fl.txt"
#define M_WASTE "master_waste.txt"

void updateDelM(int);

void overwriteWasteId(int, FILE*, struct Master*);

int insertM(struct Master);

int getM(struct Master*, int, char*);

int updateM(struct Master);

int delM(int , char*);

int calcM(char*);

int utM(char*);

struct Master
{
	int id;
	char name[16];
	int countOfStudents;
	long firstSlaveAddress;
	int slavesCount;
	int allSlaves;
};
struct Indexer
{
	int id;
	int address;
	int exists;
};