#include "webserv.hpp"

std::map<std::string, std::string> gen_mime()
{
	std::map<std::string, std::string>    mime;

	mime.insert(std::pair<std::string, std::string>(".aac", "audio/aac"));
	mime.insert(std::pair<std::string, std::string>(".abw", "application/x-abiword"));
	mime.insert(std::pair<std::string, std::string>(".arc", "application/octet-stream"));
	mime.insert(std::pair<std::string, std::string>(".avi", "video/x-msvideo"));
	mime.insert(std::pair<std::string, std::string>(".azw", "application/vnd.amazon.ebook"));
	mime.insert(std::pair<std::string, std::string>(".bin", "application/octet-stream"));
	mime.insert(std::pair<std::string, std::string>(".bz", "application/x-bzip"));
	mime.insert(std::pair<std::string, std::string>(".bz2", "application/x-bzip2"));
	mime.insert(std::pair<std::string, std::string>(".csh", "application/x-csh"));
	mime.insert(std::pair<std::string, std::string>(".css", "text/css"));
	mime.insert(std::pair<std::string, std::string>(".csv", "text/csv"));
	mime.insert(std::pair<std::string, std::string>(".doc", "application/msword"));
	mime.insert(std::pair<std::string, std::string>(".epub", "application/epub+zip"));
	mime.insert(std::pair<std::string, std::string>(".gif", "image/gif"));
	mime.insert(std::pair<std::string, std::string>(".htm", "text/html"));
	mime.insert(std::pair<std::string, std::string>(".html", "text/html"));
	mime.insert(std::pair<std::string, std::string>(".ico", "image/x-icon"));
	mime.insert(std::pair<std::string, std::string>(".ics", "text/calendar"));
	mime.insert(std::pair<std::string, std::string>(".jar", "application/java-archive"));
	mime.insert(std::pair<std::string, std::string>(".jpeg", "image/jpeg"));
	mime.insert(std::pair<std::string, std::string>(".jpg", "image/jpeg"));
	mime.insert(std::pair<std::string, std::string>(".js", "application/javascript"));
	mime.insert(std::pair<std::string, std::string>(".json", "application/json"));
	mime.insert(std::pair<std::string, std::string>(".mid", "audio/midi"));
	mime.insert(std::pair<std::string, std::string>(".midi", "audio/midi"));
	mime.insert(std::pair<std::string, std::string>(".mpeg", "video/mpeg"));
	mime.insert(std::pair<std::string, std::string>(".mpkg", "application/vnd.apple.installer+xml"));
	mime.insert(std::pair<std::string, std::string>(".odp", "application/vnd.oasis.opendocument.presentation"));
	mime.insert(std::pair<std::string, std::string>(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
	mime.insert(std::pair<std::string, std::string>(".odt", "application/vnd.oasis.opendocument.text"));
	mime.insert(std::pair<std::string, std::string>(".oga", "audio/ogg"));
	mime.insert(std::pair<std::string, std::string>(".ogv", "video/ogg"));
	mime.insert(std::pair<std::string, std::string>(".ogx", "application/ogg"));
	mime.insert(std::pair<std::string, std::string>(".pdf", "application/pdf"));
	mime.insert(std::pair<std::string, std::string>(".png", "image/png"));
	mime.insert(std::pair<std::string, std::string>(".ppt", "application/vnd.ms-powerpoint"));
	mime.insert(std::pair<std::string, std::string>(".rar", "application/x-rar-compressed"));
	mime.insert(std::pair<std::string, std::string>(".rtf", "application/rtf"));
	mime.insert(std::pair<std::string, std::string>(".sh", "application/x-sh"));
	mime.insert(std::pair<std::string, std::string>(".svg", "image/svg+xml"));
	mime.insert(std::pair<std::string, std::string>(".swf", "application/x-shockwave-flash"));
	mime.insert(std::pair<std::string, std::string>(".tar", "application/x-tar"));
	mime.insert(std::pair<std::string, std::string>(".tif", "image/tiff"));
	mime.insert(std::pair<std::string, std::string>(".tiff", "image/tiff"));
	mime.insert(std::pair<std::string, std::string>(".ttf", "font/ttf"));
	mime.insert(std::pair<std::string, std::string>(".vsd", "application/vnd.visio"));
	mime.insert(std::pair<std::string, std::string>(".wav", "audio/x-wav"));
	mime.insert(std::pair<std::string, std::string>(".weba", "audio/webm"));
	mime.insert(std::pair<std::string, std::string>(".webm", "video/webm"));
	mime.insert(std::pair<std::string, std::string>(".webp", "image/webp"));
	mime.insert(std::pair<std::string, std::string>(".woff", "font/woff"));
	mime.insert(std::pair<std::string, std::string>(".woff2", "font/woff2"));
	mime.insert(std::pair<std::string, std::string>(".xhtml", "application/xhtml+xml"));
	mime.insert(std::pair<std::string, std::string>(".xls", "application/vnd.ms-excel"));
	mime.insert(std::pair<std::string, std::string>(".xml", "application/xml"));
	mime.insert(std::pair<std::string, std::string>(".xul", "application/vnd.mozilla.xul+xml"));
	mime.insert(std::pair<std::string, std::string>(".zip", "application/zip"));
	mime.insert(std::pair<std::string, std::string>(".3gp", "video/3gpp"));
	mime.insert(std::pair<std::string, std::string>(".3g2", "video/3gpp2"));
	mime.insert(std::pair<std::string, std::string>(".7z", "application/x-7z-compressed"));
	return (mime);
}

std::string mime_select(std::string ext)
{
	static std::map<std::string, std::string>    		mime = gen_mime();
	std::map<std::string, std::string>::iterator	it = mime.find(ext);

	if (it != mime.end())
		return (it->second);
	return ("text/html");
}
