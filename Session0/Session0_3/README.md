# Đỗ Quốc Vi

# Những câu lệnh cơ bản thường sử dụng:

# Thiết lập chứng thực cá nhân :

# $ git config --global user.name "User Name"

# $ git config --global user.email "username@gmail.com"

# 2\. Tạo một kho chứa Git

# $ git init

# 3\. Sao chép một kho chứa đã tồn tại:

# $ git clone https://github.com/user/repository.git

# 4\. Nhánh trong git:

# $ git branch

# Để tạo mới một branch:

# 

# &#x20;$ git branch <name\_branch>

# 

# 5\. Để chuyển và tạo mới:

# &#x20;$ git checkout -b <name\_branch>

# 6\. Chuyển nhánh:

# 7\. $ git checkout <name\_branch>

# 8\. Cập nhật thay đổi:

# $ git add .

# 9\. Sau lệnh add, bạn cần sử dụng câu lệnh Commit để đây thông tin thay đổi lên Local Respository:

# 

# $ git commit -m "Message"

# 10\. Cập nhật lên server:

# $ git push origin <name\_branch>

# Ngoài ra, nếu chưa tồn tại remote trên server thì bạn cần phải add mới một remote trước rồi mới push:

# 

# $ git remote add origin <remote\_url>

# $ git push origin <name\_branch>

# 11\. Gộp nhánh

# $ git checkout master

# $ git merge <new\_branch>

# 12\. Xem lại lịch sử commit:

# $ git log

# 13\. Xem thay đổi trước khi push:

# &#x20; $ git diff

# 14\. Gộp commit: $ git rebase -i HEAD\~

# Pull từ remote repository:

# $ git pull origin master

