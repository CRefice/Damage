void unimpl_op() { throw; }

//8-bit loads
template <uint x>
void ld_r_n() {
	r[x] = read(r[PC]++);
}

template <uint r1, uint r2>
void ld_r_r() {
	r[r1] = r[r2];
}

template <uint x>
void ld_r_hl() {
	r[x] = read(r[HL]);
}

template <uint x>
void ld_hl_r() {
	write(r[HL], r[x]);
}

void ld_hl_n() {
	write(r[HL], read(r[PC]++));
}

template <uint x>
void ld_a_rr() {
	r[A] = read(r[x]);
}

void ld_a_mm() {
	uint8 lo = read(r[PC]++);
	uint8 hi = read(r[PC]++);
	r[A] = read((hi << 8) | (lo << 0));
}

void ld_a_n() {
	r[A] = read(r[PC]++);
}

template <uint x>
void ld_r_a() {
	r[x] = r[A];
}

template <uint x>
void ld_rr_a() {
	write(r[x], r[A]);
}

void ld_mm_a() {
	uint8 lo = read(r[PC]++);
	uint8 hi = read(r[PC]++);

	write((hi << 8) | (lo << 0), r[A]);
}

void ld_a_c() {
	r[A] = read(0xFF00 | r[C]);
}

void ld_c_a() {
	write(0xFF00 | r[C], r[A]);
}

void ldd_a_hl() {
	r[A] = read(r[HL]);
	r[HL]--;
}

void ldd_hl_a() {
	write(r[HL], r[A]);
	r[HL]--;
}

void ldi_a_hl() {
	r[A] = read(r[HL]);
	r[HL]++;
}

void ldi_hl_a() {
	write(r[HL], r[A]);
	r[HL]++;
}

void ldh_n_a() {
	write(0xFF00 | read(r[PC]++), r[A]);
}

void ldh_a_n() {
	r[A] = read(0xFF00 | read(r[PC]++));
}

//16-bit loads
template <uint x>
void ld_rr_nn() {
	uint8 lo = read(r[PC]++);
	uint8 hi = read(r[PC]++);

	r[x] = (hi << 8) | (lo << 0);
}

void ld_sp_hl() {
	cycle();
	r[SP] = r[HL];
}

void ld_hl_sp_n() {
	cycle();

	int8 n = (int8)read(r[PC]++);

	r.f.z = 0;
	r.f.n = 0;
	r.f.h = ((r[SP] & 0x0f) + (n & 0x0f)) > 0x0f;
	r.f.c = ((r[SP] & 0xff) + (n & 0xff)) > 0xff;

	r[HL] = r[SP] + n;
}

void ld_mm_sp() {
	uint16 addr = read(r[PC]++);
	addr |= read(r[PC]++) << 8;
	write(addr, r[SP] >> 0);
	write(addr + 1, r[SP] >> 8);
}

template <uint x>
void push_rr() {
	cycle();
	stackPush(r[x]);
}

template <uint x>
void pop_rr() {
	r[x] = stackPop();
}

//8-bit arithmetic
void add_a(uint8 n) {
	uint16 hi = r[A] + n;
	uint16 lo = (r[A] & 0x0F) + (n & 0x0F);

	r[A] = hi;

	r.f.z = (uint8)hi == 0;
	r.f.n = 0;
	r.f.h = lo > 0x0F;
	r.f.c = hi > 0xFF;

}
template <uint x>
void add_a_r() { add_a(r[x]); }

void add_a_hl() { add_a(read(r[HL])); }

void add_a_n() { add_a(read(r[PC]++)); }

void adc_a(uint8 n) {
	uint16 hi = r[A] + n + r.f.c;
	uint16 lo = (r[A] & 0x0F) + (n & 0x0F) + r.f.c;

	r[A] = hi;

	r.f.z = (uint8)hi == 0;
	r.f.n = 0;
	r.f.h = lo > 0x0F;
	r.f.c = hi > 0xFF;
}

template <uint x>
void adc_a_r() { adc_a(r[x]); }

void adc_a_hl() { adc_a(read(r[HL])); }

void adc_a_n() { adc_a(read(r[PC]++)); }

void sub_a(uint8 n) {
	uint16 hi = r[A] - n;
	uint16 lo = (r[A] & 0x0F) - (n & 0x0F);

	r[A] = hi;

	r.f.z = (uint8)hi == 0;
	r.f.n = 1;
	r.f.h = lo > 0x0F;
	r.f.c = hi > 0xFF;
}

template <uint x>
void sub_a_r() { sub_a(r[x]); }

void sub_a_hl() { sub_a(read(r[HL])); }

void sub_a_n() { sub_a(read(r[PC]++)); }

void sbc_a(uint8 n) {
	uint16 hi = r[A] - n - r.f.c;
	uint16 lo = (r[A] & 0x0F) - (n & 0x0F) - r.f.c;

	r[A] = hi;

	r.f.z = (uint8)hi == 0;
	r.f.n = 1;
	r.f.h = lo > 0x0F;
	r.f.c = hi > 0xFF;
}

template <uint x>
void sbc_a_r() { sbc_a(r[x]); }

void sbc_a_hl() { sbc_a(read(r[HL])); }

void sbc_a_n() { sbc_a(read(r[PC]++)); }

void and_a(uint8 n) {
	r[A] &= n;

	r.f.z = r[A] == 0;
	r.f.n = 0;
	r.f.h = 1;
	r.f.c = 0;
}

template <uint x>
void and_a_r() { and_a(r[x]); }

void and_a_hl() { and_a(read(r[HL])); }

void and_a_n() { and_a(read(r[PC]++)); }

void or_a(uint8 n) {
	r[A] |= n;

	r.f.z = r[A] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = 0;
}

template <uint x>
void or_a_r() { or_a(r[x]); }

void or_a_hl() { or_a(read(r[HL])); }

void or_a_n() { or_a(read(r[PC]++)); }

void xor_a(uint8 n) {
	r[A] ^= n;

	r.f.z = r[A] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = 0;
}

template <uint x>
void xor_a_r() { xor_a(r[x]); }

void xor_a_hl() { xor_a(read(r[HL])); }

void xor_a_n() { xor_a(read(r[PC]++)); }

void cp_a(uint8 n) {
	uint16 hi = r[A] - n;
	uint16 lo = (r[A] & 0x0F) - (n & 0x0F);

	r.f.z = (uint8)hi == 0;
	r.f.n = 1;
	r.f.h = lo > 0x0F;
	r.f.c = hi > 0xFF;
}

template<uint x>
void cp_a_r() { cp_a(r[x]); }

void cp_a_hl() { cp_a(read(r[HL])); }

void cp_a_n() { cp_a(read(r[PC]++)); }

template <uint x>
void inc_r() {
	r[x]++;

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = (r[x] & 0x0F) == 0;
}

void inc_hl() {
	uint8 n = read(r[HL]);
	write(r[HL], ++n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = (n & 0x0F) == 0;
}

template <uint x>
void dec_r() {
	r[x]--;

	r.f.z = r[x] == 0;
	r.f.n = 1;
	r.f.h = (r[x] & 0x0F) == 0x0F;
}

void dec_hl() {
	uint8 n = read(r[HL]);
	write(r[HL], --n);

	r.f.z = n == 0;
	r.f.n = 1;
	r.f.h = (n & 0x0F) == 0x0F;
}

//16-bit arithmetic
template <uint x>
void add_hl_rr() {
	cycle();

	uint hi = (r[HL] + r[x]);
	uint lo = (r[HL] & 0x0FFF) + (r[x] & 0x0FFF);

	r[HL] = hi;

	r.f.n = 0;
	r.f.h = lo > 0x0FFF;
	r.f.c = hi > 0xFFFF;
}

void add_sp_n() {
	cycle();
	cycle();

	int8 n = read(r[PC]++);

	r.f.z = 0;
	r.f.n = 0;
	r.f.h = ((r[SP] & 0x0F) + (n & 0x0F)) > 0x0F;
	r.f.c = ((r[SP] & 0xFF) + (n & 0xFF)) > 0xFF;

	r[SP] += n;
}

template <uint x>
void inc_rr() {
	cycle();
	r[x]++;
}

template <uint x>
void dec_rr() {
	cycle();
	r[x]--;
}


//Miscellaneous commands
void daa() {
	uint16 a = r[A];

	if (r.f.n == 0) {
		if (r.f.h || (a & 0x0f) > 0x09) a += 0x06;
		if (r.f.c || (a) > 0x9f) a += 0x60;
	}
	else {
		if (r.f.h) {
			a -= 0x06;
			if (r.f.c == 0) a &= 0xff;
		}
		if (r.f.c) a -= 0x60;
	}
	r[A] = a;
	r.f.z = r[A] == 0;
	r.f.h = false;
	r.f.c = r.f.c || (a & 0x100) != 0;
}

void cpl() {
	r[A] ^= 0xFF;

	r.f.n = 1;
	r.f.h = 1;
}

void ccf() {
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = !r.f.c;
}

void scf() {
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = 1;
}

void nop() {}

void halt() {
	r.halt = true;
}

void stop() {
	r.stop = true;
}

void di() {
	r.ime = false;
}

void ei() {
	r.ei = true;
}

//Rotates and shift operations
void rlca() {
	r[A] = (r[A] << 1) | (r[A] >> 7);

	r.f.z = 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = r[A] & 0x01;
}

void rla() {
	bool c = (r[A] & 0x80) != 0;;
	r[A] = (r[A] << 1) | (int)r.f.c;

	r.f.z = 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

void rrca() {
	r[A] = (r[A] >> 1) | (r[A] << 7);

	r.f.z = 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = (r[A] & 0x80) != 0;
}

void rra() {
	bool c = r[A] & 0x01;
	r[A] = (r[A] >> 1) | (r.f.c << 7);

	r.f.z = 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

template <uint x>
void rlc_r() {
	r[x] = (r[x] << 1) | (r[x] >> 7);

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = r[x] & 0x01;
}

void rlc_hl() {
	uint8 n = read(r[HL]);
	n = (n << 1) | (n >> 7);

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = n & 0x01;
}

template <uint x>
void rl_r() {
	bool c = (r[x] & 0x80) != 0;;
	r[x] = (r[x] << 1) | (int)r.f.c;

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

void rl_hl() {
	uint8 n = read(r[HL]);
	bool c = (n & 0x80) != 0;
	n = (n << 1) | (int)r.f.c;

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

template <uint x>
void rrc_r() {
	r[x] = (r[x] >> 1) | (r[x] << 7);

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = (r[x] & 0x80) != 0;
}

void rrc_hl() {
	uint8 n = read(r[HL]);
	n = (n >> 1) | (n << 7);

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = (n & 0x80) != 0;
}

template <uint x>
void rr_r() {
	bool c = r[x] & 0x01;
	r[x] = (r[x] >> 1) | (r.f.c << 7);

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

void rr_hl() {
	uint8 n = read(r[HL]);
	bool c = n & 0x01;
	n = (n >> 1) | (r.f.c << 7);

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

template <uint x>
void sla_r() {
	bool c = (r[x] & 0x80) != 0;
	r[x] <<= 1;

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

void sla_hl() {
	uint8 n = read(r[HL]);
	bool c = (n & 0x80) != 0;
	n <<= 1;

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

template <uint x>
void sra_r() {
	bool c = r[x] & 0x01;
	int8 n = int8(r[x]);
	r[x] = n >>= 1;

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

void sra_hl() {
	uint8 n = read(r[HL]);
	bool c = n & 0x01;
	n = ((int8)n >> 1);

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

template <uint x>
void srl_r() {
	bool c = r[x] & 0x01;
	r[x] >>= 1;

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

void srl_hl() {
	uint8 n = read(r[HL]);
	bool c = n & 0x01;
	n >>= 1;

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = c;
}

template <uint x>
void swap_r() {
	r[x] = (r[x] << 4) | (r[x] >> 4);

	r.f.z = r[x] == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = 0;
}

void swap_hl() {
	uint8 n = read(r[HL]);
	n = (n << 4) | (n >> 4);

	write(r[HL], n);

	r.f.z = n == 0;
	r.f.n = 0;
	r.f.h = 0;
	r.f.c = 0;
}

//Bit operations
template <uint b, uint x>
void bit_b_r() {
	r.f.z = (r[x] & (0x01 << b)) == 0;
	r.f.n = 0;
	r.f.h = 1;
}

template <uint b>
void bit_b_hl() {
	uint8 n = read(r[HL]);
	r.f.z = (n & (0x01 << b)) == 0;
	r.f.n = 0;
	r.f.h = 1;
}

template <uint b, uint x>
void set_b_r() {
	r[x] |= 0x01 << b;
}

template <uint b>
void set_b_hl() {
	uint8 n = read(r[HL]);
	n |= 0x01 << b;
	write(r[HL], n);
}

template <uint b, uint x>
void res_b_r() {
	r[x] &= ~(1 << b);
}

template <uint b>
void res_b_hl() {
	uint8 n = read(r[HL]);
	n &= ~(1 << b);
	write(r[HL], n);
}

//Jumps
void jp_nn() {
	uint8 lo = read(r[PC]++);
	uint8 hi = read(r[PC]++);

	r[PC] = (hi << 8) | (lo << 0);

	cycle();
}

template <uint c, bool y>
void jp_cc_nn() {
	uint8 lo = read(r[PC]++);
	uint8 hi = read(r[PC]++);

	if (r.f[c] == y) {
		r[PC] = (hi << 8) | (lo << 0);
		cycle();
	}
}

void jp_hl() {
	r[PC] = r[HL];
}

void jr_n() {
	uint8 x = read(r[PC]++);
	int8 n = reinterpret_cast<int8&>(x);
	r[PC] += n;
	cycle();
}

template <uint c, bool y>
void jr_cc_n() {
	uint8 x = read(r[PC]++);
	int8 n = reinterpret_cast<int8&>(x);

	if (r.f[c] == y) {
		r[PC] += n;
		cycle();
	}
}

//Calls
void call_nn() {
	uint8 lo = read(r[PC]++);
	uint8 hi = read(r[PC]++);
	cycle();

	stackPush(r[PC]);
	r[PC] = (hi << 8) | (lo << 0);
}

template <uint x, bool y>
void call_cc_nn() {
	uint8 lo = read(r[PC]++);
	uint8 hi = read(r[PC]++);

	if (r.f[x] == y) {
		cycle();
		stackPush(r[PC]);
		r[PC] = (hi << 8) | (lo << 0);
	}
}

//Restarts
template <uint n>
void rst_n() {
	cycle();
	stackPush(r[PC]);
	r[PC] = n;
}

//Returns
void ret() {
	r[PC] = stackPop();
	cycle();
}

template <uint x, bool y>
void ret_cc() {
	cycle();
	if (r.f[x] == y) {
		r[PC] = stackPop();
		cycle();
	}
}

void reti() {
	r[PC] = stackPop();
	cycle();
	r.ime = true;
}
