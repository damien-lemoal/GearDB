#ifndef LEVELDB_HM_STATUS_H
#define LEVELDB_HM_STATUS_H

// Disk physical block size
#define PHY_SECTOR_SIZE		4096

// The proportion of Compaction window to the total number
// of zone numbers in the level
#define COM_WINDOW_SCALE	4

// The level's data reaches the level threshold * 1/HAVE_WINDOW_SCALE,
// then have compaction window
#define HAVE_WINDOW_SCALE	4

// 0 means the compaction window selects zone random; 1 means the compaction
// window selects zone compaction
#define COM_WINDOW_SEQ		1

// The size of the alignment when applying for memory using posix_memalign
#define MEMALIGN_SIZE		(sysconf(_SC_PAGESIZE))

// To confirm whether the SSTable is useful, every time an SSTable is written
// to the disk, it will read the handle of the file and add it to the leveldb's
// table cache. This is the leveldb's own mechanism;
// 1 means that there is this mechanism; 0 means no such mechanism.
#define Verify_Table		1

// 1 means that the entire file is read together when reading the SSTable;
// 0 means that a block is read during the read operation.
#define Read_Whole_Table	1

// 1 means that when finding key-value, it is the same as the original,
// a block is read; 0 means that the search key-value is the same as the
// Read_Whole_Table parameter.
#define Find_Table_Old		1

#endif
