#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>
#include"Master.h"
#define S_FL "students_fl.txt"
#define S_WASTE "slave_waste.txt"
void updateDelS(int);

void overwriteWasteIdS(int, FILE*, struct Slave*);

int insertS(struct Master, struct Slave, char*);

int getS(struct Master, struct Slave*, int, char*);

int updateS(struct Slave, int);

void delS(struct Master, struct Slave, int, char*);

int calcS(char*);

int utS(char*);

struct Slave
{
	int masterId;
	int studentId;
	char name[20];
	char department[20];
	int exists;
	long selfAddress;
	long nextAddress;
};