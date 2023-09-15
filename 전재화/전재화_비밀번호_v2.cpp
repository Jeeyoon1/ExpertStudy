#include <cstdio>

using LL = long long;

LL getBig(LL a) {
    LL lsb = a & -a;                    // lsbã��
    a = a / lsb + 1;                    // **** 0111 ������  **** 1000 �������� �����
    return a * lsb + (a & -a) / 2 - 1;  /*a�� ������ �ڸ��� ������ ������ �κ��� ä��� -> (a & -a)��  0�� ������ �� LSB���� ���� ���ӵ� 1 ex) 3���� 2b1000
    ->������ 2�� �ؼ� MSB������ �ö� 1�ϳ��� ���� ex) 2b100 -> -1�� ���ؼ� 2b11�� ����*/
}

int main() {
    LL a;
    scanf("%lld", &a);
    printf("%lld %lld\n", ~getBig(~a), getBig(a));
    return 0;
}