#include "pikSaveRW.h"
#include "offsets.h"

#include <iostream>

long long charByteToLLBE(char* arr, int offset)
{
	return ((arr[offset + 0] & 0xFFll) << 7 * 8) |
		((arr[offset + 1] & 0xFFll) << 6 * 8) |
		((arr[offset + 2] & 0xFFll) << 5 * 8) |
		((arr[offset + 3] & 0xFFll) << 4 * 8) |
		((arr[offset + 4] & 0xFFll) << 3 * 8) |
		((arr[offset + 5] & 0xFFll) << 2 * 8) |
		((arr[offset + 6] & 0xFFll) << 1 * 8) |
		((arr[offset + 7] & 0xFFll) << 0);
}

long long charByteToLLLE(char* arr, int offset)
{
	return ((arr[offset + 7] & 0xFFll) << 7 * 8) |
		((arr[offset + 6] & 0xFFll) << 6 * 8) |
		((arr[offset + 5] & 0xFFll) << 5 * 8) |
		((arr[offset + 4] & 0xFFll) << 4 * 8) |
		((arr[offset + 3] & 0xFFll) << 3 * 8) |
		((arr[offset + 2] & 0xFFll) << 2 * 8) |
		((arr[offset + 1] & 0xFFll) << 1 * 8) |
		((arr[offset + 0] & 0xFFll) << 0);
}

int charByteToIntBE(char* arr, int offset)
{
	return ((arr[offset + 0] & 0xFF) << 3 * 8) | ((arr[offset + 1] & 0xFF) << 2 * 8) | ((arr[offset + 2] & 0xFF) << 1 * 8) | ((arr[offset + 3] & 0xFF) << 0);
}

int charByteToIntLE(char* arr, int offset)
{
	return (arr[offset + 3] << 3 * 8) | (arr[offset + 2] << 2 * 8) | (arr[offset + 1] << 1 * 8) | (arr[offset + 0] << 0);
}

unsigned short charByteToUShortBE(char* arr, int offset)
{
	return ((arr[offset + 0] & 0xFF) << 1 * 8) | ((arr[offset + 1] & 0xFF) << 0);
}

unsigned short charByteToUShortLE(char* arr, int offset)
{
	return (arr[offset + 1] << 1 * 8) | (arr[offset + 0] << 0);
}

void intToCharByteBE(int num, char* arr)
{
	arr[0] = ((num >> (3 * 8)) & 0xFF);
	arr[1] = ((num >> (2 * 8)) & 0xFF);
	arr[2] = ((num >> (1 * 8)) & 0xFF);
	arr[3] = ((num >> (0 * 8)) & 0xFF);
}

void intToCharByteLE(int num, char* arr)
{
	arr[0] = ((num >> (0 * 8)) & 0xFF);
	arr[1] = ((num >> (1 * 8)) & 0xFF);
	arr[2] = ((num >> (2 * 8)) & 0xFF);
	arr[3] = ((num >> (3 * 8)) & 0xFF);
}

void shortToCharByteBE(int num, char* arr)
{
	arr[0] = ((num >> (1 * 8)) & 0xFF);
	arr[1] = ((num >> (0 * 8)) & 0xFF);
}

void shortToCharByteLE(int num, char* arr)
{
	arr[0] = ((num >> (0 * 8)) & 0xFF);
	arr[1] = ((num >> (1 * 8)) & 0xFF);
}

void getSaveOffsets(std::fstream& saveFile, int* saves)
{
	//int currentSave = 0;
	saveFile.seekg(0, std::ios::end);
	int saveFileEnd = saveFile.tellg();
	int seekPos = 0;
	char* saveHeaderbuff = new char[8];
//	const char* saveHeaderString = "PlVa0003";
	long long saveHeader = 0;

	for (int i = 0; i < saveFileEnd; i += 0x10) {
		saveFile.seekg(i);
		saveFile.read(saveHeaderbuff, 9);
		//std::cout << saveHeaderbuff << "\n";

		if ((int)saveHeaderbuff[0] == 0x50 &&
			(int)saveHeaderbuff[1] == 0x6C &&
			(int)saveHeaderbuff[2] == 0x56 &&
			(int)saveHeaderbuff[3] == 0x61 &&
			(int)saveHeaderbuff[4] == 0x30 &&
			(int)saveHeaderbuff[5] == 0x30 &&
			(int)saveHeaderbuff[6] == 0x30 &&
			(int)saveHeaderbuff[7] == 0x33) {
			//std::cout << "Match\n";
			int foundSave = readByte(saveFile, i + 8, 0, 1);
			//std::cout << foundSave << "\n";
			saves[foundSave] = i;
			i += PIK_SAV_SIZE - 0x10;
		}
	}

	//delete[] saveHeaderbuff;
}

void writeSaveToFile(std::fstream& saveFile, int saveOffset, std::string newFilePath)
{
	char* buff = new char[PIK_SAV_SIZE];

	saveFile.seekg(saveOffset);
	saveFile.read(buff, PIK_SAV_SIZE);

	std::fstream outFile;
	outFile.open(newFilePath.c_str(), std::ios::out | std::ios::binary);

	outFile.write(buff, PIK_SAV_SIZE);

	delete[] buff;
}

void writeFileToSave(std::fstream& saveFile, int saveOffset, std::string filePath)
{
	char* buff = new char[PIK_SAV_SIZE];

	std::fstream inFile;
	inFile.open(filePath.c_str(), std::ios::in | std::ios::binary);

	if (!inFile.is_open()) { std::cerr << "Couldn't read file"; return; }

	inFile.read(buff, PIK_SAV_SIZE);

	/*
	for (int i = 0; i < PIK_SAV_SIZE; i++) {
		std::cout << std::hex << ((int)buff[i] & 0xFF) << (i % 16 == 0 && i != 0 ? "\n" : " ");
	}*/

	saveFile.seekp(saveOffset);
	saveFile.write(buff, PIK_SAV_SIZE);

	delete[] buff;
}

void readCharArr(std::fstream& saveFile, int location, int saveOffset, int size, char* arr)
{
	saveFile.seekg(location + saveOffset);
	saveFile.read(arr, size);
}

int readInt(std::fstream& saveFile, int location, int saveOffset, int size)
{
	char* buff = new char[size];

	saveFile.seekg(location + saveOffset);
	saveFile.read(buff, size);

	int result = charByteToIntBE(buff);

	delete[] buff;

	return result;
}

short readShort(std::fstream& saveFile, int location, int saveOffset, int size)
{
	char* buff = new char[size];

	saveFile.seekg(location + saveOffset);
	saveFile.read(buff, size);

	short result = charByteToUShortBE(buff);

	delete[] buff;

	return result;
}

int readByte(std::fstream& saveFile, int location, int saveOffset, int size)
{
	char* buff = new char[size];

	saveFile.seekg(location + saveOffset);
	saveFile.read(buff, size);

	//std::cout << std::hex << buff[0] << "|\n";

	int result = buff[0];

	delete[] buff;

	return result;
}

float readFloat(std::fstream& saveFile, int location, int saveOffset, int size)
{
	int temp = readInt(saveFile, location, saveOffset, size);
	return *((float*)(&temp));
}

void writeInt(std::fstream& saveFile, int location, int saveOffset, int value)
{
	saveFile.seekp(location + saveOffset);
	char* val = new char[4];

	//std::cout << "Write Location:\t" << std::hex << PIK_SKY + pikminType << "\t" << PIK_SKY + SIZE_PIK_SKY << "\n";

	intToCharByteBE(value, val);
	saveFile.write(val, 4);

	delete[] val;
}

void writeShort(std::fstream& saveFile, int location, int saveOffset, short value)
{
	saveFile.seekp(location + saveOffset);
	char* val = new char[4];

	//std::cout << "Write Location:\t" << std::hex << PIK_SKY + pikminType << "\t" << PIK_SKY + SIZE_PIK_SKY << "\n";

	shortToCharByteBE(value, val);
	saveFile.write(val, 4);

	delete[] val;
}

void writeByte(std::fstream& saveFile, int location, int saveOffset, char value)
{
	saveFile.seekp(location + saveOffset);

	saveFile.write(&value, 1);
}

void writeFloat(std::fstream& saveFile, int location, int saveOffset, float val)
{
	writeInt(saveFile, location, saveOffset, *((int*)(&val)));
}

void readSkyPiks(std::fstream& saveFile, int saveOffset, int* tempArr)
{
	char* skyPiks = new char[SIZE_PIK_SKY];

	saveFile.seekg(PIK_SKY + saveOffset);
	saveFile.read(skyPiks, SIZE_PIK_SKY);

	for (int i = 0; i < SIZE_PIK_SKY; i += 4) {
		tempArr[i / 4] = charByteToIntBE(skyPiks, i);
	}

	delete[] skyPiks;
}

void readOnionPiks(std::fstream& saveFile, int saveOffset, int* tempArr)
{
	char* onionPiks = new char[SIZE_PIK_ONION];

	saveFile.seekg(PIK_ONION + saveOffset);
	saveFile.read(onionPiks, SIZE_PIK_ONION);

	for (int i = 0; i < SIZE_PIK_ONION; i += 4) {
		tempArr[i / 4] = charByteToIntBE(onionPiks, i);
	}

	delete[] onionPiks;
}

int readDispDayCount(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, DAY_COUNT_DISP, saveOffset, SIZE_DAY_COUNT_DISP);
}

int readRealDayCount(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, DAY_COUNT_REAL, saveOffset, SIZE_DAY_COUNT_REAL);
}

int readDispPokoCount(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, POKO_COUNT_DISP, saveOffset, SIZE_POKO_COUNT_DISP);
}

int readTreasureCount(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, TRESURE_COUNT, saveOffset, SIZE_TRESURE_COUNT);
}

int readRealPokoCount(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, POKO_COUNT_REAL, saveOffset, SIZE_POKO_COUNT_REAL);
}

int readPlayTime(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, TIME_PLAYED, saveOffset, SIZE_TIME_PLAYED);
}

int readValleyOfReposeProgress(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, VALLYOFREPOSE, saveOffset, SIZE_VALLYOFREPOSE);
}

int readAwakeningWoodProgress(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, AWAKENINGWOOD, saveOffset, SIZE_AWAKENINGWOOD);
}

int readPerplexingPoolProgress(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, PERPLEXINGPOOL, saveOffset, SIZE_PERPLEXINGPOOL);
}

int readWistfulWildProgress(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, WISTFULWILD, saveOffset, SIZE_WISTFULWILD);
}

int readValleyOfReposeTreasure(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, VALLYOFREPOSE_ABOVETREASURE, saveOffset, SIZE_VALLYOFREPOSE_ABOVETREASURE);
}

int readAwakeningWoodTreasure(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, AWAKENINGWOOD_ABOVETREASURE, saveOffset, SIZE_AWAKENINGWOOD_ABOVETREASURE);
}

int readPerplexingPoolTreasure(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, PERPLEXINGPOOL_ABOVETREASURE, saveOffset, SIZE_PERPLEXINGPOOL_ABOVETREASURE);
}

int readWistfulWildTreasure(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, WISTFULWILD_ABOVETREASURE, saveOffset, SIZE_WISTFULWILD_ABOVETREASURE);
}

int readUnlockedPikmin(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, PIK_UNLOCKED, saveOffset, SIZE_PIK_UNLOCKED);
}

int readAwakeOnions(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, ONION_AWAKE, saveOffset, SIZE_ONION_AWAKE);
}

int readUnlockedPellet(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, PELLET_UNLOCKED, saveOffset, SIZE_PELLET_UNLOCKED);
}

int readMapState(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, MAP_STATE, saveOffset, SIZE_MAP_STATE);
}

int readAreaID(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, AREA_ID, saveOffset, SIZE_AREA_ID);
}

int readCaveID(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, CAVE_ID, saveOffset, SIZE_CAVE_ID);
}

int readAreaLetter(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, AREA_LETTER, saveOffset, SIZE_AREA_LETTER);
}

int readSublevelFloor(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, SUBLEVEL_FLOOR, saveOffset, SIZE_SUBLEVEL_FLOOR);
}

underGroundPik readUnderGroundPik(std::fstream& saveFile, int saveOffset, int location)
{
	underGroundPik temp;
	temp.pikId = readByte(saveFile, location, saveOffset, 1);
	temp.x = readFloat(saveFile, location+1, saveOffset, 4);
	temp.y = readFloat(saveFile, location + 5, saveOffset, 4);
	temp.z = readFloat(saveFile, location + 9, saveOffset, 4);

	return temp;
}

int readNew(std::fstream& saveFile, int saveOffset)
{
	return readByte(saveFile, SAVE_NEW, saveOffset, SIZE_SAVE_NEW);
}

int readSpicy(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, SPICEY_SPRAY_COUNT, saveOffset, SIZE_SPICEY_SPRAY_COUNT);
}

int readBitter(std::fstream& saveFile, int saveOffset)
{
	return readInt(saveFile, BITTER_SPRAY_COUNT, saveOffset, SIZE_BITTER_SPRAY_COUNT);
}

void writeSkyPiks(std::fstream& saveFile, int saveOffset, int pikminType, int count)
{
	//should probably check that pikmin type isn't greater than 20
	saveFile.seekp(PIK_SKY + saveOffset + pikminType);
	char* val = new char[4];

	//std::cout << "Write Location:\t" << std::hex << PIK_SKY + pikminType << "\t" << PIK_SKY + SIZE_PIK_SKY << "\n";

	intToCharByteBE(count, val);
	saveFile.write(val, 4);

	delete[] val;
}

void writeOnionPiks(std::fstream& saveFile, int saveOffset, int pikminType, int count)
{
	//should probably check that pikmin type isn't greater than 20
	saveFile.seekp(PIK_ONION + saveOffset + pikminType);
	char* val = new char[4];

	//std::cout << "Write Location:\t" << std::hex << PIK_SKY + pikminType << "\t" << PIK_SKY + SIZE_PIK_SKY << "\n";

	intToCharByteBE(count, val);
	saveFile.write(val, 4);

	delete[] val;
}

void writeDispDayCount(std::fstream& saveFile, int saveOffset, int day)
{
	writeInt(saveFile, DAY_COUNT_DISP, saveOffset, day);
}

void writeRealDayCount(std::fstream& saveFile, int saveOffset, int day)
{
	writeInt(saveFile, DAY_COUNT_REAL, saveOffset, day);
}

void writeDispPokoCount(std::fstream& saveFile, int saveOffset, int pokos)
{
	writeInt(saveFile, POKO_COUNT_DISP, saveOffset, pokos);
}

void writeTreasureCount(std::fstream& saveFile, int saveOffset, int treasure)
{
	writeInt(saveFile, TRESURE_COUNT, saveOffset, treasure);
}

void writeRealPokoCount(std::fstream& saveFile, int saveOffset, int pokos)
{
	writeInt(saveFile, POKO_COUNT_REAL, saveOffset, pokos);
}

void writePlayTime(std::fstream& saveFile, int saveOffset, int mins)
{
	writeInt(saveFile, TIME_PLAYED, saveOffset, mins);
}

void writeValleyOfReposeProgress(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, VALLYOFREPOSE, saveOffset, val);
}

void writeAwakeningWoodProgress(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, AWAKENINGWOOD, saveOffset, val);
}

void writePerplexingPoolProgress(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, PERPLEXINGPOOL, saveOffset, val);
}

void writeWistfulWildProgress(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, WISTFULWILD, saveOffset, val);
}

void writeValleyOfReposeTreasure(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, VALLYOFREPOSE_ABOVETREASURE, saveOffset, val);
}

void writeAwakeningWoodTreasure(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, AWAKENINGWOOD_ABOVETREASURE, saveOffset, val);
}

void writePerplexingPoolTreasure(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, PERPLEXINGPOOL_ABOVETREASURE, saveOffset, val);
}

void writeWistfulWildTreasure(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, WISTFULWILD_ABOVETREASURE, saveOffset, val);
}

void writeUnlockedPikmin(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, PIK_UNLOCKED, saveOffset, val);
}

void writeAwakeOnions(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, ONION_AWAKE, saveOffset, val);
}

void writeUnlockedPellet(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, PELLET_UNLOCKED, saveOffset, val);
}

void writeMapState(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, MAP_STATE, saveOffset, val);
}

void writeAreaID(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, AREA_ID, saveOffset, val);
}

void writeCaveID(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, CAVE_ID, saveOffset, val);
}

void writeAreaLetter(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, AREA_LETTER, saveOffset, val);
}

void writeSublevelFloor(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, SUBLEVEL_FLOOR, saveOffset, val);
}

void writeUnderGroundPik(std::fstream& saveFile, int saveOffset, int location, underGroundPik val)
{
	writeByte(saveFile, location, saveOffset, val.pikId & 0xFF);
	writeFloat(saveFile, location, saveOffset, val.x);
	writeFloat(saveFile, location, saveOffset, val.y);
	writeFloat(saveFile, location, saveOffset, val.z);
}

void writeNew(std::fstream& saveFile, int saveOffset, int val)
{
	writeByte(saveFile, SAVE_NEW, saveOffset, val);
}

int calculateCheckSum(std::fstream& saveFile, int saveOffset)
{
	int seekPos = saveOffset;

	unsigned short c1 = 0, c2 = 0;

	char* buff = new char[2];

	while (seekPos != PIK_SAV_SIZE + saveOffset - 4) {
		saveFile.seekg(seekPos);
		saveFile.read(buff, 2);

		unsigned short tempShort = charByteToUShortBE(buff);

		c1 = (c1 + tempShort) & 0xFFFF;
		c2 = (c2 + (tempShort ^ 0xffff)) & 0xFFFF;

		seekPos += 2;
	}

	delete[] buff;

	if (c1 == 0xffff) { c1 = 0; }
	if (c2 == 0xffff) { c2 = 0; }

	//std::cout << std::hex << std::setiosflags(std::ios::showbase) << c1 << " " << c2 << "\n";
	//std::cout << std::hex << std::setiosflags(std::ios::showbase) << ((c1 << 16) | c2) << "\n";

	return (c1 << 16) | c2;
}
