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
	long signedSize = ftell(f);
	fseek(f, 0, SEEK_SET);
	if(signedSize < 0) {
		fclose(f);
		return -2;	// File too large
	}

	size_t size = signedSize;
	BufferProxy buffer = getBuffer(id);
	buffer.setReadPos(0);
	buffer.setWritePos(0);
	if(!buffer.setLength(size)) {
		fclose(f);
		buffer.setLength(0);
		return -3;	// Buffer too small
	}

	size_t copied = 0;
	uint8_t tempBuffer[4096];
	while(copied < size) {
		size_t copySize = std::min(size - copied, (size_t)4096);
		if(fread(tempBuffer, sizeof(uint8_t), copySize, f) != copySize) {
			fclose(f);
			buffer.setLength(0);
			return -4;	// Read error
		} else {
			buffer.write(tempBuffer, copySize);
			copied += copySize;
		}
	}

	fclose(f);
	return 1;	// Success
}

gmexport double buffer_write_to_file(double id, char* filename) {
	FILE *f = fopen(filename, "wb");
	if(f == NULL) {
		return -1;	// Cannot open file
	}

	BufferProxy buffer = getBuffer(id);
	size_t oldReadPos = buffer.getReadPos();
	buffer.setReadPos(0);

	size_t copied = 0;
	size_t size = buffer.getLength();
	uint8_t tempBuffer[4096];
	while(copied < size) {
		size_t copySize = std::min(size - copied, (size_t)4096);
		buffer.read(tempBuffer, copySize);
		if(fwrite(tempBuffer, sizeof(uint8_t), copySize, f) != copySize) {
			buffer.setReadPos(oldReadPos);
			fclose(f);
			return -2;	// I/O-Error writing to the file
		} else {
			copied += copySize;
		}
	}

	buffer.setReadPos(oldReadPos);
	fclose(f);
	return 1;	// Success
}
