#include "gm.h"
#include <cstdio>

using namespace shb;

gmexport double buffer_exists(double id) {
	return getBuffer(id).bufferExists() ? 1 : 0;
}

gmexport double buffer_get_read_pos(double id) {
	return getBuffer(id).getReadPos();
}

gmexport double buffer_get_write_pos(double id) {
	return getBuffer(id).getWritePos();
}

gmexport double buffer_get_length(double id) {
	return getBuffer(id).getLength();
}

gmexport double buffer_set_read_pos(double id, double readpos) {
	getBuffer(id).setReadPos(gm_double_to_X<size_t>(readpos));
	return 0;
}

gmexport double buffer_set_write_pos(double id, double writepos) {
	getBuffer(id).setWritePos(gm_double_to_X<size_t>(writepos));
	return 0;
}

gmexport double buffer_set_length(double id, double length) {
	return getBuffer(id).setLength(gm_double_to_X<size_t>(length)) ? 1 : 0;
}

gmexport double buffer_destroy(double id) {
	getBuffer(id).destroy();
	return 0;
}

gmexport double buffer_read_from_file(double id, char* filename) {
	FILE *f = fopen(filename, "rb");
	if(f == NULL) {
		return -1;	// File not found
	}
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);
	if(size < 0) {
		fclose(f);
		return -2;	// File too large
	}

	BufferProxy buffer = getBuffer(id);
	if(!buffer.setLength(size)) {
		fclose(f);
		buffer.setLength(0);
		return -3;	// Buffer too small
	}

	size_t writePos = 0;
	BufferFragment fragment;
	while(fragment = buffer.getFragment(writePos), fragment.isValid()) {
		if(fread(fragment.getStart(), sizeof(uint8_t), fragment.getLength(), f) != fragment.getLength()) {
			fclose(f);
			buffer.setLength(0);
			return -4;	// Read error
		} else {
			writePos += fragment.getLength();
		}
	}

	buffer.setReadPos(0);
	buffer.setWritePos(writePos);
	fclose(f);
	return 1;	// Success
}

gmexport double buffer_write_to_file(double id, char* filename) {
	FILE *f = fopen(filename, "wb");
	if(f == NULL) {
		return -1;	// Cannot open file
	}

	BufferProxy buffer = getBuffer(id);

	size_t pos = 0;
	BufferFragment fragment;
	while(fragment = buffer.getFragment(pos), fragment.isValid()) {
		if(fwrite(fragment.getStart(), sizeof(uint8_t), fragment.getLength(), f) != fragment.getLength()) {
			fclose(f);
			return -2;	// I/O-Error writing to the file
		} else {
			pos += fragment.getLength();
		}
	}

	fclose(f);
	return 1;	// Success
}
