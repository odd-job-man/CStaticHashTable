#include <iostream>
#include <cstring>
#include <Windows.h>
#include "LinkedList.h"
#include "HashTable.h"

constexpr DWORD dwMaxBucketNum = 7000;

DWORD keyFunc(const void* pKey)
{
    DWORD ret;
    DWORD key;
    key = (DWORD)pKey;
    ret = key % dwMaxBucketNum;
    return ret;
}

void keyAssignFunc(void** ppKeyInHash, const void* pKey)
{
    *(DWORD*)ppKeyInHash = (DWORD)pKey;
}

BOOL keyCompareFunc(const void* pKeyInHash, const void* pKey)
{
    return pKeyInHash == pKey;
}

struct Data
{
    DWORD id_;
    DWORD a;
    DWORD b;
    STATIC_HASH_METADATA shm;
    Data(int id)
    {
        id_ = id;
    }
    ~Data();
};

Data* dataArr[dwMaxBucketNum * 3];
Data* dataArr2[2];

int main()
{
    CStaticHashTable hash;
    Data* pData;
    hash.Initialize(dwMaxBucketNum, sizeof(int), keyFunc, keyAssignFunc, keyCompareFunc, offsetof(Data, shm));
    for (int i = 0; i < dwMaxBucketNum * 3; ++i)
    {
        pData = new Data(i);
        hash.Insert((const void*)pData, (const void*)(pData->id_), sizeof(DWORD));
    }

    BOOL bRet = FALSE;
    hash.GetAllItems((void**)&dataArr, dwMaxBucketNum * 3, &bRet);
    if (bRet)
    {
        __debugbreak();
    }

    for (DWORD i = 0; i < 3; ++i)
    {
        hash.Find((void**)&pData, 1, (const void*)(1 + 7000 * i), sizeof(DWORD));
        hash.Delete((const void*)pData);
    }

    for (DWORD i = 0; i < 3; ++i)
    {
        hash.Find((void**)&pData, 3, (const void*)(3 + 7000 * i), sizeof(DWORD));
        hash.Delete((const void*)pData);
    }

    pData = (Data*)hash.GetFirst();
    while (pData)
    {
        const void* pKey = pData->shm.pKey;
        printf("hashVal : %d, key : %d, id : %d\n", keyFunc(pKey), (DWORD)(pKey), pData->id_);
        pData = (Data*)hash.GetNext(pData);
    }


 //   hash.Find((void**)&pData, 1, (const void*)7001, sizeof(DWORD));
	//printf("hashVal : %d, key : %d, id : %d\n", keyFunc(pData->shm.pKey), (DWORD)(pData->shm.pKey), pData->id_);

    //for (int i = 0; i < dwMaxBucketNum * 2; ++i)
    //{
    //    printf("hashVal : %d, key : %d, id : %d\n",keyFunc(dataArr[i]->shm.pKey), (DWORD)(dataArr[i]->shm.pKey), dataArr[i]->id_);
    //}
    return 0;
}
