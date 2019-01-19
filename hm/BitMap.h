#ifndef LEVELDB_MASTER_BITMAP_H
#define LEVELDB_MASTER_BITMAP_H 
//////
//模块功能:实现bitmap管理
//////
namespace leveldb{
    class BitMap {

    public:
        BitMap();

        BitMap(int n);

        ~BitMap();

        int get(int x);

        int set(int x);

        int clr(int x);

        int reset();
        
    private:
        char *bitmap;
        int gsize;
    }; 
}
#endif