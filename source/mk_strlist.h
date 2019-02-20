#ifndef mk_strlist_h
#define mk_strlist_h

struct mk_strlist;

struct mk_strlist* mk_strlist_create();

void mk_strlist_destroy(struct mk_strlist* strlist);

int mk_strlist_push(struct mk_strlist* strlist, char* str);

#endif /* mk_strlist_h */
