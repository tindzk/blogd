#import "Resource.h"

#define self Static

class {
	RdString _path;

	String file;
	String style;
};

action(_Serve) {
	if (String_Equals  (this->file.rd, $("."))  ||
		String_Equals  (this->file.rd, $("..")) ||
		String_Contains(this->file.rd, $("/")))
	{
		Response_SetStatus(resp, HTTP_Status_ClientError_Forbidden);
		BufferResponse(resp, $$("Access denied."));

		return;
	}

	ConfigurationInstance config = Configuration_GetInstance();

	String path;

	if (this->style.len > 0) {
		path = String_Format($("%/%/%/%"),
			Configuration_GetFilePath(config),
			this->_path, this->style.rd, this->file.rd);
	} else {
		path = String_Format($("%/%/%"),
			Configuration_GetFilePath(config),
			this->_path, this->file.rd);
	}

	FileResponse(resp, path.rd, *(DateTime *) &req.lastModified);

	String_Destroy(&path);
}

action(ServeCss) {
	this->_path = $("css");
	dispatch(_Serve);
}

action(ServeJs) {
	this->_path = $("js");
	dispatch(_Serve);
}

action(ServeImage) {
	this->_path = $("images");
	dispatch(_Serve);
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.members = {
		{ .name = $("file"),  Member(file)  },
		{ .name = $("style"), Member(style) }
	},

	.routes = {
		{ .path   = $("/css/:style/:file"),
		  .action = Action(ServeCss) },

		{ .path   = $("/css/:file"),
		  .action = Action(ServeCss) },

		{ .path   = $("/js/:file"),
		  .action = Action(ServeJs) },

		{ .path   = $("/images/:style/:file"),
		  .action = Action(ServeImage) },

		{ .path   = $("/images/:file"),
		  .action = Action(ServeImage) }
	}
};

RouterConstructor;
