#import <YAML.h>
#import <Macros.h>
#import <FileStream.h>
#import <BufferedStream.h>

#import "Categories.h"

#define self Configuration

record(External) {
	String name;
	String url;
};

Array(External, ExternalArray);

class {
	String title;
	String descr;
	String author;
	String copyright;
	String license;
	String url;
	String articles;
	String about;
	String filePath;
	String mail;
	ExternalArray *external;
};

SingletonPrototype(self);

#define Get(name) \
	def(String, Get##name)

Get(Title);
Get(Descr);
Get(Author);
Get(Copyright);
Get(License);
Get(Url);
Get(ArticlePath);
Get(About);
Get(FilePath);
Get(Mail);

#undef Get

def(ExternalArray *, GetExternal);

def(void, Init);
def(void, Destroy);
def(void, Parse, String path);

#undef self
