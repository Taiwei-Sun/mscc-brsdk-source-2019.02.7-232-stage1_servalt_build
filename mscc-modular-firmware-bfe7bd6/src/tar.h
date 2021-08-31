// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#ifndef __MFI_SRC_TAR_HXX__
#define __MFI_SRC_TAR_HXX__

static const int offset_name = 0;
static const int offset_mode = 100;
static const int offset_size = 124;
static const int offset_type = 156;
static const int offset_linkname = 157;
static const int offset_dev_major = 329;
static const int offset_dev_minor = 337;
static const int offset_content = 512;

enum Type {
    Unknown,
    File,
    HardLink,
    SymbolicLink,
    Device,
    BlockDevice,
    Directory,
    NamedPipe,
    Longname,
    Longlinkname,
};

typedef enum {
    false,
    true
} bool;

typedef struct TarIterator {
    int data_left;
    const char *data;
    const char *longname;
    const char *longlinkname;
} TarIterator;

TarIterator *TarIterator_new(const char *d, int s);

bool tariterator_next(TarIterator *t);
bool tariterator_check(TarIterator *t);

int tariterator_size(TarIterator *t);
int tariterator_mode(TarIterator *t);
enum Type tariterator_type(TarIterator *t);
int tariterator_dev_minor(TarIterator *t);
int tariterator_dev_major(TarIterator *t);
const char *tariterator_content(TarIterator *t);
const char *tariterator_name(TarIterator *t);
const char *tariterator_linkname(TarIterator *t);


int tar_extract(const char *d, size_t s, const char *prefix);

int tar_xz_extract(const char *d, size_t s, const char *prefix);

enum TarError {
    TAR_ERR_FAILED = -1,
    TAR_ERR_AGAIN = -2,
    TAR_ERR_DONE = -3,
};

typedef struct tar_state {
    bool init;
    int fd;

    char name[512];

    char hdr[512];
    unsigned hdr_valid;
    unsigned data_consumed;

} tar_state_t;

int tar_extract_(const char *d, size_t s, const char *prefix);


#endif  // __MFI_SRC_TAR_HXX__
