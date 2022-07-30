
/**
 * 文件名：atom.c
 * 作者：qylWheels
 * 日期：2022年7月29日
 * 功能：接口的实现
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atom.h"

#define NELEMS(x) (sizeof(x) / sizeof(x[0]))

// 原子的数据结构
struct _Atom {
    int len;
    char *str;
    struct _Atom *next;
} *_buckets[2048];

// 用于均匀生成字符串hash的数据结构
struct _Scatter {
    int flag;
    unsigned long scatter_table[256];
} _scatter = {0, {0}};

// 内部函数声明
const    char *_atom_new    (const char *str, int len);
         void  _init_scatter(void);
unsigned long  _calc_hash   (const char *str, int len);

// 接口的实现
int atom_length(const char *str) {

}

const char *atom_string(const char *str) {

}

const char *atom_int(long n) {

}

// 内部函数的实现
const char *_atom_new(const char *str, int len) {
    assert(str != NULL);
    assert(len >= 0);

    struct _Atom *p;

    // 查找是否已有相同的原子，有则直接返回
    unsigned long h;
    int i;

    h = _calc_hash(str, strlen(str));
    h &= NELEMS(_buckets) - 1;

    for(p = _buckets[h]; p != NULL; p = p->next) {
        if(p->len == len) {
            for(i = 0; i < len && p->str[i] == str[i]; ) {
                i++;
            }
            if(i == len) {
                return p->str;
            }
        }
    }
    
    // 分配空间
    p = (struct _Atom *)malloc(sizeof(struct _Atom));
    p->str = (char *)malloc(len + 1);

    // 装载原子
    p->len = len;
    if(len > 0) {
        memcpy(p->str, str, len);
    }
    p->str[len] = '\0';
    p->next = _buckets[h];
    _buckets[h] = p;

    return p->str;
}

unsigned long _calc_hash(const char *str, int len) {

}
