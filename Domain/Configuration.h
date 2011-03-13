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
	Logger *logger;
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
	String flattr;
	ExternalArray *external;
};

#define Get(name) \
	def(RdString, Get##name)

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
Get(Flattr);

#undef Get

def(ExternalArray *, GetExternal);

rsdef(self, New);
def(void, Destroy);
def(void, SetLogger, Logger *logger);
def(void, Parse, RdString path);

SingletonPrototype(self);

#undef self
