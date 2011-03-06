#import <Array.h>
#import <String.h>

#import "Article.h"

#define self Category

record(self) {
	CarrierString name;
	Articles *articles;
};

Array(self, CategoryArray);

#undef self
