/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/mime_types.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_mime_types)


BOOST_AUTO_TEST_CASE(test_extension_to_type)
{
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("html"),    "text/html");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("htm"),     "text/html");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("shtml"),   "text/html");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("css"),     "text/css");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("xml"),     "text/xml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("gif"),     "image/gif");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("jpeg"),    "image/jpeg");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("jpg"),     "image/jpeg");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("js"),      "application/javascript");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("atom"),    "application/atom+xml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("rss"),     "application/rss+xml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mml"),     "text/mathml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("txt"),     "text/plain");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("jad"),     "text/vnd.sun.j2me.app-descriptor");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("wml"),     "text/vnd.wap.wml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("htc"),     "text/x-component");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("png"),     "image/png");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("tif"),     "image/tiff");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("tiff"),    "image/tiff");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("wbmp"),    "image/vnd.wap.wbmp");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("ico"),     "image/x-icon");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("jng"),     "image/x-jng");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("bmp"),     "image/x-ms-bmp");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("svg"),     "image/svg+xml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("svgz"),    "image/svg+xml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("webp"),    "image/webp");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("woff"),    "application/font-woff");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("jar"),     "application/java-archive");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("war"),     "application/java-archive");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("ear"),     "application/java-archive");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("json"),    "application/json");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("hqx"),     "application/mac-binhex40");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("doc"),     "application/msword");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("pdf"),     "application/pdf");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("ps"),      "application/postscript");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("eps"),     "application/postscript");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("ai"),      "application/postscript");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("rtf"),     "application/rtf");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("xls"),     "application/vnd.ms-excel");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("eot"),     "application/vnd.ms-fontobject");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("ppt"),     "application/vnd.ms-powerpoint");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("wmlc"),    "application/vnd.wap.wmlc");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("kml"),     "application/vnd.google-earth.kml+xml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("kmz"),     "application/vnd.google-earth.kmz");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("7z"),      "application/x-7z-compressed");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("cco"),     "application/x-cocoa");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("jardiff"), "application/x-java-archive-diff");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("jnlp"),    "application/x-java-jnlp-file");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("run"),     "application/x-makeself");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("pl"),      "application/x-perl");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("pm"),      "application/x-perl");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("prc"),     "application/x-pilot");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("pdb"),     "application/x-pilot");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("rar"),     "application/x-rar-compressed");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("rpm"),     "application/x-redhat-package-manager");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("sea"),     "application/x-sea");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("swf"),     "application/x-shockwave-flash");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("sit"),     "application/x-stuffit");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("tcl"),     "application/x-tcl");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("tk"),      "application/x-tcl");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("der"),     "application/x-x509-ca-cert");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("pem"),     "application/x-x509-ca-cert");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("crt"),     "application/x-x509-ca-cert");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("xpi"),     "application/x-xpinstall");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("xhtml"),   "application/xhtml+xml");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("zip"),     "application/zip");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("bin"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("exe"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("dll"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("deb"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("dmg"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("iso"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("img"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("msi"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("msp"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("msm"),     "application/octet-stream");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("docx"),    "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("xlsx"),    "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("pptx"),    "application/vnd.openxmlformats-officedocument.presentationml.presentation");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mid"),     "audio/midi");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("midi"),    "audio/midi");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("kar"),     "audio/midi");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mp3"),     "audio/mpeg");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("ogg"),     "audio/ogg");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("m4a"),     "audio/x-m4a");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("ra"),      "audio/x-realaudio");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("3gpp"),    "video/3gpp");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("3gp"), "   video/3gpp");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mp4"),     "video/mp4");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mpeg"),    "video/mpeg");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mpg"),     "video/mpeg");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mov"),     "video/quicktime");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("webm"),    "video/webm");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("flv"),     "video/x-flv");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("m4v"),     "video/x-m4v");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("mng"),     "video/x-mng");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("asx"),     "video/x-ms-asf");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("asf"),     "video/x-ms-asf");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("wmv"),     "video/x-ms-wmv");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("avi"),     "video/x-msvideo");
    BOOST_CHECK_EQUAL(reactive::http::mime_types::extension_to_type("bad"),     "text/plain");
}

BOOST_AUTO_TEST_SUITE_END()
