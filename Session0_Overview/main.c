#include <stdio.h>

int main() {
    int num1, num2, sum;

    // Nhập hai số từ người dùng
    printf("Nhập số thứ nhất: ");
    scanf("%d", &num1);

    printf("Nhập số thứ hai: ");
    scanf("%d", &num2);

    // Tính tổng
    sum = num1 + num2;

    // In kết quả
    printf("Tổng của %d và %d là: %d\n", num1, num2, sum);

    return 0;
}
