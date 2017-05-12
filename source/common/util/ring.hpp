#pragma once

template<typename T>
struct ring
{
	ring() = default;

	ring(const ring& source) {
		operator=(source);
	}

	ring(ring&& source) {
		operator=(std::move(source));
	}

	ring& operator=(const ring& source) {
		if (this == &source) return *this;
		reset();
		_size = source._size;
		_data = new T[_size];
		for (auto n : range(_size)) _data[n] = source._data[n];
		_read = source._read;
		_write = source._write;
		return *this;
	}

	ring& operator=(ring&& source) {
		if (this == &source) return *this;
		_data = source._data;
		_size = source._size;
		_read = source._read;
		_write = source._write;
		source._data = nullptr;
		source.reset();
	}

	~ring() {
		reset();
	}

	explicit operator bool() const {
		return _size;
	}

	size_t size() const {
		return _size;
	}

	T* data() {
		return _data;
	}

	const T* data() const {
	  return _data;
	}

	void reset() {
		delete[] _data;
		_data = nullptr;
		_size = 0;
		_read = 0;
		_write = 0;
	}

	void resize(size_t size) {
		reset();
		_size = size;
		_data = new T[_size];
		for (auto n = 0; n < _size; ++n) _data[n] = T();
	}

	bool pending() const {
		return _read != _write;
	}

	T read() {
		T result = _data[_read];
		if (++_read >= _size) _read = 0;
		return result;
	}

	T last() const {
		return _data[_write];
	}

	void write(const T& value) {
		_data[_write] = value;
		if (++_write >= _size) _write = 0;
	}

private:
	T* _data = nullptr;
	size_t _size = 0;
	size_t _read = 0;
	size_t _write = 0;
};
