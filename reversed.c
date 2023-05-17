#include <time.h>
#include <stdio.h>
#include <time.h>

int main(void)

{
  int randInt1;
  time_t currentTime;
  long in_FS_OFFSET;
  unsigned int timeToInt;
  signed char randInt2;
  long i;
  FILE *pInputFile;
  size_t fileSize;
  unsigned char *pMemory;
  FILE *pOutputFile;
  long checkSum;
  
  checkSum = *(long *)(in_FS_OFFSET + 0x28);
  pInputFile = fopen("flag","rb");
    // Move cursor to EOF
  fseek(pInputFile,0,2);
    // Get position of EOF (get the file size)
  fileSize = ftell(pInputFile);
    // Move Cursor to the begining of the file
  fseek(pInputFile,0,0);
  pMemory = (unsigned char *)malloc(fileSize);
  fread(pMemory,fileSize,1,pInputFile);
  fclose(pInputFile);
  currentTime = time((time_t *)0x0);
  timeToInt = (unsigned int)currentTime; // = 1658229288
  srand(timeToInt);
  for (i = 0; i < (long)fileSize; i = i + 1) {
    randInt1 = rand();
    // XOR the char in the `i` position with randInt1
    pMemory[i] = pMemory[i] ^ (unsigned char)randInt1;
    randInt2 = (signed char)rand();
    // 0 < randInt2 < 8
    randInt2 = randInt2 & 7;
    // SHL memory[i], randInt2 OR SHR memory[i], randInt2
    pMemory[i] = pMemory[i] << randInt2 | pMemory[i] >> 8 - randInt2;
  }
  pOutputFile = fopen("flag.enc","wb");
  fwrite(&timeToInt,1,4,pOutputFile);
  fwrite(pMemory,1,fileSize,pOutputFile);
  fclose(pOutputFile);
  if (checkSum != *(long *)(in_FS_OFFSET + 0x28)) {
    // WARNING: Subroutine does not return
    __stack_chk_fail();
  }
  return 0;
}
