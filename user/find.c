#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

/// 从路径中找到文件名
char *fmt_name(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  memmove(buf, p, strlen(p) + 1);
  return buf;
}

///比较两个文件名
void compare_filename(char *fileName, char *findName)
{
  if (strcmp(fmt_name(fileName), findName) == 0)
  {
    printf("%s\n", fileName);
  }
}

///找到对应文件
void find(char *path, char *findName)
{
  int fd;
  struct stat st;

  // 以只读的方式打开path文件路径，打开的文件状态赋值到fd 成功打开返回0，否则返回-1
  if ((fd = open(path, O_RDONLY)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  // 将已经打开的fd文件路径的信息存储在结构体st中
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  char buf[512], *p;
  struct dirent de;

  // 判断打开的fd的类型
  switch (st.type)
  {
  // 比较打开的文件是否是目标文件
  case T_FILE:
    compare_filename(path, findName);
    break;

  // 从当前打开的目录下寻找目标文件
  case T_DIR:
    // 路径过长
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("find: path too long\n");
      break;
    }
    
    // 形成当前目录的子目录的根目录
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    // 循环读取当前目录下的每一个子目录（文件路径）的状态
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      // 不允许递归进入”.”和”..”
      if (de.inum == 0 || de.inum == 1 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;

      // 形成子目录并递归调用find
      memmove(p, de.name, strlen(de.name));
      p[strlen(de.name)] = 0;
      find(buf, findName);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("find: find <path> <fileName>\n");
    exit();
  }
  find(argv[1], argv[2]);
  exit();
}