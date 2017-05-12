#pragma once

#include "common\inttypes.hpp"

namespace core
{
enum {
	A, F,
	B, C,
	D, E,
	H, L,
	AF, BC,
	DE, HL,
	SP, PC 
};

enum {
	ZF, NF, HF, CF
};

struct Register
{
	virtual operator uint() const = 0;
	virtual uint operator=(uint x) = 0;

	Register& operator=(const Register& x) {
		operator=((uint)x);
		return *this;
	}

	uint operator++() { return operator=(*this + 1); }
	uint operator--() { return operator=(*this - 1); }
	uint operator++(int) {
		const uint x = *this;
		operator++();
		return x;
	}
	uint operator--(int) {
		const uint x = *this;
		operator--();
		return x;
	}

	uint operator+=(uint x) { return operator=(*this + x); }
	uint operator-=(uint x) { return operator=(*this - x); }
	uint operator*=(uint x) { return operator=(*this * x); }
	uint operator/=(uint x) { return operator=(*this / x); }
	uint operator%=(uint x) { return operator=(*this % x); }
	uint operator&=(uint x) { return operator=(*this & x); }
	uint operator^=(uint x) { return operator=(*this ^ x); }
	uint operator|=(uint x) { return operator=(*this | x); }
	uint operator<<=(uint x) { return operator=(*this << x); }
	uint operator>>=(uint x) { return operator=(*this >> x); }
};

struct Register8 : Register
{
public:
	operator uint() const final { return data; }
	uint operator=(uint x) final { return data = x; }

private:
	uint8 data;
};

struct Register16 : Register
{
public:
	operator uint() const final { return data; }
	uint operator=(uint x) final { return data = x; }

private:
	uint16 data;
};

struct RegisterF : Register
{
	operator uint() const final {
		return (z << 7)
			   | (n << 6)
				 | (h << 5)
				 | (c << 4);
	}
	uint operator=(uint x) final {
		z = x & 0x80;
		n = x & 0x40;
		h = x & 0x20;
		c = x & 0x10;
		return x;
	}
	
	bool& operator[](uint x) {
		static bool* const regs[] = { &z, &n, &h, &c };
		return *regs[x];
	}

	bool z, n, h, c;
};

struct RegisterUnion : Register
{
public:
	RegisterUnion(Register& hi, Register& lo) : hi(hi), lo(lo) {}
	operator uint() const final { return (hi << 8) | lo; }
	uint operator=(uint x) final {
		hi = x >> 8;
		lo = x & 0xFF;
		return *this;
	}

private:
	Register& hi;
	Register& lo;
};

struct Registers
{
	Register8 a;
	RegisterF f;
	Register8 b;
	Register8 c;
	Register8 d;
	Register8 e;
	Register8 h;
	Register8 l;

	Register16 sp;
	Register16 pc;

	RegisterUnion af{a, f};
	RegisterUnion bc{b, c};
	RegisterUnion de{d, e};
	RegisterUnion hl{h, l};

	bool stop;
	bool halt;
	bool ime;
	bool ei;

	Register& operator[](uint x) {
		static Register* const regs[] = {
			&a, &f, &b, &c, &d, &e, &h, &l, &af, &bc, &de, &hl, &sp, &pc
		};
		return *regs[x];
	}
};
}
