#ifndef LEVELDB_HM_MANAGER_H
#define LEVELDB_HM_MANAGER_H

//////
//Module function: Main module
//////

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <map>
#include <vector>
#include "leveldb/env.h"

#include "../db/dbformat.h"
#include "../hm/my_log.h"
#include "../hm/BitMap.h"
#include "../hm/hm_status.h"

extern "C" {
#include <libzbc/zbc.h>
}

namespace leveldb {

  struct Ldbfile {	// file = SSTable ,file Metadata struct
    uint64_t table;	// file name = fiel serial number
    uint64_t zone;	// file's zone number
    uint64_t offset;	// file's offset in the zone
    uint64_t size;	// file's size
    int      level;	// file in the level number

  Ldbfile(uint64_t a, uint64_t b, uint64_t c, uint64_t d, int e):
    table(a), zone(b), offset(c), size(d), level(e){};
    ~Ldbfile(){};
  };

  struct Zonefile {	// zone struct
    uint64_t zone;	// Number of the zone used for the file
    std::vector<struct Ldbfile*> ldb; //SSTable pointers

  Zonefile(uint64_t a):zone(a){};
    ~Zonefile(){};

    void add_table(struct Ldbfile* file)
    {
      ldb.push_back(file);
    }

    void delete_table(struct Ldbfile* file)
    {
      std::vector<struct Ldbfile*>::iterator it;

      for (it = ldb.begin(); it != ldb.end(); it++) {
	if (*it == file) {
	  ldb.erase(it);
	  return;
	}
      }
    }

    uint64_t get_all_file_size(void)
    {
      std::vector<struct Ldbfile*>::iterator it;
      uint64_t size = 0;

      for (it = ldb.begin(); it != ldb.end(); it++)
	size += (*it)->size;

      return size;
    }
  };

  class HMManager {
  public:
    HMManager(const Comparator *icmp);
    ~HMManager();

    // Open a disk
    int hm_open(const char *disk_path);

    // Write a SSTable file to a level
    ssize_t hm_write(int level, uint64_t filenum,
		     const void *buf, uint64_t count);

    // Read a SSTable file
    ssize_t hm_read(uint64_t filenum, void *buf,
		    uint64_t count, uint64_t offset);

    // Delete a SSTable file
    ssize_t hm_delete(uint64_t filenum);

    // Move a SSTable file
    ssize_t move_file(uint64_t filenum,int to_level);

    // Get a SSTable file pointer
    struct Ldbfile* get_one_table(uint64_t filenum);

    // Get table_map
    void get_table(std::map<uint64_t, struct Ldbfile*> **table_map)
    {
      *table_map = &table_map_;
    };

    // Dump relation
    void get_zone_table(uint64_t filenum,
			std::vector<struct Ldbfile*> **zone_table);
    bool trivial_zone_size_move(uint64_t filenum);
    void move_zone(uint64_t filenum);

    // Compaction relation
    void update_com_window(int level);
    void get_com_window_table(int level,
			      std::vector<struct Ldbfile*> *window_table);
    ssize_t adjust_com_window_num(int level);
    void set_com_window(int level, int num);
    void set_com_window_seq(int level, int num);

    // Statistics
    uint64_t get_zone_num();
    void get_one_level(int level, uint64_t *table_num,
		       uint64_t *table_size);
    void get_per_level_info();
    void get_valid_info();
    void get_all_info();
    void get_valid_data();
    void get_info(int num);
    void get_valid_all_data(int num);

  private:
    const char *disk_path_;
    struct zbc_device *dev_;
    struct zbc_zone  *zone_;
    BitMap *bitmap_;
    unsigned int zonenum_;
    unsigned int first_zonenum_;

    const InternalKeyComparator icmp_;

    //<file number, metadate pointer>
    std::map<uint64_t, struct Ldbfile*> table_map_;

    // Each level of zone
    std::vector<struct Zonefile*> zone_info_[config::kNumLevels];

    // Each level of compaction window
    std::vector<struct Zonefile*> com_window_[config::kNumLevels];

    // Statistics
    uint64_t delete_zone_num;
    uint64_t all_table_size;
    uint64_t kv_store_sector;
    uint64_t kv_read_sector;
    uint64_t max_zone_num;
    uint64_t move_file_size;
    uint64_t read_time;
    uint64_t write_time;
    Env* env;

    unsigned int set_first_zonenum();
    ssize_t hm_alloc(int level, uint64_t size);
    ssize_t hm_alloc_zone();
    void hm_free_zone(uint64_t zone);

    bool is_com_window(int level, uint64_t zone);
  };
}

#endif
