
/**
 * 文件名：atom.h
 * 作者：qylWheels
 * 日期：2022年7月29日
 * 功能：提供接口
 */

#ifndef _ATOM_H_
#define _ATOM_H_

// 对外提供的接口
extern       int   atom_length(const char *str);
extern const char *atom_string(const char *str);
extern const char *atom_int   (long n);

/* XXX: 测试用的接口 */
extern       int atom_get_count(void);

#endif

