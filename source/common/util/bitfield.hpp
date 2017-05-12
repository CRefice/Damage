#pragma once

using uint = unsigned int;

template <typename Type, unsigned Bits>
class BitField
{
public:
	enum : Type { Mask = ~0ull >> (64 - Bits)};

	inline BitField() : data(0) {}
	template <typename T>
	inline BitField(const T& value) { set(value); }

	inline operator Type() const { return data; }
	template <typename T>
	inline auto& operator= (const T value) { set(value); return *this; }
	
	inline auto operator++(int) { Type value = data; set(data + 1); return value; }
	inline auto operator--(int) { Type value = data; set(data - 1); return value; }
	inline auto& operator++() { set(data + 1); return *this; }
	inline auto& operator--() { set(data - 1); return *this; }

	inline auto& operator &=(const Type value) { set(data & value); return *this; }
	inline auto& operator |=(const Type value) { set(data | value); return *this; }
	inline auto& operator ^=(const Type value) { set(data ^ value); return *this; }
	inline auto& operator<<=(const Type value) { set(data << value); return *this; }
	inline auto& operator>>=(const Type value) { set(data >> value); return *this; }
	inline auto& operator +=(const Type value) { set(data + value); return *this; }
	inline auto& operator -=(const Type value) { set(data - value); return *this; }
	inline auto& operator *=(const Type value) { set(data * value); return *this; }
	inline auto& operator /=(const Type value) { set(data / value); return *this; }
	inline auto& operator %=(const Type value) { set(data % value); return *this; }

	class Reference
	{
	public:
		inline Reference(BitField& source_, uint lo, uint hi) : source(source_), Hi(hi), Lo(lo) {}
		
		inline operator Type() const {
			const Type RangeBits = Hi - Lo + 1;
			const Type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
			return (source & RangeMask) >> Lo;
		}
		inline auto& operator=(const Type value) {
			const Type RangeBits = Hi - Lo + 1;
			const Type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
			source = (source & ~RangeMask) | ((value << Lo) & RangeMask);
			return *this;
		}

	private:
		BitField& source;
		const uint Hi;
		const uint Lo;
	};

	inline Reference bits(uint lo, uint hi) { return{ *this, lo < hi ? lo : hi, hi > lo ? hi : lo }; }
	inline Reference bit(uint index) { return{ *this, index, index }; }

	inline const Reference bits(uint lo, uint hi) const { return{ (BitField&)*this, lo < hi ? lo : hi, hi > lo ? hi : lo }; }
	inline const Reference bit(uint index) const { return{ (BitField&)*this, index, index }; }

private:
	inline void set(const Type val) { data = val & Mask; }

	Type data;
};

template<uint bits> inline auto sclamp(const intmax_t x) -> intmax_t {
	enum : intmax_t { b = 1ull << (bits - 1), m = b - 1 };
	return (x > m) ? m : (x < -b) ? -b : x;
}
