
/**
 * 文件名：atom.c
 * 作者：qylWheels
 * 日期：2022年7月31日
 * 功能：接口的实现
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
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
    assert(str != NULL);
    
    struct _Atom *p;
    int i;

    for(i = 0; i < NELEMS(_buckets); i++) {
	    for(p = _buckets[i]; p != NULL; p = p->next) {
		    if(p->str == str) {
			    return p->len;
		    }
	    }
    }

    assert(0);
    return -1;
}

const char *atom_string(const char *str) {
    assert(str != NULL);

    return _atom_new(str, strlen(str));
}

const char *atom_int(long n) {
    unsigned long m;
    char str[43];	// 43是一个魔法数字，一个十进制long型数据不会超过43位
    char *s = str + sizeof(str);

    if(n == LONG_MIN) {
        m = LONG_MAX + 1UL;
    } else if(n < 0) {
        m = -n; 
    } else {
        m = n; 
    }

    do {
	*--s = m % 10 + '0';
    } while((m /= 10) > 0);
    if(n < 0) {
	*--s = '-';
    }

    return _atom_new(s, str + sizeof(str) - s);
}

/* XXX: 测试用的接口 */
int atom_get_count(void) {
	struct _Atom *p;
	int atom_cnt = 0;

	for (int i = 0; i < 2048; i++) {
		for (p = _buckets[i]; p != NULL; p = p->next) {
			atom_cnt++;
		}
	}
	return atom_cnt;
}

// 内部函数的实现
const char *_atom_new(const char *str, int len) {
    assert(str != NULL);
    assert(len >= 0);

    // 若_scatter数组未初始化，先初始化
    if(_scatter.flag == 0) {
	_init_scatter();
    }

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

void _init_scatter(void) {
    int i;
    struct timeval t;

    for(i = 0; i < 256; i++) {
	gettimeofday(&t, NULL);
        srand((unsigned int)t.tv_usec);
    	_scatter.scatter_table[i] = (unsigned long)rand();
    } 
    
    _scatter.flag = 1;

    return;
}

unsigned long _calc_hash(const char *str, int len) {
    int i;
    unsigned long h;
    for(h = 0, i = 0; i < len; i++) {
	h = (h << 1) + _scatter.scatter_table[(unsigned char)str[i]];
    }
    return h;
}

