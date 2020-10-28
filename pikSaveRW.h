#ifndef PIKSAVRW_H
#define PIKSAVRW_H

#include <fstream>

struct underGroundPik {
	int pikId;
	float x, y, z;
};

long long charByteToLLBE(char* arr, int offset = 0);
long long charByteToLLLE(char* arr, int offset = 0);
int charByteToIntBE(char* arr, int offset = 0);
int charByteToIntLE(char* arr, int offset = 0);
unsigned short charByteToUShortBE(char* arr, int offset = 0);
unsigned short charByteToUShortLE(char* arr, int offset = 0);
void intToCharByteBE(int num, char* arr);
void intToCharByteLE(int num, char* arr);
void shortToCharByteBE(int num, char* arr);
void shortToCharByteLE(int num, char* arr);

void getSaveOffsets(std::fstream& saveFile, int* saves);

void writeSaveToFile(std::fstream& saveFile, int saveOffset, std::string newFilePath);
void writeFileToSave(std::fstream& saveFile, int saveOffset, std::string filePath);

void readCharArr(std::fstream& saveFile, int location, int saveOffset, int size, char* arr);
int readInt(std::fstream& saveFile, int location, int saveOffset, int size);
short readShort(std::fstream& saveFile, int location, int saveOffset, int size);
int readByte(std::fstream& saveFile, int location, int saveOfset, int size);
float readFloat(std::fstream& saveFile, int location, int saveOffset, int size);
void writeInt(std::fstream& saveFile, int location, int saveOffset, int value);
void writeShort(std::fstream& saveFile, int location, int saveOffset, short value);
void writeByte(std::fstream& saveFile, int location, int saveOffset, char value);
void writeFloat(std::fstream& saveFile, int location, int saveOffset, float val);

void readSkyPiks(std::fstream& saveFile, int saveOffset, int* tempArr);
void readOnionPiks(std::fstream& saveFile, int saveOffset, int* tempArr);
int readDispDayCount(std::fstream& saveFile, int saveOffset);
int readRealDayCount(std::fstream& saveFile, int saveOffset);
int readDispPokoCount(std::fstream& saveFile, int saveOffset);
int readTreasureCount(std::fstream& saveFile, int saveOffset);
int readRealPokoCount(std::fstream& saveFile, int saveOffset);
int readPlayTime(std::fstream& saveFile, int saveOffset);
int readValleyOfReposeProgress(std::fstream& saveFile, int saveOffset);
int readAwakeningWoodProgress(std::fstream& saveFile, int saveOffset);
int readPerplexingPoolProgress(std::fstream& saveFile, int saveOffset);
int readWistfulWildProgress(std::fstream& saveFile, int saveOffset);
int readValleyOfReposeTreasure(std::fstream& saveFile, int saveOffset);
int readAwakeningWoodTreasure(std::fstream& saveFile, int saveOffset);
int readPerplexingPoolTreasure(std::fstream& saveFile, int saveOffset);
int readWistfulWildTreasure(std::fstream& saveFile, int saveOffset);
int readUnlockedPikmin(std::fstream& saveFile, int saveOffset);
int readAwakeOnions(std::fstream& saveFile, int saveOffset);
int readUnlockedPellet(std::fstream& saveFile, int saveOffset);
int readMapState(std::fstream& saveFile, int saveOffset);
int readAreaID(std::fstream& saveFile, int saveOffset);
int readCaveID(std::fstream& saveFile, int saveOffset);
int readAreaLetter(std::fstream& saveFile, int saveOffset);
int readSublevelFloor(std::fstream& saveFile, int saveOffset);
underGroundPik readUnderGroundPik(std::fstream& saveFile, int saveOffset, int location);
int readNew(std::fstream& saveFile, int saveOffset);
int readSpicy(std::fstream& saveFile, int saveOffset);
int readBitter(std::fstream& saveFile, int saveOffset);

void writeSkyPiks(std::fstream& saveFile, int saveOffset, int pikminType, int count);
void writeOnionPiks(std::fstream& saveFile, int saveOffset, int pikminType, int count);
void writeDispDayCount(std::fstream& saveFile, int saveOffset, int day);
void writeRealDayCount(std::fstream& saveFile, int saveOffset, int day);
void writeDispPokoCount(std::fstream& saveFile, int saveOffset, int pokos);
void writeTreasureCount(std::fstream& saveFile, int saveOffset, int treasure);
void writeRealPokoCount(std::fstream& saveFile, int saveOffset , int pokos);
void writePlayTime(std::fstream& saveFile, int saveOffset, int mins);
void writeValleyOfReposeProgress(std::fstream& saveFile, int saveOffset, int val);
void writeAwakeningWoodProgress(std::fstream& saveFile, int saveOffset, int val);
void writePerplexingPoolProgress(std::fstream& saveFile, int saveOffset, int val);
void writeWistfulWildProgress(std::fstream& saveFile, int saveOffset, int val);
void writeValleyOfReposeTreasure(std::fstream& saveFile, int saveOffset, int val);
void writeAwakeningWoodTreasure(std::fstream& saveFile, int saveOffset, int val);
void writePerplexingPoolTreasure(std::fstream& saveFile, int saveOffset, int val);
void writeWistfulWildTreasure(std::fstream& saveFile, int saveOffset, int val);
void writeUnlockedPikmin(std::fstream& saveFile, int saveOffset, int val);
void writeAwakeOnions(std::fstream& saveFile, int saveOffset, int val);
void writeUnlockedPellet(std::fstream& saveFile, int saveOffset, int val);
void writeMapState(std::fstream& saveFile, int saveOffset, int val);
void writeAreaID(std::fstream& saveFile, int saveOffset, int val);
void writeCaveID(std::fstream& saveFile, int saveOffset, int val);
void writeAreaLetter(std::fstream& saveFile, int saveOffset, int val);
void writeSublevelFloor(std::fstream& saveFile, int saveOffset, int val);
void writeUnderGroundPik(std::fstream& saveFile, int saveOffset, int location, underGroundPik val);
void writeNew(std::fstream& saveFile, int saveOffset, int val);

int calculateCheckSum(std::fstream& saveFile, int saveOffset);

#endif // !PIKSAVRW_H
