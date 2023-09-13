/// ***** user.cpp *****
#include <stdio.h>
struct rect {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
};

rect R[100];
int rp;
int s_image[65][65];

void rect_check(int start, int pp, int mp, int* p, int* m) {
    //모든 조합을 찾고 뒤져보기.
    for (int i = 0; i < pp; i++) {
        for (int j = 0; j < pp; j++) {
            if ((p[i] + 9) >= m[j]) {
                continue; //최적화 가능할듯
            }
            if (s_image[start][p[i]] <= 0 || s_image[start][m[j]] >= 0) { //이미 처리한 요청일수도?
                continue;
            }
            for (int k = start + 9; k < 65; k++) {
                if (s_image[k][p[i]] < 0 && s_image[k][m[j]] > 0) {
                    R[rp].start_x = start;
                    R[rp].start_y = p[i];
                    R[rp].end_x = k;
                    R[rp++].end_y = m[j];
                    s_image[start][p[i]]--;
                    s_image[start][m[j]]++;
                    s_image[k][p[i]]++;
                    s_image[k][m[j]]--;
                    break;
                }
            }

        }
    }
}
void next_rect_check(int start, int pp, int mp, int* p, int* m) {
    int flag = 0;
    for (int i = 0; i < pp; i++) {
        for (int j = 0; j < pp; j++) {
            if ((p[i] + 9) >= m[j]) {
                continue; //최적화 가능할듯
            }
            if (s_image[start][p[i]] <= 0 || s_image[start][m[j]] >= 0) { //이미 처리한 요청일수도?
                continue;
            }
            for (int k = start + 9; k < 65; k++) {
                if (s_image[k][p[i]] < 0 || s_image[k][m[j]] > 0) {
                    R[rp].start_x = start;
                    R[rp].start_y = p[i];
                    R[rp].end_x = k;
                    R[rp++].end_y = m[j];
                    s_image[start][p[i]]--;
                    s_image[start][m[j]]++;
                    s_image[k][p[i]]++;
                    s_image[k][m[j]]--;
                    flag = 1;
                    break;
                }
            }
            //if (flag == 1) break;
        }
        //if (flag == 1) break;
    }
}

void print_s_image() {
    for (int i = 0; i < 65; i++) {
        for (int j = 0; j < 65; j++) {
            printf("%2d ", s_image[i][j]);
        }
        printf("\n");
    }
}

void dfs(int start_x) {
    int p_array;
    for (int i = 0; i <= 64; i++) {
        if (s_image[start_x][i] > 0) {

        }
    }
}
int recog(unsigned char image[64][64])
{
    rp = 0;
    s_image[0][0] = image[0][0];
    for (int i = 1; i < 64; i++) {
        //image[0][i] = image[0][i-1] + s_image[0][i];
        s_image[0][i] = image[0][i] - image[0][i - 1];
        s_image[i][0] = image[i][0] - image[i - 1][0];

    }
    s_image[0][64] = -1 * image[0][63];
    s_image[64][0] = -1 * image[63][0];

    int a, b, c;
    for (int i = 1; i < 65; i++) {
        for (int j = 1; j < 65; j++) {
            // image[i][j] = image[i][0] + image[0][j] - image[0][0] + s_image[i][j];
            if (i == 64 && j == 64) {
                a = 0; b = 0; c = 0;
            }
            else if (i == 64 && j < 64) {
                a = 0; b = 0; c = image[i - 1][j];
            }
            else if (i < 64 && j == 64) {
                a = 0; b = image[i][j - 1]; c = 0;
            }
            else  if (i < 64 && j < 64) {
                a = image[i][j]; b = image[i][j - 1]; c = image[i - 1][j];
            }
            //s_image[i][j] = image[i][j] - image[i][j-1] - image[i-1][j] + image[i-1][j-1];
            s_image[i][j] = a - b - c + image[i - 1][j - 1];
        }
    }
    int sum = 0;
    int p_arr[64];
    int m_arr[64];
    int pp;
    int mp;
    int flag;

    // print_s_image();
    //확실하게 찾아지는 애들 먼저 다 찾아두고, 마지막에 남은 애들끼리 조합을 해보자!
    // ㅈ같이 생겨도 가능성은 있겠지 뭐...
    //확시 + 위에서 아래를 보는 애만 찾을거임 (무조건 p가 m보다 앞에 있어야함!!)
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i <= 64; i++) {
            pp = 0; mp = 0; flag = 0; sum = 0;
            //int j = 0;
            for (int j = 0; j < 65; j++) {
                int value = s_image[i][j];
                while (value > 0) {
                    p_arr[pp++] = j;
                    value--;
                    sum++;
                    flag = 1;
                }
                while (value < 0) {
                    m_arr[mp++] = j;
                    value++;
                    sum--;
                    flag = 1;
                }
            }
            if (flag == 1 && sum == 0) {
                rect_check(i, pp, mp, p_arr, m_arr);
                flag = 0;
                sum = 0;
                pp = 0; mp = 0;
            }
        }
    }
    //printf("rp is %d\n", rp);
    for (int i = 0; i <= 64; i++) {
        pp = 0; mp = 0; flag = 0; sum = 0;
        //int j = 0;
        for (int j = 0; j < 65; j++) {
            int value = s_image[i][j];
            while (value > 0) {
                p_arr[pp++] = j;
                value--;
                sum++;
                flag = 1;
            }
            while (value < 0) {
                m_arr[mp++] = j;
                value++;
                sum--;
                flag = 1;
            }
        }
        if (flag == 1 && sum == 0) {
            next_rect_check(i, pp, mp, p_arr, m_arr);
            flag = 0;
            sum = 0;
            pp = 0; mp = 0;
        }
    }
   
    int ans = 0;
   
    for (int i = 0; i < rp; i++) {
        if ((R[i].end_x - R[i].start_x) == (R[i].end_y - R[i].start_y)) {
            ans++;
        }
    }
    /*if (m_sum > 0) {
        printf("123\n");
    }*/
    
    return ans;
}
