# Đỗ Quốc Vi
TIMER REGISTER - Session 2

Ý tưởng: TIM2 có 4 channel tương ứng lần lượt PA0,PA1,PA2,PA3
         TIM4 sử dụng xuất xung 4 channel tương tứng lần lượt PB6,PB7,PB8,PB9
         Nối các chân của 2 Timer 2 và 4 theo tương ứng channel: PA0-PB6,PA1-PB7,PA2-PB8,PA3-PB9
         PSC= 800 , ARR = Max
         1/ Dùng cặp PA0-PB6 để tính T or f.
            1.1. Capture value input vào CCR1 khi gặp cạnh rising  
            1.2  Reset CNT về 0 khi gặp cạnh rising 
        2/ Dùng PA1,PA2,PA3 để tính độ rộng xung của PB7,PB8,PB9
            - Capture value input vào lần lượt CCR2,CCR3,CCR4 khi gặp cạnh falling

2/ rule commit 
git commit -m "[Moudle][Tên người dev] nội dung commit"



3/ lưu video bài tập GPIO
https://drive.google.com/drive/folders/1lYW8SN_iZ43FdmnMjRMlmO3wTWQ5yoNK