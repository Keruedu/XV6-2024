#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h" 
                                                                                          // hàm fmt để trả về tên thư mục hoặc tệp từ đường dẫn path
char *fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

                                                                                              // Tìm kí tự '/' cuối cùng
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

                                                                                       // Trả về tên của  của tệp hoặc thư mục đang đọc
  if (strlen(p) >= DIRSIZ)                                                              // kiểm tra chiều dài có vượt quá giới hạn không
    return p;
  memmove(buf, p, strlen(p));                                                         // copy tên thư mục hoặc tệp đang đọc vào buf
  buf[strlen(p)] = '\0';                                                              // chuyển kí tự cuối cùng về '\0'
  return buf;                                                                         // trả về buf
} 
                                                                                          // viết hàm find
void find(char *path, char *findname)
{
  char buf[512], *p;                                                              // buf là mảng kí tự dùng để lưu đường dẫn tạm. p là con trỏ trỏ tới phần tử của mảng đó
  int fd;                                                                             // lưu file descriptor
  struct dirent de;                                                                  // lưu phần tử đại diện cho 1 entry
  struct stat st;                                                                    // lưu phần tử chứa thông ti của tệp

  // lấy fd từ đường dẫn và kiểm tra
  if ((fd = open(path, O_RDONLY)) < 0)
  {
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

                                                                                      // lấy st từ fd và kiểm tra
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
                                                                                        // tùy loại tệp mà ta xử lý khác nhau
  switch (st.type)
  {
  case T_DEVICE:
  case T_FILE:                                                                          // nếu là file thì in ra đường dẫn luôn

    if (strcmp(fmtname(path), findname) == 0)                                           // kiểm tra tên file đang đọc có trùng với tên file đang kiếm không
      printf("%s\n", path);
    break;

  case T_DIR:                                                                                 // nếu là thư mục thì ta phải vừa tìm vừa truy xuất vào thư mục con ( dùng đệ quy )
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {                                                                                       // kiểm tra chiều dài
      printf("ls: path too long\n");
      break;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {                                                                                        // đọc các thư mục hoặc tệp ở thư mục hiện tại

      if (strcmp(".", fmtname(de.name)) == 0 || strcmp("..", fmtname(de.name)) == 0)
      {                                                                                     // bỏ qua thư mục . và  ..
        continue;
      }

      if (de.inum == 0)                                                                          // kiểm tra thông tin của entry
        continue;
                                                                                                   // tạo đường dẫn đến các thư mục, tệp con
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
                                                                                                   // Kiểm tra xem có mở được không
      if (stat(buf, &st) < 0)
      {
        printf("ls: cannot stat %s\n", buf);
        continue;
      }

                                                                                                      // Đệ quy vào tệp hoặc thư mục, nếu là tệp thì phía trên đã xử lý, nếu là thư mục thì sẽ đệ quy lần nữa
      find(buf, findname);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  // Kiểm tra số lượng tham số
  if (argc < 3)
  {
    printf("Usage: %s <path> <name_to_find>\n", argv[0]);
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}