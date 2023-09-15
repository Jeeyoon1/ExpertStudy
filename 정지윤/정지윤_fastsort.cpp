/// ***** user.cpp *****

#define SIZE (1048576)
#define MASK (0xF)

extern void hw_sort(short data[1024]);

inline void swap(int& a, int& b) {
	int t = a;
	a = b;
	b = t;
}

void radix_sort(unsigned int* data) {
	//short = 16bit
	//두번만에 sorting이 가능하다...
	//뭘까??
	short radix_data[1024];
	unsigned int new_data[1024];
	unsigned int *dt = data, * nwdt = new_data;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 1024; j++) {
			radix_data[j] = j;
			radix_data[j] |= (((dt[j] >> (4 * i)) & MASK) << 10); // 10 bit for index, 6bit for data
		}
	 	hw_sort(radix_data);
		for (int j = 0; j < 1024; j++) {
			nwdt[j] = dt[radix_data[j] & 0x3FF];
		}
		unsigned int* t = nwdt;
		nwdt = dt;
		dt = t;
	}

}


void test(unsigned int data[SIZE]) {
	//처음에는 fast_sort를 이용하여 1024개씩 sorting 되게 설정
	for (int i = 0; i < 1024; i++) {
		radix_sort(data + i * 1024);
	}
	unsigned int temp_dummy[65536];
	int tp = 0;
	int num_total;
	// 이후 65536까지는 Merge sort 진행
	
	// 첫번 째 Merge sort --> 거꾸리 하기
	int s = 1024 * 2;
	int t = 0;
	for (int i = 0; i < SIZE; i += s) {
		t = 0;
		int b_s = i + s / 2 - 1;
		int a_s = i + s - 1;
		while (b_s >= i  && a_s >= i + s / 2) {
			if (data[b_s] > data[a_s]) {
				temp_dummy[t++] = data[b_s--];
			}
			else {
				temp_dummy[t++] = data[a_s--];
			}
		}
		while (b_s >= i) {
			temp_dummy[t++] = data[b_s--];
		}
		while (a_s >= i + s/2) {
			temp_dummy[t++] = data[a_s--];
		}
		for (int j = i; j < i + s; j++) {
			data[j] = temp_dummy[j - i];
		}
		
	}
	//2번 째 부터 65536 (지역변수로 커버 가능한 경우) --> merge sort 진행
	s = 1024*4;
	while (s <= 65536) {
		for (int i = 0; i < SIZE; i += s) {
			t = 0;
			int b_s = i;
			int a_s = i + s / 2;
			while (b_s < i + s / 2 && a_s < i + s) {
				if (data[b_s] > data[a_s]) {
					temp_dummy[t++] = data[b_s++];
				}
				else {
					temp_dummy[t++] = data[a_s++];
				}
			}
			while (b_s < i + s / 2) {
				temp_dummy[t++] = data[b_s++];
			}
			while (a_s < i + s) {
				temp_dummy[t++] = data[a_s++];
			}
			for (int j = i; j < i + s; j++) {
				data[j] = temp_dummy[j - i];
			}
		
		}
		s *= 2;
	}
	//이악물고 Merge sort 진행.. (65536의 공간으로 Mergesort 진행 가능!)
	while (s <= SIZE) {
		for (int i = 0; i < SIZE; i += s) {
			t = 0;
			num_total = s;
			int b_s = i;
			int a_s = i + s / 2;
			int last_idx = i + s / 2 - 1;
			while (true) {
				while (b_s <= last_idx && a_s < i + s && t < 65536) {
					if (data[b_s] > data[a_s]) {
						temp_dummy[t++] = data[b_s++];
					}
					else {
						temp_dummy[t++] = data[a_s++];
					}
				}
				while (b_s <= last_idx && t < 65536) {
					temp_dummy[t++] = data[b_s++];
				}
				while (a_s < i + s && t < 65536) {
					temp_dummy[t++] = data[a_s++];
				}
				int k, j;
				for (k = a_s - 1,j = last_idx; j >= b_s; k--, j--) { //남은 공간을 뒤로 밀어버리기
					data[k] = data[j];
				}
				b_s = k+1;
				for (j = 0; j < 65536; j++) {
					data[i+s-num_total+j] = temp_dummy[j];
				}
				last_idx = a_s - 1;
				num_total -= 65536;
				t = 0;
				if (num_total == 0) break;
			}
		}
		s *= 2;
	}
}
