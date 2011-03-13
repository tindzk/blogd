#import "Utils.h"

#define self Utils

sdef(RdString, ExtractName, RdString name) {
	ssize_t pos = String_ReverseFind(name, '.');

	if (pos != String_NotFound) {
		return String_Slice(name, 0, pos);
	}

	return name;
}
