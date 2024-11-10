#include "kernel/types.h"  // Khai báo các kiểu dữ liệu cơ bản (int, uint, ...)
#include "kernel/stat.h"   // Khai báo các hàm liên quan đến file stat
#include "user/user.h"     // Khai báo các hàm hệ thống như fork, pipe, read, write

int main(int argc, char *argv[])
{
       int p[2];  // tạo 2 file descriptor p[0] để, đọc p[1] để ghi
    char recv_buf[5]; // tạo chuỗi chứa dữ liệu nhận được

    pipe(p); // tạo đường ống giữa 2 tiến trình

    if (fork() == 0) {  // gọi fork() để tạo tiến trình con trong tiến trình cha đang chạy, nó sẽ trả về khác 0 ở tiến trình cha và bằng 0 ở tiến trình con
        read(p[0], recv_buf, 5);  // đọc dữ liệu từ cha
        printf("%d: received %s\n", getpid(), recv_buf); // in ra màn hình
        close(p[0]);  // đóng đầu đọc

        write(p[1], "pong", 5); // ghi dữ liệu vào pipe
        close(p[1]); // đóng đầu ghi

    } else {  // Tiến trình cha
        write(p[1], "ping", 5);  // viết vào pipe
        close(p[1]);    // đóng đầu viết
             wait(0);  // chờ tiến trình con chạy xong
        read(p[0], recv_buf, 5);  // đọc dữ liệu từ tiến trình con
        printf("%d: received %s\n", getpid(), recv_buf); // in ra màn hình
        close(p[0]); // đóng đầu đọc
    }
    exit(0);
}