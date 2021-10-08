# NachOS
Đây là nơi để thực hiện đồ án môn Hệ điều hành trên hệ điều hành giả lập NachOS

## Lưu ý
- Khi clone lần đầu tiên, hãy `make` tất cả các thành phần như hướng dẫn.
- Trước khi commit, hãy **xem kỹ** các file được commit.  
- Nếu có file không mong muốn, hay thêm vào `.gitignore` trước khi commit.

### Khi muốn thực hiện thay đổi:
1. Tạo branch mới
2. Thay đổi trên branch vừa tạo
3. Mở *Pull request* nếu cần trao đổi thêm
4. Merge `main/master` vào branch của mình
5. Resolve conflict nếu có
6. Merge những thay đổi vào `main/master`

## Các nơi cần code khi thêm 1 system call 
1. test/start.s
2. userprog/syscall.h
3. userprog/exception.cc
4. Thêm file để test system call: test/Makefile
## Resource
[NachOS Setup](https://www.fit.hcmus.edu.vn/~ntquan/os/setup_nachos.html)
