#include <cstdio>

using LL = long long;

LL getBig(LL a) {
    LL lsb = a & -a;                    // lsb찾기
    a = a / lsb + 1;                    // **** 0111 형식은  **** 1000 형식으로 만들기
    return a * lsb + (a & -a) / 2 - 1;  /*a를 원래의 자리로 돌리고 나머지 부분을 채우기 -> (a & -a)는  0이 나오기 전 LSB에서 부터 연속된 1 ex) 3개면 2b1000
    ->나누기 2를 해서 MSB쪽으로 올라간 1하나를 지움 ex) 2b100 -> -1을 통해서 2b11로 만듦*/
}

int main() {
    LL a;
    scanf("%lld", &a);
    printf("%lld %lld\n", ~getBig(~a), getBig(a));
    return 0;
}